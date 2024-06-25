import os
import re
import glob

type_map = {
#  TYPE  |code type               |method type      |is heap |default value
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
    "a": ["cmc_buff *",           "buff",            True,   "NULL",                    ],
    "S": ["cmc_slot *",           "slot",            True,   "NULL",                    ],
    "m": ["cmc_entity_metadata ", "entity_metadata", True,   "{.size=0,.entries=NULL}", ],
    "u": ["cmc_uuid ",             "uuid",           False,  "{.lower=0,.upper=0}",     ],
    "A": ["cmc_array ",            None,             True,   "{.data=NULL,.size=0}",    ],
}

def type_c(f): return type_map[f['type']][0]
def type_func_name(f): return type_map[f['type']][1]
def is_heap(f): return type_map[f['type']][2]

types_with_extra_data = "A"

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
    if curr_str:
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

def replace_with_functions(tag, function, packets):
    replace_code_segments("".join([function(packet) for packet in packets]), tag)
    #print("".join([function(packet) for packet in packets]))

def replace_c_functions(tag, function, packets):
    headers = ""
    code = ""
    for packet in packets:
        generated_code = function(packet)
        if not generated_code[0]:
            continue
        
        headers += f"{generated_code[1]};"
        code += f"{generated_code[1]} {{{generated_code[0]}}}"

    #print(code)

    replace_code_segments(code, tag + "_c")
    replace_code_segments(headers, tag + "_h")



def parse_exp_fields(fields):
    out = []
    for field_str in careful_split(fields):
        field = {}
        field["type"] = field_str[0]
        if field["type"] in types_with_extra_data:
            field["name"], array_expression, field["size_key"] = split_array_exp(field_str)
            field["content"] = parse_exp_fields(array_expression)
        else:
            field["name"] = field_str[1:]
        out.append(field)
    return out

def gather_packets():
    out = []
    know_exp = []
    for fname in os.listdir("packets"):
        with open("packets/" + fname, "r") as f:
            vid = int(fname.split(".")[0])
            for exp in f.read().replace(" ", "").split("\n"):
                if exp.find("#") != -1:
                    exp = exp[:exp.find("#")]
                    
                if not exp:
                    continue
                
                if exp in know_exp:
                    continue
                
                info = {}
                name, packet_id, fields = exp.split(";", maxsplit=2)
                packet_id = int(packet_id, base=16)
                info["direction"], info["state"], *_ = name.split("_")
                info["name"] = name
                info["name_and_version"] = f"{name}_{vid}"
                info["type"] = f"cmc_packet_{name}_{vid}"
                info["version"] = vid
                info["packet_id"] = packet_id
                info["is_empty"] = not bool(fields) # dont generate type then and also disregard all would be needed methods
                
                info["content"] = parse_exp_fields(fields)
                info["needs_free"] = any([is_heap(f) for f in info["content"]]) # ohhh the python magic 
                out.append(info)                                                # i dont know if its ugly
                know_exp.append(exp)                                            # or modern art
    
    return out

"""
struct S2C_handshake_handshake {
    type 
};

"""
def type_def(packet):
    code = ""
    if packet["is_empty"]:
        return code
    
    def content(fields, struct_name):
        nonlocal code
        code_block = ""
        code_block += f"struct {struct_name} {{"
        for field in fields:
            if field["type"] == "A":
                content(field["content"], struct_name + "_" + field["name"])
                code_block += f"{struct_name + "_" + field["name"]} *{field["name"]};"
            else:
                code_block += f"{type_c(field)}{field['name']};"
        code_block += "};"
        code_block += f"typedef struct {struct_name} {struct_name};"
        code += code_block
    
    content(packet["content"], f"cmc_packet_{packet['name']}_{packet['version']}")
    return code
    
def packet_id_enums(packets):
    states = {}
    for packet in packets:
        concated_state = f"{packet["direction"]}_{packet["state"]}_{packet["version"]}"
        if concated_state not in states:
            states[concated_state] = []
        states[concated_state].append(packet)

    code = ""
    for state in states:
        state_name = "cmc_packet_ids_state_" + state
        code += f"enum {state_name} {{"
        for packet in states[state]:
            code += f"cmc_packet_id_{packet['name_and_version']} = {packet["packet_id"]},"
        code += f"}}; typedef enum {state_name} {state_name};"
        
    return code

