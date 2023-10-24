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

#define CHECK_ERRMSG                                                           \
  if (*errmsg != NULL)                                                         \
    return NULL;

json_t *get_server_status(char *ip, unsigned short port, char **errmsg) {
  MConn *conn = MConn_init(ip, port, errmsg);

  send_packet_C2S_handshake(conn, 47, ip, port, 1, errmsg);
  send_packet_C2S_status_request(conn, errmsg);
  MCbuffer *buff = MConn_recive_packet(conn, errmsg);

  int packet_id = MCbuffer_unpack_varint(buff, errmsg);
  printf("packet id: %i\n", packet_id);
  S2C_status_response_packet_t packet =
      unpack_S2C_status_response_packet(buff, errmsg);
  MCbuffer_free(buff);

  MConn_free(conn, errmsg);

  return packet.response;
}

void S2C_login_set_compression(MConn *conn, MCbuffer *buff, char **errmsg) {
  S2C_login_set_compression_packet_t packet = unpack_S2C_login_set_compression_packet(buff, errmsg);
  if(*errmsg != NULL) return;
  conn->compression_threshold = packet.threshold;
}

int main() {
  char *errmsg = NULL;
  MConn *conn = MConn_init("127.0.0.1", 25565, &errmsg);
  send_packet_C2S_handshake(conn, 47, "127.0.0.1", 25565, CONN_STATE_LOGIN, &errmsg);
  send_packet_C2S_login_start(conn, "Admin_Else", &errmsg);
  conn->state = CONN_STATE_LOGIN;
  while (1) {
    MCbuffer *buff = MConn_recive_packet(conn, &errmsg);
    int packet_id = MCbuffer_unpack_varint(buff, &errmsg);
    switch (conn->state) {
    case CONN_STATE_LOGIN:
      switch (packet_id) {
        case PACKETID_S2C_LOGIN_SET_COMPRESSION:
          S2C_login_set_compression(conn, buff, &errmsg);
          break;
      }
    }
  }
}