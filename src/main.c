#include "conn.h"
#include "err.h"
#include "heap_utils.h"
#include "packet_types.h"
#include "packets.h"
#include <stdio.h>
#include <stdlib.h>

enum cmc_err cmc_err = ERR_NO;

int main() {
  (void)malloc(100000);
  cmc_conn conn = cmc_conn_init(765);

  conn.sockfd = socket(AF_INET, SOCK_STREAM, 0);
  ERR_IF(conn.sockfd < 0, ERR_SOCKET, goto err_conn_fail;);

  ERR_IF_LESS_THAN_ZERO(
      connect(conn.sockfd, (struct sockaddr *)&conn.addr, sizeof(conn.addr)),
      ERR_CONNETING, goto err_conn_fail;);
  
  C2S_handshake_handshake_packet handshake = (C2S_handshake_handshake_packet){
      .next_state = CMC_CONN_STATE_LOGIN,
      .server_port = 25565,
      .server_addr = "127.0.0.1",
      .protocole_version = conn.protocol_version};
  C2S_login_start_packet login_start = {.name = "Botzo",
                                        .uuid = {.lower = 0, .upper = 0}};

  conn.state = CMC_CONN_STATE_HANDSHAKE;

  ERR_ABLE(cmc_send_C2S_handshake_handshake_packet(&conn, &handshake),
           goto err_close_conn;);
  ERR_ABLE(cmc_send_C2S_login_start_packet(&conn, &login_start),
           goto err_close_conn;);

  conn.state = CMC_CONN_STATE_LOGIN;

  while (conn.state != CMC_CONN_STATE_OFFLINE) {
    cmc_buffer *raw_packet =
        ERR_ABLE(cmc_conn_recive_packet(&conn), goto err_close_conn;);
    int packet_id = cmc_buffer_unpack_varint(raw_packet);
    cmc_packet_name_id pnid = cmc_packet_id_to_packet_name_id(
        packet_id, conn.state, CMC_DIRECTION_S2C, conn.protocol_version);
    printf("pnid %s packetid 0x%02X size %zu:\n",
           cmc_packet_name_id_string(pnid), packet_id, raw_packet->length);
    // cmc_buffer_print_info(raw_packet);
    switch (pnid) {
    case CMC_S2C_LOGIN_SET_COMPRESSION_NAME_ID: {
      S2C_login_set_compression_packet packet =
          unpack_S2C_login_set_compression_packet(raw_packet);
      conn.compression_threshold = packet.threshold;
      cmc_free_S2C_login_set_compression_packet(&packet);
      printf("set compression threshold to %zd\n", conn.compression_threshold);
      break;
    }
    case CMC_S2C_LOGIN_DISCONNECT_NAME_ID: {
      S2C_login_disconnect_packet packet =
          unpack_S2C_login_disconnect_packet(raw_packet);
      printf("kicked while logging in with message: %s\n", packet.reason);
      cmc_conn_close(&conn);
      cmc_free_S2C_login_disconnect_packet(&packet);
      break;
    }
    case CMC_S2C_LOGIN_SUCCESS_NAME_ID: {
      S2C_login_success_packet packet =
          unpack_S2C_login_success_packet(raw_packet);
      printf("login succses with name \"%s\".\n", packet.name);
      cmc_free_S2C_login_success_packet(&packet);
      cmc_send_C2S_login_acknowledged_packet(&conn);
      conn.state = CMC_CONN_STATE_CONFIG;
      break;
    }
    case CMC_S2C_CONFIG_FINISH_NAME_ID: {
      conn.state = CMC_CONN_STATE_PLAY;
      break;
    }
    case CMC_S2C_PLAY_KEEP_ALIVE_NAME_ID: {
      S2C_play_keep_alive_packet packet =
          unpack_S2C_play_keep_alive_packet(raw_packet);
      printf("pog: %ld\n", packet.keep_alive_id_long);
      cmc_buffer_print_info(raw_packet);
      C2S_play_keep_alive_packet pong = {.keep_alive_id_long =
                                             packet.keep_alive_id_long,
                                         .keep_alive_id = packet.keep_alive_id};
      cmc_send_C2S_play_keep_alive_packet(&conn, &pong);
      cmc_free_S2C_play_keep_alive_packet(&packet);
      break;
    }
    case CMC_S2C_PLAY_DISCONNECT_NAME_ID: {
      S2C_play_disconnect_packet packet =
          unpack_S2C_play_disconnect_packet(raw_packet);
      cmc_free_S2C_play_disconnect_packet(&packet);
      cmc_conn_close(&conn);
      break;
    }
    default: {
      // printf("pnid %s packetid %d\n", cmc_packet_name_id_string(pnid),
      //        packet_id);
    }
    }
    cmc_buffer_free(raw_packet);
  }
err_close_conn:
  if (conn.state != CMC_CONN_STATE_OFFLINE) {
    cmc_conn_close(&conn);
  }
err_conn_fail:
  if (cmc_err) {
    printf("error: %s\n", cmc_err_as_str(cmc_err));
    perror("errorno");
    return 1;
  }
  return 0;
}