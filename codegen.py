import os
import re
from pprint import pprint
import glob

type_map = {
#  TYPE  |code tpye               |method type      |is heap |default value
    "b": ["int8_t ",              "char",            False,  "0",                       False],
    "B": ["uint8_t ",             "byte",            False,  "0",                       False],
    "h": ["int16_t ",             "short",           False,  "0",                       False],
    "H": ["uint16_t ",            "ushort",          False,  "0",                       False],
    "i": ["int32_t ",             "int",             False,  "0",                       False],
    "I": ["uint32_t ",            "uint",            False,  "0",                       False],
    "l": ["int64_t ",             "long",            False,  "0",                       False],
    "L": ["uint64_t ",            "ulong",           False,  "0",                       False],
    "f": ["float ",               "float",           False,  "0",                       False],
    "d": ["double ",              "double",          False,  "0",                       False],
    "?": ["bool ",                "bool",            False,  "false",                   False],
    "v": ["int32_t ",             "varint",          False,  "0",                       False],
    "s": ["char *",               "string",          True,   "NULL",                    False],
    "p": ["cmc_block_pos ",       "position",        False,  "{.x=0,.y=0,.z=0}",        False],
    "n": ["cmc_nbt *",            "nbt",             True,   "NULL",                    True ],
    "a": ["cmc_buff *",           "buff",            True,   "NULL",                    False],
    "S": ["cmc_slot *",           "slot",            True,   "NULL",                    True ],
    "m": ["cmc_entity_metadata ", "entity_metadata", True,   "{.size=0,.entries=NULL}", True ],
    "u": ["cmc_uuid ",             "uuid",           False,  "{.lower=0,.upper=0}",     False],
    "A": ["cmc_array ",            None,             True,   "{.data=NULL,.size=0}",    False],
}

replacement_paths = ["src/*.c", "include/cmc/*.h"]

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

def get_absolute_paths(file_paths):
    absolute_paths = []
    for file_path in file_paths:
        matches = glob.glob(file_path)
        for match in matches:
            absolute_path = os.path.abspath(match)
            absolute_paths.append(absolute_path)
    return absolute_paths

def replace_code_segments(replacement_code, tag):
    start_tag = f"// CGSS: {tag}"  # Code Generator Segment Start
    end_tag   = f"// CGSE: {tag}"  # Code Generator Segment End
    pattern = re.compile(
        f"({re.escape(start_tag)}).*?({re.escape(end_tag)})", re.DOTALL
    )

    for file_path in get_absolute_paths(replacement_paths):
        with open(file_path, "r") as file:
            file_content = file.read()
        if not pattern.search(file_content):
            continue
        file_content = pattern.sub(rf"\1\n{replacement_code}\n\2", file_content)
        with open(file_path, "w") as file:
            file.write(file_content)
        return

    raise ValueError(f"didnt find tag {tag}")


def type_def_content(token, packet_name, wrap_name):
    typedefs = []
    members = []
    for sym in careful_split(token):
        exp_data = sym[1:]
        exp_type = sym[0]
        if exp_type == "A":
            name, array_exp, _ = split_array_exp(sym)
            typedefs.append(type_def_content(array_exp, packet_name, f"{packet_name}_{name}"))
            members.append(f"{type_map[exp_type][0]} {name};")
        else:
            members.append(f"{type_map[exp_type][0]} {exp_data};")

    typedefs.append(f"""
        typedef struct {{
            {''.join(members)}
        }} {wrap_name};
    """)

    return "\n".join(typedefs)


def type_def(inp):
    if inp["is_empty"]:
        return ""
    return type_def_content(inp["type_def_content_str"], inp["name"], f"{inp['name']}_packet")

