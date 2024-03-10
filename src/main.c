#include "conn.h"
#include "err.h"
#include "heap_utils.h"
#include "packet_types.h"
#include "packets.h"
#include <stdio.h>

enum cmc_err cmc_err = ERR_NO;

int main() {
  cmc_conn conn = cmc_conn_init(765);
  cmc_conn_login(&conn);
  if (cmc_err != ERR_NO) {
    printf("%s\n", cmc_err_as_str(cmc_err));
    return 1;
  }
  while (conn.state != CMC_CONN_STATE_OFFLINE) {
    cmc_buffer *raw_packet = ERR_ABLE(cmc_conn_recive_packet(&conn), break;);
    int packet_id = cmc_buffer_unpack_varint(raw_packet);
    cmc_packet_name_id pnid = cmc_packet_id_to_packet_name_id(
        packet_id, conn.state, CMC_DIRECTION_S2C, 765);
    switch (pnid) {
    case CMC_S2C_PLAY_KEEP_ALIVE_NAME_ID: {
      puts("keepalive");
      S2C_play_keep_alive_packet keep_alive_sever =
          unpack_S2C_play_keep_alive_packet(raw_packet);
      C2S_play_keep_alive_packet keep_alive_client = {
          .keep_alive_id = keep_alive_sever.keep_alive_id};
      cmc_send_C2S_play_keep_alive_packet(&conn, &keep_alive_client);
      cmc_free_C2S_play_keep_alive_packet(&keep_alive_client);
      cmc_free_S2C_play_keep_alive_packet(&keep_alive_sever);
      break;
    }
    default: {
      printf("packet %s, packet_id %d\n", cmc_packet_name_id_string(pnid),
             packet_id);
    }
    }
    cmc_buffer_free(raw_packet);
    if (cmc_err) {
      conn.state = CMC_CONN_STATE_OFFLINE;
      printf("error occured %s\n", cmc_err_as_str(cmc_err));
    }
  }
  cmc_conn_close(&conn);
  if (cmc_err) {
    conn.state = CMC_CONN_STATE_OFFLINE;
    printf("error occured %s\n", cmc_err_as_str(cmc_err));
  }
  return 0;
}