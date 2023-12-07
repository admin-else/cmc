import os
import re

type_map = {
#  TYPE  |code tpye             |method type       |is heap
    "b": ["char ",               "char",            False],
    "B": ["byte_t ",             "byte",            False],
    "h": ["short ",              "short",           False],
    "H": ["unsigned short ",     "ushort",          False],
    "i": ["int ",                "int",             False],
    "I": ["unsigned int ",       "uint",            False],
    "l": ["long ",               "long",            False],
    "L": ["unsigned long ",      "ulong",           False],
    "f": ["float ",              "float",           False],
    "d": ["double ",             "double",          False],
    "q": ["long long ",          "llong",           False],
    "Q": ["unsigned long long ", "ullong",          False],
    "?": ["bool ",               "bool",            False],
    "v": ["varint_t ",           "varint",          False],
    "s": ["char *",              "string",          True ],
    "p": ["block_pos_t ",        "position",        False],
    "n": ["nbt_node *",          "nbt",             True ],
    "j": ["json_t *",            "json",            True ],
    "a": ["MCbuffer *",          "byte_array",      True ],
    "S": ["slot_t *",            "slot",            True ],
    "m": ["entity_metadata_t ",  "entity_metadata", True ]
}

"""
ok this will generate the on_packet structs content that contains function pointers that will be called with packet info



"""
def replace_code_segments(replacement_code, tag):
  start_tag = f"// CGSS: {tag}" # Code Generator Segment Start
  end_tag = f"// CGSE: {tag}"   # Code Generator Segment End

  for file_name in os.listdir('./src'):
    if file_name.endswith(".c") or file_name.endswith(".h"): 
      file_path = os.path.join('./src', file_name)

      with open(file_path, 'r') as file:
        file_content = file.read()

      pattern = re.compile(f'({re.escape(start_tag)}).*?({re.escape(end_tag)})', re.DOTALL)
      if pattern.search(file_content):
        return
      file_content = pattern.sub(rf'\1\n{replacement_code}\n\2', file_content)

      with open(file_path, 'w') as file:
        file.write(file_content)

  raise ValueError(f"didnt find tag {tag}")  
  
def parse(input_str):
  packet_name, packet_id, symbol_str = input_str.split(";", maxsplit=2)
  struct_name = packet_name + "_packet_t"
  symbols = [sym for sym in symbol_str.split(";") if sym != ""]
  args = ""
  type_def = ""
  if symbols:
    args = ", ".join([f"{type_map[symbol[0]][0]}{symbol[1:]}" for symbol in symbols]) + ", "
    type_def_content = "".join(
      [f"  {type_map[symbol[0]][0]}{symbol[1:]};" for symbol in symbols])
    type_def = f"typedef struct {{{type_def_content}}} {struct_name};"
  pack_methods = f"  MCbuffer_pack_varint(buff, PACKETID_{packet_name.upper()}, errmsg);" + "".join([f"MCbuffer_pack_{type_map[symbol[0]][1]}(buff, {symbol[1:]}, errmsg);" for symbol in symbols])
  unpack_methods = "".join([f"packet.{symbol[1:]}=MCbuffer_unpack_{type_map[symbol[0]][1]}(buff,errmsg);" for symbol in symbols])
  send_method_define = f"void send_packet_{packet_name}(MConn *conn, {args}char **errmsg)"
  unpack_method_define = f"{struct_name} unpack_{packet_name}_packet(MCbuffer *buff, char **errmsg)"
  send_method = f"{send_method_define} {{MCbuffer *buff = MCbuffer_init();{pack_methods}  PACK_ERR_HANDELER({packet_name});MConn_send_packet(conn, buff, errmsg);}}"
  send_method_h = f"{send_method_define};"
  unpack_method = f"{unpack_method_define} {{{struct_name} packet;{unpack_methods} UNPACK_ERR_HANDELER({packet_name});return packet;}}" if symbols else ""
  unpack_method_h = f"{unpack_method_define};" if symbols else ""
  return send_method + unpack_method, type_def + send_method_h + unpack_method_h

def type_def(input_str):
  packet_name, packet_id, symbol_str = input_str.split(";", maxsplit=2)
  symbols = [sym for sym in symbol_str.split(";") if sym != ""]
  type_def = f"typedef struct {{{''.join([f'{type_map[symbol[0]][0]}{symbol[1:]};' for symbol in symbols])}}} {packet_name}_packet_t;" if symbols else ""
  return type_def

def send_method(input_str):
  packet_name, packet_id, symbol_str = input_str.split(";", maxsplit=2)
  symbols = [sym for sym in symbol_str.split(";") if sym != ""]
  pack_methods = f"  MCbuffer_pack_varint(buff, PACKETID_{packet_name.upper()}, errmsg);" + "".join([f"MCbuffer_pack_{type_map[symbol[0]][1]}(buff, {symbol[1:]}, errmsg);" for symbol in symbols])
  send_method_define = f"void send_packet_{packet_name}(MConn *conn, {', '.join([f'{type_map[symbol[0]][0]}{symbol[1:]}' for symbol in symbols]) + ', ' if symbols else ''}char **errmsg)"
  send_method = f"{send_method_define} {{MCbuffer *buff = MCbuffer_init();{pack_methods}  PACK_ERR_HANDELER({packet_name});MConn_send_packet(conn, buff, errmsg);}}\n\n"
  return send_method

def send_method_h(input_str):
  packet_name, packet_id, symbol_str = input_str.split(";", maxsplit=2)
  symbols = [sym for sym in symbol_str.split(";") if sym != ""]
  send_method_define = f"void send_packet_{packet_name}(MConn *conn, {', '.join([f'{type_map[symbol[0]][0]}{symbol[1:]}' for symbol in symbols]) + ', ' if symbols else ''}char **errmsg)"
  return send_method_define

def comment_filter(raw):
  out = ""
  comment = False
  for token in raw:
    if token == "#":
      comment = True

    if comment:
      if token == "\n":
        out += token
        comment = False
      continue

    out += token
  return out

def main():
    with open("packets.txt", "r") as f:
        raw = f.read()
    
    raw = comment_filter(raw)

    raw = raw.replace(" ", "").split("\n")
    tmp = []
    for l in raw:
      if not l == "":
        tmp.append(l)
    mc_packet_exps = tmp
    
    c_code = ""
    h_code = ""

    for mc_packet_exp in mc_packet_exps:
        c, h = parse(mc_packet_exp)
        c_code += c
        h_code += h

    
    
    # packet id to string
    replace_code_segments("".join([
      f"PACKET_DATA_TO_STRING_UTIL({mc_packet_exp.split(';')[1]}, CONN_STATE_{mc_packet_exp.split(';')[0].split('_')[1].upper()}, DIRECTION_{mc_packet_exp.split(';')[0].split('_')[0].upper()}, \"{mc_packet_exp.split(';')[0].upper()}\");"for mc_packet_exp in mc_packet_exps]), "packet_data_to_string")
    
    # packet ids
    replace_code_segments("\n".join([f"#define PACKETID_{mc_packet_exp.split(';')[0].upper()} {mc_packet_exp.split(';')[1]}\n" for mc_packet_exp in mc_packet_exps]), "packet_ids")
    
    os.system("clang-format -i ./src/*.c ./src/*.h")
    # The Zen of Python, by Tim Peters
    # 
    # Beautiful is better than ugly.
    # ...

    # i mean there is no zen of c ... would be funny tho

if __name__=='__main__':
  print(send_method("C2S_login_start;0x00;ltime"))
  
  #main()