def unpack_method_content(to_unpack_to, exp, deepness, packet_name):
    def handle_value(sym):
        exp_type = sym[0]
        exp_data = sym[1:]
        exp_ctype = type_map[exp_type][1]
        return f"{to_unpack_to}{exp_data} = cmc_buff_unpack_{exp_ctype}(buff);"

    def handle_array(sym):
        name, array_exp, key = split_array_exp(sym)
        i = chr(deepness)
        return f"""
            {to_unpack_to}{name}.size = {to_unpack_to}{key};
            {to_unpack_to}{name}.data = CMC_ERRB_ABLE(cmc_malloc({to_unpack_to}{name}.size * sizeof({packet_name}_{name}), &buff->err), goto err;);
            for (size_t {i} = 0; {i} < {to_unpack_to}{name}.size; ++{i}) {{
                {packet_name}_{name} *p_{name} = &(({packet_name}_{name} *){to_unpack_to}{name}.data)[{i}];
                {unpack_method_content(f'p_{name}->', array_exp, deepness + 1, packet_name)}
            }}
        """

    return "".join(
        handle_array(sym) if (sym[0] == "A") else handle_value(sym)
        for sym in careful_split(exp)
    )

def unpack_method(inp):
    if inp["is_empty"]:
        return ""

    return "".join((
        f"""
        {inp['name']}_packet unpack_{inp['name']}_packet(cmc_buff *buff) {{
            {inp['name']}_packet packet = {{}};
            switch(buff->protocol_version) {{
        """,
        *(
            f"""
            case CMC_PROTOCOL_VERSION_{pv}: {{
                {unpack_method_content('packet.', data['content_str'], ord('i'), inp['name'])}
                break;
            }}
            """
            for pv, data in inp["packet_data"].items()
        ),
        f"""
            default:
                CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
            }}
            CMC_ERRB_ABLE(,goto err);
            if(buff->position != buff->length) CMC_ERRB(CMC_ERR_BUFF_UNDERFLOW, goto err;);
            return packet;
            err:
                cmc_free_{inp['name']}_packet(&packet, &buff->err);
                return ({inp['name']}_packet){{}};
        }}
        """,
    ))

def send_method_content(data, to_send, deepness, packet_name):
    def handle_value(symbol):
        name = symbol[1:]
        return f"cmc_buff_pack_{type_map[symbol[0]][1]}(buff, {to_send}{name});"

    def handle_array(symbol):
        name, array_exp, _ = split_array_exp(symbol)
        i = chr(deepness)
        return f"""
            for (size_t {i} = 0; {i} < {to_send}{name}.size; ++{i}) {{
                {packet_name}_{name} *p_{name} =  &(({packet_name}_{name} *){to_send}{name}.data)[{i}];
                {send_method_content(array_exp, f'p_{name}->', deepness+1, packet_name)}
            }}
        """

    return "".join(
        handle_array(symbol) if (symbol[0] == "A") else handle_value(symbol)
        for symbol in careful_split(data)
    )


def send_method(inp):
    second_param = "" if inp["is_empty"] else f", {inp['name']}_packet *packet"
    return "".join((
        f"""
        cmc_err cmc_send_{inp['name']}_packet(cmc_conn *conn{second_param}) {{
            cmc_buff *buff = cmc_buff_init(conn->protocol_version);
            switch(conn->protocol_version) {{
        """,
        *(
            f"""
            case CMC_PROTOCOL_VERSION_{pv}: {{
                cmc_buff_pack_varint(buff, {data['packet_id']});
                {
                    send_method_content(data['content_str'], 'packet->', ord('i'), inp['name'])
                    if 'content' in data else ''
                }
                break;
            }}
            """
            for pv, data in inp["packet_data"].items()
        ),
        f"""
            default:
                cmc_buff_free(buff);
                CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
            }}
            cmc_conn_send_packet(conn, buff);
            cmc_buff_free(buff);
            return CMC_ERR_NO;
        }}
        """,
    ))

def packet_ids(mc_packet_exps):
    packet_states = ["_".join(exp.split("_")[:2]) for exp in mc_packet_exps]

    code = ""
    for state in packet_states:
        code += f"enum packetids_{state} {{"
        for exp in mc_packet_exps:
            if exp.startswith(state):
                code += f"CMC_PACKETID_{state.upper()}_{'_'.join(exp.split('_')[2:]).split(';')[0].upper()} = {exp.split(';')[1]},"
        code += "};"

    return code

