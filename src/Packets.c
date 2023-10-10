// keep_alive,v,keep_alive_id

#include "MCbuffer.h"
#include "MConn.h"
#include <stdlib.h>

typedef struct {
  int keep_alive_id;
} keep_alive_packet;

void send_packet_keep_alive(MConn *conn, int keep_alive_id, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();

  MCbuffer_pack_varint(buff, keep_alive_id, errmsg); // v
  if (*errmsg != NULL) {
    free(buff);
    return;
  }

  MConn_send_buffer(conn, buff, errmsg);
}

keep_alive_packet unpack_keep_alive_packet(MCbuffer *buff, char **errmsg) {
  keep_alive_packet packet;

  packet.keep_alive_id = MCbuffer_unpack_int(buff, errmsg);
  return packet;
}
