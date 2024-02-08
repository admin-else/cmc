#include "err.h"
#include "heap_utils.h"
#include "mconn.h"
#include "mctypes.h"
#include "packet_types.h"
#include "packets.h"
#include <stdio.h>

struct cmc_err cmc_err =
    (struct cmc_err){.file = NULL, .line = 0, .type = ERR_NO};

void on_join_game(const S2C_play_join_game_packet_t data, struct MConn *conn) {
  printf("%s\n", data.level_type);
}

void on_keep_alive(const S2C_play_keep_alive_packet_t data,
                   struct MConn *conn) {
  send_packet_C2S_play_keep_alive(conn, data.keep_alive_id);
}

void on_chat_message(const S2C_play_chat_message_packet_t data,
                     struct MConn *conn) {
  printf("%s\n", data.message);
}

void on_kick(const S2C_play_disconnect_packet_t data, struct MConn *conn) {
  printf("kicked %s\n", data.reason);
  MConn_close(conn);
}

void unhandeld(MCbuffer *buff, int packet_id, struct MConn *conn) {
  const char *packet_name =
      packet_id_to_string(packet_id, CONN_STATE_PLAY, DIRECTION_S2C);
  printf("unhandeld packet %s\n", packet_name);
}
int main() {
#define ERR_ACTION return;
  struct MConn *conn = MConn_init();
  conn->on_packet.join_game = on_join_game;
  conn->on_packet.keep_alive = on_keep_alive;
  conn->on_packet.chat_message = on_chat_message;
  conn->on_packet.disconnect = on_kick;
  conn->on_unhandeld_packet = unhandeld;
  MConn_loop(conn);
  if (cmc_err.type != ERR_NO) {
    printf("%s at %s:%d\n", err_id2str(cmc_err.type), cmc_err.file,
           cmc_err.line);
  }
  MConn_free(conn);
  return 0;
}