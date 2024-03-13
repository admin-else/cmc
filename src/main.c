#include "conn.h"
#include "err.h"
#include "heap_utils.h"
#include "packet_types.h"
#include "packets.h"
#include <stdio.h>

enum cmc_err cmc_err = ERR_NO;

int main() {
  cmc_conn conn = cmc_conn_init(765);
  
  conn.sockfd = socket(AF_INET, SOCK_STREAM, 0);
  ERR_IF_NOT(conn.sockfd, ERR_SOCKET, return 1;);

  ERR_IF_LESS_THAN_ZERO(
      connect(conn.sockfd, (struct sockaddr *)&conn.addr, sizeof(conn.addr)),
      ERR_CONNETING, return 1;);

  C2S_handshake_handshake_packet handshake =
      (C2S_handshake_handshake_packet){.next_state = CMC_CONN_STATE_LOGIN,
                                       .server_port = 25565,
                                       .server_addr = "127.0.0.1",
                                       .protocole_version = conn.protocol_version};
  C2S_login_start_packet login_start = {.name = "Botzo",
                                        .uuid = {.lower = 0, .upper = 0}};
  
  conn.state = CMC_CONN_STATE_HANDSHAKE;
  
  ERR_ABLE(cmc_send_C2S_handshake_handshake_packet(&conn, &handshake), goto err_close_conn;);
  ERR_ABLE(cmc_send_C2S_login_start_packet(&conn, &login_start), goto err_close_conn;);
  
  conn.state = CMC_CONN_STATE_LOGIN;
  
  while (conn.state != CMC_CONN_STATE_OFFLINE) {
    cmc_buffer *raw_packet = ERR_ABLE(cmc_conn_recive_packet(&conn), goto err_close_conn;);
    int packet_id = cmc_buffer_unpack_varint(raw_packet);
    cmc_packet_id_to_packet_name_id(packet_id, conn.state, CMC_DIRECTION_S2C, conn.protocol_version);
    cmc_buffer_free(raw_packet);
  }
err_close_conn:
  if (conn.state != CMC_CONN_STATE_OFFLINE) {
    cmc_conn_close(&conn);
  }
  return 0;
}