def unpack_packet(packet):
    iterator_decls = ""
    
    err_handling = [] # we populate this whenever a thing is created that needs to be cleaned up 
                      # and we append in front so it all runs reversed 
    on_err = ""
    remove_last_err_handler = False
    def add_failiure_point(lable, code):
        nonlocal err_handling, on_err, remove_last_err_handler
        err_handling.insert(0, f"{f"{lable}:" if lable else ""} {code}")

        if lable:        
            on_err = f"goto {lable};"
            remove_last_err_handler = True
    
    code = ""
    header = ""
    if packet["is_empty"]:
        return (code, header)
    
    header = f"{packet["type"]} {packet["type"]}_unpack(cmc_buff *buff)"
    code += f"{packet["type"]} packet = {{}};"
    
    add_failiure_point("err", f"return ({packet["type"]}){{}};")
    
    iterator_char = 'h' # before i 
    
    def unpack_code(unpack_to, content, lable_prefixs):
        nonlocal code, iterator_char, iterator_decls, err_handling, remove_last_err_handler, on_err
        for field in content:
            if field["type"] == "A":
                iterator_char = chr(ord(iterator_char) + 1)
                code += f"if({unpack_to}{field["size_key"]} > 0) {{"
                code += f"{unpack_to}{field["name"]} = CMC_ERRB_ABLE(cmc_malloc_packet_array(buff, {unpack_to}{field["size_key"]} * sizeof(*{unpack_to}{field["name"]})), {on_err});"
                add_failiure_point(None, "}")
                add_failiure_point(lable_prefixs + "_" + field["name"], f"free({unpack_to}{field["name"]});")
                add_failiure_point(None, "}")
                code += f"for({iterator_char} = 0; {iterator_char} < {unpack_to}{field["size_key"]}; ++{iterator_char}) {{"
                
                on_err = f"goto {lable_prefixs + "_" + field["name"] + "_forloop"}"
                iterator_decls += f"int {iterator_char} = 0;"
                unpack_code(f"{unpack_to}{field["name"]}[{iterator_char}].", field["content"], lable_prefixs + "_" + field["name"])
                add_failiure_point(None, f"{lable_prefixs}_{field["name"]}_forloop:for(;{iterator_char}>0;--{iterator_char}) {{")
                iterator_char = chr(ord(iterator_char) - 1)
                code += "}"
                
                add_failiure_point(None, f"if({unpack_to}{field["size_key"]} > 0) {{")
                code += "}"
            else:
                code += f"{unpack_to}{field["name"]} = CMC_ERRB_ABLE(cmc_buff_unpack_{type_func_name(field)}(buff), {on_err});"
                remove_last_err_handler = False
                if is_heap(field):
                    add_failiure_point(lable_prefixs + "_" + field["name"],
                                       f"cmc_{type_func_name(field)}_free({unpack_to}{field["name"]});")
    
    unpack_code("packet.", packet["content"], "err")
    code += "return packet;"
    if remove_last_err_handler:
        err_handling.pop(0)
    
    code = iterator_decls + code + "".join(err_handling)
    return (code, header)

def free_packet(packet):
    # i love it no labels not error handling
    def free_content(content, to_free):
        code = ""
        for field in content:
            if is_heap(field):
                if field["type"] == "A":
                    maybe_code = free_content(field["content"], f"{to_free}{field["name"]}[{to_free}{field["size_key"]}].")
                    code += f"if({to_free}{field["size_key"]} > 0) {{"
                    if maybe_code:
                        code += f"for(;{to_free}{field["size_key"]}>0;--{to_free}{field["size_key"]}) {{"
                        code += maybe_code
                        code += "}"
                    code += f"free({to_free}{field["name"]});"
                    code += "}"
                else:
                    code += f"cmc_{type_func_name(field)}_free({to_free}{field["name"]});"

        return code

    return (free_content(packet["content"], "packet->"), f"void {packet["type"]}_free({packet["type"]} *packet)")

def pack_packet(packet):
    def pack_content(content, to_pack, iterator_char):
        code = ""
        for field in content:
            if field["type"] == "A":
                code += f"for(int {iterator_char} = 0; {iterator_char}<{to_pack}{field["size_key"]}; ++{iterator_char}) {{"
                code += pack_content(field["content"], f"{to_pack}{field["name"]}[{iterator_char}].", chr(ord(iterator_char) + 1))
                code += "}"
            else:
                code += f"cmc_buff_pack_{type_func_name(field)}(buff, {to_pack}{field["name"]});"

        return code

    return (pack_content(packet["content"], "packet->", "i"), f"void {packet["type"]}_pack(cmc_buff *buff, {packet["type"]} *packet)")
    

def main():
    packets = gather_packets()
    
    replace_with_functions("packet_types", type_def, packets)
    replace_code_segments(packet_id_enums(packets), "packet_ids")
    replace_c_functions("unpack_methods", unpack_packet, packets)
    replace_c_functions("free_methods", free_packet, packets)
    replace_c_functions("pack_methods", pack_packet, packets)
    
if __name__ == "__main__":
    main()
