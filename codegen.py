import os
import re
from pprint import pprint

type_map = {
#  TYPE  |code tpye               |method type      |is heap |default value
    "b": ["int8_t ",              "char",            False,  "0",                       ],
    "B": ["uint8_t ",             "byte",            False,  "0",                       ],
    "h": ["int16_t ",             "short",           False,  "0",                       ],
    "H": ["uint16_t ",            "ushort",          False,  "0",                       ],
    "i": ["int32_t ",             "int",             False,  "0",                       ],
    "I": ["uint32_t ",            "uint",            False,  "0",                       ],
    "l": ["int64_t ",             "long",            False,  "0",                       ],
    "L": ["uint64_t ",            "ulong",           False,  "0",                       ],
    "f": ["float ",               "float",           False,  "0",                       ],
    "d": ["double ",              "double",          False,  "0",                       ],
    "?": ["bool ",                "bool",            False,  "false",                   ],
    "v": ["int32_t ",             "varint",          False,  "0",                       ],
    "s": ["char *",               "string",          True,   "NULL",                    ],
    "p": ["cmc_block_pos ",       "position",        False,  "{.x=0,.y=0,.z=0}",        ],
    "n": ["cmc_nbt *",            "nbt",             True,   "NULL",                    ],
    "a": ["cmc_buffer *",         "byte_array",      True,   "NULL",                    ],
    "S": ["cmc_slot *",           "slot",            True,   "NULL",                    ],
    "m": ["cmc_entity_metadata ", "entity_metadata", True,   "{.size=0,.entries=NULL}", ],
    "u": ["cmc_uuid ",             "uuid",           False,  "{.lower=0,.upper=0}",     ],
    "A": ["cmc_array ",            None,             True,   "{.data=NULL,.len=0}",     ],
}

def split_array_exp(inp):
    name = inp[:inp.find("[")]
    inp = inp[inp.find("[")+1:]
    deepness = 0
    for i, c in enumerate(inp):
        if c == "[":
            deepness += 1
        if c == "]":
            deepness -= 1
            if deepness == -1:
                break
    return name[1:], inp[:i], inp[i+1:]

def careful_split(exp):
    out = []
    curr_str = ""
    deepness = 0
    for c in exp:
        if c == ";" and deepness == 0:
            out.append(curr_str)
            curr_str = ""
        else:
            curr_str += c
        
        if c == "[":
            deepness += 1
        elif c == "]":
            deepness -= 1
            if deepness < 0:
                raise ValueError("closed nonexistent array") 
    out.append(curr_str)
    return out

def replace_code_segments(replacement_code, tag):
    start_tag = f"// CGSS: {tag}"  # Code Generator Segment Start
    end_tag   = f"// CGSE: {tag}"  # Code Generator Segment End

    for file_name in os.listdir("./src").extend(os.listdir("./include/cmc")):
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


def type_def_content(token, packet_name, wrap_name):
    code = ""
    other_typedef = ""
    for sym in careful_split(token):
        exp_data = sym[1:]
        exp_type = sym[0]
        if exp_type == "A":
            name, array_exp, key = split_array_exp(sym)
            other_typedef += type_def_content(array_exp, packet_name, f"{packet_name}_{name}") 
            code += f"{type_map[exp_type][0]} {name};"
        else:
            code += f"{type_map[exp_type][0]} {exp_data};"
    code = f"typedef struct {{{code}}} {wrap_name};"
    return other_typedef + code


def type_def(inp):
    if inp["is_empty"]:
        return ""
    code = type_def_content(inp["type_def_content_str"], inp["name"], f"{inp['name']}_packet")    
    return code

