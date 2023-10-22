#include "FileUtils.h"
#include "MCbuffer.h"
#include "MConn.h"
#include "MCtypes.h"
#include "Nbt.h"
#include "Packets.h"
#include "heap-utils.h"
#include "textcolor.h"
#include <curses.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_IF_ERR(message)                                                   \
  if (errmsg != NULL) {                                                        \
    fprintf(stderr, message, errmsg);                                          \
    exit(1);                                                                   \
  }



int main() {
  char *errmsg = NULL;

  MConn *conn = MConn_init("127.0.0.1", 25565, &errmsg);

  send_packet_handshake(conn, 47, "127.0.0.1", 25565, 1, &errmsg);
  send_packet_status_request(conn, &errmsg);
  MCbuffer *buff = MConn_recive_packet(conn, &errmsg);

  MCbuffer_unpack_varint(buff, &errmsg);

  EXIT_IF_ERR("i sold my second son on ebay becouse: %s\n")

  if (errmsg != NULL) {
    printf("Error msg %s\n", errmsg);
    return 1;
  }
  return 0;
}