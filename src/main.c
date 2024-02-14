#include "conn.h"
#include "err.h"
#include "heap_utils.h"
#include "packet_types.h"
#include "packets.h"
#include <stdio.h>

struct cmc_err cmc_err =
    (struct cmc_err){.file = NULL, .line = 0, .type = ERR_NO};

static void on_data(cmc_buffer *buff, int packet_id, cmc_conn *conn) {
  (void)buff;
  (void)conn;
  const char *packet_name =
      packet_id_to_string(packet_id, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C);
  switch (packet_id) {
  case CMC_PACKETID_C2S_PLAY_KEEP_ALIVE: {
    S2C_play_keep_alive_packet keep_alive =
        unpack_S2C_play_keep_alive_packet(buff);
    send_packet_C2S_play_keep_alive(conn, keep_alive.keep_alive_id);
    break;
  }
  default:
    printf("unhandeld packet %X %s\n", packet_id, packet_name);
    break;
  }
}

int main() {
  cmc_conn *conn = cmc_conn_init();
  conn->on_packet = on_data;
  cmc_conn_loop(conn);
  if (cmc_err.type != ERR_NO) {
    printf("%s at %s:%d\n", err_id2str(cmc_err.type), cmc_err.file,
           cmc_err.line);
  }
  cmc_conn_free(conn);
  return 0;
}