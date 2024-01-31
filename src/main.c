#include "mconn.h"
#include "packet_types.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stddef.h>
#include <sys/socket.h>

void on_chat_message(const S2C_play_chat_message_packet_t packet) {}

int main() {
  char *errmsg = NULL;
  MConn *conn = MConn_init();
  conn->on_packet.chat_message = on_chat_message;
  MConn_loop(conn, &errmsg);

  return 0;
}