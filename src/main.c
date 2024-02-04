#include "err.h"
#include "mconn.h"
#include "packet_types.h"
#include "packets.h"
#include <arpa/inet.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/socket.h>

struct cmc_err cmc_err =
    (struct cmc_err){.file = NULL, .line = 0, .type = ERR_NO};

void on_join_game(const S2C_play_join_game_packet_t data, struct MConn *conn) {
  printf("%s\n", data.level_type);
}

void on_keep_alive(const S2C_play_keep_alive_packet_t data,
                   struct MConn *conn) {
  send_packet_C2S_play_keep_alive(conn, data.keep_alive_id);
}

int main() {
#define ERR_ACTION return;

  struct MConn *conn = MConn_init();
  conn->on_packet.join_game = on_join_game;
  conn->on_packet.keep_alive = on_keep_alive;
  MConn_loop(conn);
  if (cmc_err.type != ERR_NO) {
    printf("%s at %s:%d\n", err_id2str(cmc_err.type), cmc_err.file,
           cmc_err.line);
  }
  return 0;
}