def unpack_method_content(to_unpack_to, exp, deepness, packet_name):
    code = ""
    for sym in careful_split(exp):
        exp_type = sym[0]
        exp_data = sym[1:]
        if exp_type == "A":
            name, array_exp, key = split_array_exp(sym)
            deepnessc = chr(deepness)
            code += f"""{to_unpack_to}{name}.len = {to_unpack_to}{key};
{to_unpack_to}{name}.data = MALLOC({to_unpack_to}{name}.len * sizeof({packet_name}_{name}));
for(int {deepnessc} = 0; i < {to_unpack_to}{name}.len; ++{deepnessc}) {{
{packet_name}_{name} *p_{name} = {deepnessc} * sizeof(*p_{name}) + ({packet_name}_{name} *){to_unpack_to}{name}.data;"""
            code += unpack_method_content(f"p_{name}->", array_exp, deepness+1, packet_name)
            code += "}"
        else:
            code += f"{to_unpack_to}{exp_data}=cmc_buffer_unpack_{type_map[sym[0]][1]}(buff);"
    return code

def unpack_method(inp):
    if inp["is_empty"]:
        return ""
    code = f"{inp['name']}_packet unpack_{inp['name']}_packet(cmc_buffer *buff)"
    empty = "{" 
    #empty += ",".join([f".{entry[1:]}={type_map[entry[0]][3]}" for entry in inp['type_def_content']])
    for entry in inp['type_def_content']:
        name = entry[1:]
        if entry[0] == "A":
            name, *_ = split_array_exp(entry)
        empty += f".{name}={type_map[entry[0]][3]},"
    empty += "}"
    code += f"{{{inp['name']}_packet packet = {empty};"
    code += "switch(buff->protocol_version) {"
    for pv, data in inp["packet_data"].items():
        code += f"case {pv}: {{" + unpack_method_content("packet.", data['content_str'], ord("i"), inp['name']) + "break;}"
    code += f"""default: 
                 ERR(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
               }}
               ERR_CHECK(goto err;);
               if(buff->position != buff->length) ERR(CMC_ERR_BUFFER_UNDERUN, goto err;);
               return packet;
               err:
                 cmc_free_{inp['name']}_packet(&packet);
                 return ({inp['name']}_packet){empty};
               }}\n\n"""
    return code

def send_method_content(data, to_send, deepness, packet_name):
    code = ""
    for symbol in careful_split(data):
        name = symbol[1:]
        if symbol[0] == "A":
            name, array_exp, key = split_array_exp(symbol)
            code += f"for(int {chr(deepness)} = 0; {chr(deepness)} < {to_send}{name}.len; ++{chr(deepness)}) {{"
            code += f"{packet_name}_{name} *p_{name} = {chr(deepness)} * sizeof({packet_name}_{name}) + {to_send}{name}.data;"
            code += send_method_content(array_exp, f"p_{name}->", deepness+1, packet_name)
            code += "}"
        else:
            code += f"cmc_buffer_pack_{type_map[symbol[0]][1]}(buff, {to_send}{name});"
    return code


def send_method(inp):
    code = "void cmc_send_{}_packet(cmc_conn *conn{})".format(inp['name'], f", {inp['name']}_packet *packet" if not inp["is_empty"] else "")
    code += "{cmc_buffer *buff = cmc_buffer_init(conn->protocol_version);"
    code += "switch(conn->protocol_version) {"
    for pv, data in inp["packet_data"].items():
        code += f"case {pv}: {{" 
        code += f"cmc_buffer_pack_varint(buff, {data['packet_id']});"
        if "content" in data:
            code += send_method_content(data["content_str"], "packet->", ord("i"), inp['name'])
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

def free_method_content(exp, tofree, deepness, packet_name):
    code = ""
    for sym in careful_split(exp):
        if type_map[sym[0]][2]:
            exp_type = sym[0]
            exp_data = sym[1:]
            if exp_type == "A":
                name, array_exp, key = split_array_exp(sym)
                deepnessc = chr(deepness)
                code += f"""for(int {deepnessc} = 0; {tofree}->{name}.len; ++{deepnessc}) {{
{packet_name}_{name} *p_{name} = {deepnessc} * sizeof(*p_{name}) + ({packet_name}_{name} *){tofree}->{name}.data;"""
                code += free_method_content(array_exp, f"p_{name}", deepness+1, packet_name)
                code += f"""}}
FREE({tofree}->{name}.data);
{tofree}->{name}.len = 0;"""
            else:
                code += f"free_{type_map[exp_type][1]}({tofree}->{exp_data});"
    return code

