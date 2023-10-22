//This code is generated.

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

#define PACK_ERR_HANDELER(pack_name)                                           \
  if (*errmsg != NULL) {                                                       \
    char error_message[256];                                                   \
    sprintf(error_message, "error while packing " #pack_name ": %s", *errmsg); \
    free(buff);                                                                \
    *errmsg = strdup(error_message);                                           \
    return;                                                                    \
  }

#define UNPACK_ERR_HANDELER(unpack_name)                                       \
  if (*errmsg != NULL) {                                                       \
    char error_message[256];                                                   \
    sprintf(error_message, "error while unpacking " #unpack_name ": %s",       \
            *errmsg);                                                          \
    *errmsg = strdup(error_message);                                           \
    return packet;                                                             \
  }

void send_packet_handshake(MConn *conn, varint_t protocole_version, char * server_addr, unsigned short port, varint_t next_state, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, 0x0, errmsg);
  MCbuffer_pack_varint(buff, protocole_version, errmsg);
  MCbuffer_pack_string(buff, server_addr, errmsg);
  MCbuffer_pack_ushort(buff, port, errmsg);
  MCbuffer_pack_varint(buff, next_state, errmsg);
  PACK_ERR_HANDELER(handshake);
  MConn_send_packet(conn, buff, errmsg);
}

handshake_packet_t unpack_handshake_packet(MCbuffer *buff, char **errmsg) {
  handshake_packet_t packet;
  packet.protocole_version=MCbuffer_unpack_varint(buff,errmsg);
  packet.server_addr=MCbuffer_unpack_string(buff,errmsg);
  packet.port=MCbuffer_unpack_ushort(buff,errmsg);
  packet.next_state=MCbuffer_unpack_varint(buff,errmsg);
  UNPACK_ERR_HANDELER(handshake);
  return packet;
}

void send_packet_status_request(MConn *conn, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, 0x0, errmsg);
  PACK_ERR_HANDELER(status_request);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_status_response(MConn *conn, json_t * response, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, 0x0, errmsg);
  MCbuffer_pack_json(buff, response, errmsg);
  PACK_ERR_HANDELER(status_response);
  MConn_send_packet(conn, buff, errmsg);
}

status_response_packet_t unpack_status_response_packet(MCbuffer *buff, char **errmsg) {
  status_response_packet_t packet;
  packet.response=MCbuffer_unpack_json(buff,errmsg);
  UNPACK_ERR_HANDELER(status_response);
  return packet;
}

