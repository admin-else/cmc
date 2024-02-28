import os
import re

type_map = {
    #  TYPE  |code tpye             |method type       |is heap
    "b": ["char ", "char", False],
    "B": ["unsigned char ", "byte", False],
    "h": ["short ", "short", False],
    "H": ["unsigned short ", "ushort", False],
    "i": ["int ", "int", False],
    "I": ["unsigned int ", "uint", False],
    "l": ["long ", "long", False],
    "L": ["unsigned long ", "ulong", False],
    "f": ["float ", "float", False],
    "d": ["double ", "double", False],
    "q": ["long long ", "llong", False],
    "Q": ["unsigned long long ", "ullong", False],
    "?": ["bool ", "bool", False],
    "v": ["int ", "varint", False],
    "s": ["char *", "string", True],
    "p": ["cmc_block_pos ", "position", False],
    "n": ["cmc_nbt_node *", "nbt", True],
    "a": ["cmc_buffer *", "byte_array", True],
    "S": ["cmc_slot *", "slot", True],
    "m": ["cmc_entity_metadata ", "entity_metadata", True],
}


def replace_code_segments(replacement_code, tag):
    start_tag = f"// CGSS: {tag}"  # Code Generator Segment Start
    end_tag = f"// CGSE: {tag}"  # Code Generator Segment End

    for file_name in os.listdir("./src"):
        if file_name.endswith(".c") or file_name.endswith(".h"):
            file_path = os.path.join("./src", file_name)

            with open(file_path, "r") as file:
                file_content = file.read()

            pattern = re.compile(
                f"({re.escape(start_tag)}).*?({re.escape(end_tag)})", re.DOTALL
            )
            if not pattern.search(file_content):
                continue

            file_content = pattern.sub(rf"\1\n{replacement_code}\n\2", file_content)

            with open(file_path, "w") as file:
                file.write(file_content)

            return

    raise ValueError(f"didnt find tag {tag}")


def type_def(inp):
    if inp["is_empty"]:
        return ""
    return f"typedef struct {{{''.join([f'{type_map[symbol[0]][0]}{symbol[1:]};' for symbol in inp['packet_type_content']])}}} {inp['name']}_packet;\n\n"
    


def unpack_method(inp):
    if inp["is_empty"]:
        return ""
    code = (
        f"{inp['name']}_packet *unpack_{inp['name']}_packet(cmc_buffer *buff) {{{inp['name']}_packet *packet = MALLOC(sizeof(*packet));"
    )
    code += "switch(buff->protocol_version) {"
    for pv, data in inp["packet_data"].items():
        code += f"case {pv}: {{" + "".join(
            [
                f"packet->{symbol[1:]}=cmc_buffer_unpack_{type_map[symbol[0]][1]}(buff);"
                for symbol in data["content"]
            ]
        ) + "break;}"
    code += """default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}\n\n"""
    return code

def send_method(inp):
    code = "void cmc_send_{}_packet(cmc_conn *conn{})".format(inp['name'], f", {inp['name']}_packet *packet" if not inp["is_empty"] else "")
    code += "{cmc_buffer *buff = cmc_buffer_init();"
    code += "switch(buff->protocol_version) {"
    for pv, data in inp["packet_data"].items():
        code += f"case {pv}: {{" 
        code += f"cmc_buffer_pack_varint(buff, {data['packet_id']});"
        if "content" in data:
            code += "".join(
                [
                    f"packet->{symbol[1:]}=cmc_buffer_unpack_{type_map[symbol[0]][1]}(buff);"
                    for symbol in data["content"]
                ]
            )
        code += "break;}"
    code += """default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }\n\n"""

    return code

def packet_ids(mc_packet_exps):
    packet_states = set()
    for exp in mc_packet_exps:
        packet_states.add("_".join(exp.split("_")[:2]))

    code = ""
    for state in packet_states:
        code += f"enum packetids_{state} {{"
        for exp in mc_packet_exps:
            if exp.startswith(state):
                code += f"CMC_PACKETID_{state.upper()}_{'_'.join(exp.split('_')[2:]).split(';')[0].upper()} = {exp.split(';')[1]},"
        code += "};"

    return code