def free_method_content(exp, tofree, deepness, packet_name):
    def handle_value(sym):
        exp_type = sym[0]
        exp_data = sym[1:]
        second_arg = ", err" if type_map[exp_type][4] else ""
        return f"cmc_{type_map[exp_type][1]}_free({tofree}->{exp_data}{second_arg});"

    def handle_array(sym):
        name, array_exp, _ = split_array_exp(sym)
        i = chr(deepness)
        loop_body = free_method_content(array_exp, f"p_{name}", deepness + 1, packet_name)
        loop_statement = f"""
            for (size_t {i} = 0; {i} < {tofree}->{name}.size; ++{i}) {{
                {packet_name}_{name} *p_{name} = &(({packet_name}_{name} *){tofree}->{name}.data)[{i}];
                {loop_body}
            }}
        """ if loop_body else ""
        return f"""
            {loop_statement}
            free({tofree}->{name}.data);
            {tofree}->{name}.size = 0;
        """

    return "".join(
        handle_array(sym) if (sym[0] == "A") else handle_value(sym)
        for sym in careful_split(exp)
        if type_map[sym[0]][2] # is_heap(exp_type)
    )

def free_method(inp):
    if inp["is_empty"]:
        return ""

    return f"""
    void cmc_free_{inp['name']}_packet({inp['name']}_packet *packet, cmc_err_extra *err) {{
        {
            free_method_content(inp['type_def_content_str'], 'packet', ord('i'), inp['name'])
            if inp['is_heap'] else '(void)packet;'
        }
        (void)err;
    }}
    """

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

                    content = []
                    for val in out[packet_name]["packet_data"].values():
                        for field in val["content"]:
                            if (duplicates := [f for f in content if f[1:] == field[1:]]):
                                if (existing_field := duplicates[0]) != field:
                                    print(f"fatal conflict between {existing_field} and {field} on {packet_name}!!!")
                                    exit(1)
                            else:
                                content.append(field)
                                if type_map[field[0]][2]:
                                    out[packet_name]["is_heap"] = True
                                
                    
                    out[packet_name]["type_def_content"] = content
                    out[packet_name]["type_def_content_str"] = ";".join(content)

                out[packet_name]["packet_data"][vid]["packet_id"] = packet_id
                out[packet_name]["name"] = packet_name

    return list(out.values())

def packet_name_id_define(exps):
    return "".join(f"CMC_{exp['name'].upper()}_NAME_ID," for exp in exps)

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

    replace_code_segments("\n".join(f"HELPER(CMC_{inp['name'].upper()}_NAME_ID);" for inp in mc_packet_exps), "packet_name_id_string")


    replace_code_segments(packet_name_id_define(mc_packet_exps), "packet_name_id_define")

    # packet unpack methods
    replace_code_segments(
        "".join(unpack_method(mc_packet_exp) for mc_packet_exp in mc_packet_exps),
        "unpack_methods_c",
    )
    replace_code_segments(
        "".join(f"{inp['name']}_packet unpack_{inp['name']}_packet(cmc_buff *buff);" if not inp['is_empty'] else "" for inp in mc_packet_exps),
        "unpack_methods_h",
    )
    # packet unpack methods header

    replace_code_segments(
        "".join(send_method(mc_packet_exp) for mc_packet_exp in mc_packet_exps),
        "send_methods_c",
    )

    # packet send methods header
    replace_code_segments(
        "".join("cmc_err cmc_send_{}_packet(cmc_conn *conn{});".format(inp['name'], f", {inp['name']}_packet *packet" if not inp["is_empty"] else "") for inp in mc_packet_exps),
        "send_methods_h",
    )

    # type defs
    replace_code_segments(
        "".join(type_def(mc_packet_exp) for mc_packet_exp in mc_packet_exps),
        "packet_types",
    )

    replace_code_segments(
        "\n".join(free_method(sym) for sym in mc_packet_exps), "free_methods_c"
    )

    replace_code_segments(
        "\n".join(f"void cmc_free_{inp['name']}_packet({inp['name']}_packet *packet, cmc_err_extra *err);" for inp in mc_packet_exps if not inp["is_empty"]), "free_methods_h"
    )
    
if __name__ == "__main__":
    main()
