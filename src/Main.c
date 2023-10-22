#include "FileUtils.h"
#include "MCbuffer.h"
#include "MConn.h"
#include "MCtypes.h"
#include "Nbt.h"
#include "Packets.h"
#include "heap-utils.h"
#include "textcolor.h"
#include <curses.h>
#include <jansson.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_IF_ERR(message)                                                   \
  if (errmsg != NULL) {                                                        \
    fprintf(stderr, message, errmsg);                                          \
    exit(1);                                                                   \
  }

#define CHECK_ERRMSG if(*errmsg != NULL) return NULL;

json_t *get_server_status(char *ip, unsigned short port, char **errmsg) {
  MConn *conn = MConn_init(ip, port, errmsg);

  send_packet_handshake(conn, 47, ip, port, 1, errmsg);
  send_packet_status_request(conn, errmsg);
  MCbuffer *buff = MConn_recive_packet(conn, errmsg);

  int packet_id = MCbuffer_unpack_varint(buff, errmsg);
  printf("packet id: %i\n", packet_id);
  status_response_packet_t packet =
      unpack_status_response_packet(buff, errmsg);
  MCbuffer_free(buff);

  MConn_free(conn, errmsg);

  return packet.response;
}

int main() {
  char *errmsg = NULL;
  json_t *server_status = get_server_status("127.0.0.1", 25565, &errmsg);
  EXIT_IF_ERR("getting server status failed %s\n")

  char *str = json_dumps(server_status, 0);

  printf("status: %s\n", str);

  FREE(str);

  json_decref(server_status);
}