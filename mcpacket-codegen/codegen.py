C_BASE = """//This code is generated.

#include "Packets.h"
#include "MCbuffer.h"
#include "MConn.h"
#include "MCtypes.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

/*
If you ask yourself WHY THE FUCK is it that instead of this:
sfprintf(*errmsg, "error while unpacking/packing " #pack_name/unpack_name ":
%s", *errmsg); wellll compiler does this if i dont make it like this: argument 3
[-Wrestrict] 11 |     sprintf(*errmsg, "error while packing " #pack_name ": %s",
*errmsg);             |             ^~~~~~~ ~~~~~~~ src/Packets.c:26:3: note: in
expansion of macro 'PACK_ERR_HANDELER' 26 |   PACK_ERR_HANDELER(keep_alive)
MConn_send_and_free_buffer(conn, buff, errmsg); |   ^~~~~~~~~~~~~~~~~

and i dont like that so made it "safer" ig...

*/

#define PACK_ERR_HANDELER(pack_name)                                           \\
  if (*errmsg != NULL) {                                                       \\
    char error_message[256];                                                   \\
    sprintf(error_message, "error while packing " #pack_name ": %s", *errmsg); \\
    free(buff);                                                                \\
    *errmsg = strdup(error_message);                                           \\
    return;                                                                    \\
  }

#define UNPACK_ERR_HANDELER(unpack_name)                                       \\
  if (*errmsg != NULL) {                                                       \\
    char error_message[256];                                                   \\
    sprintf(error_message, "error while unpacking " #unpack_name ": %s",       \\
            *errmsg);                                                          \\
    *errmsg = strdup(error_message);                                           \\
    return packet;                                                             \\
  }

"""

H_BASE = """//This code is generated.

#ifndef PACKET_H
#define PACKET_H

#include "MCbuffer.h"
#include "MConn.h"
#include "MCtypes.h"
#include <stdbool.h>
#include <jansson.h>

{}#endif"""

# Define a dictionary to map type identifiers to their corresponding C types
type_map = {
    "b": "char",
    "B": "byte_t",
    "h": "short",
    "H": "unsigned short",
    "i": "int",
    "l": "long",
    "f": "float",
    "d": "double",
    "s": "char *",
    "v": "varint_t", 
    "?": "bool",
    "j": "json_t *"
}

buffer_methods_map = {
    "v": "varint",
    "?": "bool",
    "i": "int",
    "B": "byte",
    "b": "char",
    "s": "string",
    "l": "long",
    "H": "ushort",
    "j": "json"
}

def parse(input_str):
    packet_name, symbol_str = input_str.split(":")
    struct_name = packet_name + "_packet_t"
    symbols = symbol_str.split(";")
    packet_id = symbols[0]
    symbols = symbols[1:]

    type_def_content = "".join(
        [f"  {type_map[symbol[0]]} {symbol[1:]};\n" for symbol in symbols])
    args = ""
    if symbols:
      args = ", ".join([f"{type_map[symbol[0]]} {symbol[1:]}" for symbol in symbols]) + ", "
    pack_methods = f"  MCbuffer_pack_varint(buff, {packet_id}, errmsg);\n" + "".join([f"  MCbuffer_pack_{buffer_methods_map[symbol[0]]}(buff, {symbol[1:]}, errmsg);\n" for symbol in symbols])
    unpack_methods = "".join([f"  packet.{symbol[1:]}=MCbuffer_unpack_{buffer_methods_map[symbol[0]]}(buff,errmsg);\n" for symbol in symbols])
    send_method_define = f"void send_packet_{packet_name}(MConn *conn, {args}char **errmsg)"
    unpack_method_define = f"{struct_name} unpack_{packet_name}_packet(MCbuffer *buff, char **errmsg)"

    type_def = ""
    if symbols:
      type_def = f"typedef struct {{\n{type_def_content}}} {struct_name};\n\n"
    send_method = f"""{send_method_define} {{\n  MCbuffer *buff = MCbuffer_init();\n{pack_methods}  PACK_ERR_HANDELER({packet_name});\n  MConn_send_packet(conn, buff, errmsg);\n}}\n\n"""
    send_method_h = f"{send_method_define};\n\n"
    unpack_method = ""
    if symbols:
      unpack_method = f"{unpack_method_define} {{\n  {struct_name} packet;\n{unpack_methods}  UNPACK_ERR_HANDELER({packet_name});\n  return packet;\n}}\n\n"
    unpack_method_h = ""
    if symbols:
      unpack_method_h = f"{unpack_method_define};\n\n"

    return send_method + unpack_method, type_def + send_method_h + unpack_method_h

if __name__=="__main__":
    with open("packets.txt", "r") as f:
        mc_packet_exps = f.read().split("\n")

    c_code = ""
    h_code = ""

    for mc_packet_exp in mc_packet_exps:
        c, h = parse(mc_packet_exp)
        c_code += c
        h_code += h

    c_code = C_BASE    +   c_code
    h_code = H_BASE.format(h_code)

    with open("Packets.c", "w") as f:
        f.write(c_code)

    with open("Packets.h", "w") as f:
        f.write(h_code)
