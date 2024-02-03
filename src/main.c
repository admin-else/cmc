#include "err.h"
#include "mconn.h"
#include "packet_types.h"
#include <arpa/inet.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/socket.h>

struct cmc_err cmc_err =
    (struct cmc_err){.file = NULL, .line = 0, .type = ERR_NO};

void on_join_game(const S2C_play_join_game_packet_t data) {
  printf("%s\n", data.level_type);
}

int main() {
#define ERR_ACTION return;

  MConn *conn = MConn_init();
  conn->on_packet.join_game = on_join_game;
  MConn_loop(conn);
  if (cmc_err.type) {
    printf("%s at %s:%d \n", err_id2str(cmc_err.type), cmc_err.file, cmc_err.line);
  }
  return 0;
}