def free_method(inp):
    if not inp['is_heap']:
        return ""

    code = f"void cmc_free_{inp['name']}_packet({inp['name']}_packet packet) {{"
    for sym in symbols:
        if type_map[sym[0]][2]:
            code += f"free_{type_map[sym[0]][1]}(packet.{sym[1:]});"
    code += "}"
    return code

def loop_handler(exp):
    packet_name, packet_id, symbol_str = exp.split(";", maxsplit=2)
    short_packet_name = "_".join(packet_name.split("_")[2:])
    return f"""case CMC_PACKETID_S2C_PLAY_{short_packet_name.upper()}: {{
        if (!conn->on_packet.{short_packet_name})
          goto unhandeled_packet;
        S2C_play_{short_packet_name}_packet data =
            ERR_ABLE(unpack_S2C_play_{short_packet_name}_packet(packet), goto err_free_packet;);
        conn->on_packet.{short_packet_name}(data, conn);
        {f'cmc_free_S2C_play_{short_packet_name}_packet(data);' if should_have_free_method(exp) else ''}
        break;
    }}"""

def gather_packets():
    out = {}
    for fname in os.listdir("packets"):
        with open("packets/" + fname, "r") as f:
            vid = int(fname.split(".")[0])
            for exp in f.read().replace(" ", "").split("\n"):
                if exp.find("#") != -1:
                    exp = exp[:exp.find("#")]
                if not exp:
                    continue
                
                packet_name, packet_id, symbol_str = exp.split(";", maxsplit=2)
                
                if packet_name not in out:
                    out[packet_name] = {}
                    out[packet_name]["is_empty"] = True
                if "packet_data" not in out[packet_name]:
                    out[packet_name]["packet_data"] = {}
                if vid not in out[packet_name]["packet_data"]:
                    out[packet_name]["packet_data"][vid] = {}
                if symbol_str:
                    out[packet_name]["packet_data"][vid]["content"] = symbol_str.split(";")
                    out[packet_name]["is_empty"] = False

                    out[packet_name]["is_heap"] = False
                    content = set()
                    for _, val in out[packet_name]["packet_data"].items():
                        for field in val["content"]:
                            content.add(field)
                            if type_map[field[0]][2]:
                                out[packet_name]["is_heap"] = True
                    
                    out[packet_name]["packet_type_content"] = content

                out[packet_name]["packet_data"][vid]["packet_id"] = packet_id
                out[packet_name]["name"] = packet_name

    return list(out.values())

def packet_name_id_define(exps):
    names = set([exp["name"].upper() for exp in exps])
    code = "typedef enum {"
    for name in names:
        code += f"CMC_{name}_NAME_ID,"
    code += "}cmc_packet_name_id;"
    return code

def main():
    mc_packet_exps = gather_packets()
    
    replace_code_segments(packet_name_id_define(mc_packet_exps), "packet_name_id_define")

    # packet unpack methods
    replace_code_segments(
        "".join([unpack_method(mc_packet_exp) for mc_packet_exp in mc_packet_exps]),
        "unpack_methods_c",
    )
    replace_code_segments(
        "".join([f"{inp['name']}_packet *unpack_{inp['name']}_packet(cmc_buffer *buff);" if not inp['is_empty'] else "" for inp in mc_packet_exps]),
        "unpack_methods_h",
    )
    # packet unpack methods header

    replace_code_segments(
        "".join([send_method(mc_packet_exp) for mc_packet_exp in mc_packet_exps]),
        "send_methods_c",
    )

    # packet send methods header
    replace_code_segments(
        "".join(["void cmc_send_{}_packet(cmc_conn *conn{});".format(inp['name'], f", {inp['name']}_packet *packet" if not inp["is_empty"] else "") for inp in mc_packet_exps]),
        "send_methods_h",
    )

    # type defs
    replace_code_segments(
        "".join([type_def(mc_packet_exp) for mc_packet_exp in mc_packet_exps]),
        "packet_types",
    )

    replace_code_segments(
        "\n".join([free_method(sym) for sym in mc_packet_exps]), "free_methods_c"
    )

    replace_code_segments(
        "\n".join([f"void cmc_free_{exp.split(';', maxsplit=2)[0]}_packet({exp.split(';', maxsplit=2)[0]}_packet packet);" for exp in mc_packet_exps if should_have_free_method(exp)]), "free_methods_h"
    )

if __name__ == "__main__":
    main()