def free_method(inp):
    if inp["is_empty"]:
        return ""
    
    code = f"void cmc_free_{inp['name']}_packet({inp['name']}_packet *packet) {{"
    if inp['is_heap']:
        code += free_method_content(inp['type_def_content_str'], "packet", ord("i"), inp["name"])
    else:
        code += "(void)packet;"
    code += "}"
    return code

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
                    out[packet_name]["is_heap"] = False

                if "packet_data" not in out[packet_name]:
                    out[packet_name]["packet_data"] = {}
                if vid not in out[packet_name]["packet_data"]:
                    out[packet_name]["packet_data"][vid] = {}
                if symbol_str:
                    out[packet_name]["packet_data"][vid]["content"] = careful_split(symbol_str)
                    out[packet_name]["packet_data"][vid]["content_str"] = symbol_str
                    out[packet_name]["is_empty"] = False

                    content = set()
                    for _, val in out[packet_name]["packet_data"].items():
                        for field in val["content"]:
                            for existing_field in content:
                                if not existing_field == field and existing_field[1:] == field[1:]:
                                    print(f"fatal conflict between {existing_field} and {field} on {packet_name}!!!")
                                    exit(1)
                            content.add(field)
                            if type_map[field[0]][2]:
                                out[packet_name]["is_heap"] = True
                                
                    
                    out[packet_name]["type_def_content"] = content
                    out[packet_name]["type_def_content_str"] = ";".join(content)

                out[packet_name]["packet_data"][vid]["packet_id"] = packet_id
                out[packet_name]["name"] = packet_name

    return list(out.values())

def packet_name_id_define(exps):
    names = set([exp["name"].upper() for exp in exps])
    code = ""
    for name in names:
        code += f"CMC_{name}_NAME_ID,"
    return code

def packet_id_to_packet_name_id(inp):
    code = ""
    for packet in inp:
        direction, state, *_ = packet["name"].split("_")
        for protcole_version, data in packet['packet_data'].items():
            code += f"case (COMBINE_VALUES({data['packet_id']}, CMC_CONN_STATE_{state.upper()}, CMC_DIRECTION_{direction}, {protcole_version})): return CMC_{packet['name'].upper()}_NAME_ID;"
    return code

def main():
    mc_packet_exps = gather_packets()
    #pprint(mc_packet_exps)

    replace_code_segments("".join([f"HELPER(CMC_{inp['name'].upper()}_NAME_ID);" for inp in mc_packet_exps]), "packet_name_id_string")


    replace_code_segments(packet_name_id_define(mc_packet_exps), "packet_name_id_define")

    # packet unpack methods
    replace_code_segments(
        "".join([unpack_method(mc_packet_exp) for mc_packet_exp in mc_packet_exps]),
        "unpack_methods_c",
    )
    replace_code_segments(
        "".join([f"{inp['name']}_packet unpack_{inp['name']}_packet(cmc_buffer *buff);" if not inp['is_empty'] else "" for inp in mc_packet_exps]),
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
        "\n".join([f"void cmc_free_{inp['name']}_packet({inp['name']}_packet *packet);" for inp in mc_packet_exps if not inp["is_empty"]]), "free_methods_h"
    )
    with open("errors.txt", "r") as f:
        errs = [e for e in f.read().split() if e and not e.startswith("#")]
        replace_code_segments(",".join(["CMC_ERR_" + e.upper() for e in errs if e]), "error_def")
        replace_code_segments("\n".join([f"ERRID2STR_HELPER(CMC_ERR_{e})" for e in errs if e]), "error_helper")

    replace_code_segments(packet_id_to_packet_name_id(mc_packet_exps), "packet_id_to_packet_name_id")

if __name__ == "__main__":
    main()