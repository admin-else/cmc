import os
import re
from pprint import pprint
import glob

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
    "a": ["cmc_buff *",           "buff",            True,   "NULL",                    ],
    "S": ["cmc_slot *",           "slot",            True,   "NULL",                    ],
    "m": ["cmc_entity_metadata ", "entity_metadata", True,   "{.size=0,.entries=NULL}", ],
    "u": ["cmc_uuid ",             "uuid",           False,  "{.lower=0,.upper=0}",     ],
    "A": ["cmc_array ",            None,             True,   "{.data=NULL,.size=0}",    ],
}

def type_c(f):
    return type_map[f['type']][0]

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
    #replace_code_segments("".join([function(packet) for packet in packets]), tag)
    print("".join([function(packet) for packet in packets]))

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
                info["version"] = vid
                info["packet_id"] = packet_id
                info["is_empty"] = not bool(fields) # dont generate type then and also disregard all would be needed methods
                info["needs_free"] = False
                
                info["content"] = parse_exp_fields(fields)
                out.append(info)
                know_exp.append(exp)
    
    return out

"""
struct S2C_handshake_handshake {
    type 
};

"""
def type_def(i):
    code = ""
    if i["is_empty"]:
        return code
    
    def content(fields, struct_name):
        nonlocal code
        c = ""
        c += f"struct {struct_name} {{"
        for field in fields:
            if field["type"] == "A":
                content(field["content"], struct_name + "_" + field["name"])
            else:
                c += f"{type_c(field)}{field['name']};"
        c += "};"
        c += f"typedef struct {struct_name} {struct_name};"
        code += c
    
    content(i["content"], f"cmc_packet_{i['name']}_{i['version']}")
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
def main():
    packets = gather_packets()
    pprint(packets)
    
    #replace_with_functions("packet_types", type_def, packets)
    #replace_code_segments(packet_id_enums(packets), "packet_ids")
    
    
    
if __name__ == "__main__":
    main()
