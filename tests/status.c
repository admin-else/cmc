#include <arpa/inet.h>
#include <cmc/conn.h>
#include <cmc/err.h>
#include <cmc/packets.h>
#include <stdio.h>
#include <string.h>

#define TRY_CATCH(expr, action)                                                \
  if (expr != CMC_ERR_NO) {                                                    \
    action;                                                                    \
  }

int main() {
  cmc_conn conn = cmc_conn_init(47);

  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(25565);
  inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

  TRY_CATCH(
      cmc_conn_connect(&conn, (struct sockaddr *)&serv_addr, sizeof(serv_addr)),
      goto err;);

  C2S_handshake_handshake_packet handshake;
  handshake.next_state = CMC_CONN_STATE_STATUS;
  handshake.protocole_version = 47;
  handshake.server_addr = strdup("127.0.0.1");
  handshake.server_port = 25565;

  TRY_CATCH(cmc_send_C2S_handshake_handshake_packet(&conn, &handshake),
            goto err2;);

  cmc_free_C2S_handshake_handshake_packet(&handshake);
  TRY_CATCH(cmc_send_C2S_status_request_packet(&conn), goto err1;);

  cmc_buff *packet = cmc_conn_recive_packet(&conn);
  if (!packet)
    goto err;
  
  int32_t packet_id = cmc_buff_unpack_varint(packet);
  TRY_CATCH(packet->err.err, goto err3;);

  cmc_packet_name_id pnid = cmc_packet_id_to_packet_name_id(packet_id, conn.state, CMC_DIRECTION_S2C, 47);
  
  if(pnid != CMC_S2C_STATUS_RESPONSE_NAME_ID) {
    printf("unexpected packet %s\n", cmc_packet_name_id_string(pnid));
    cmc_buff_free(packet);
    cmc_conn_close(&conn);
    return 1;
  }

  S2C_status_response_packet response = unpack_S2C_status_response_packet(packet);
  TRY_CATCH(packet->err.err, goto err3;);
  printf("status %s", response.response);
  cmc_free_S2C_status_response_packet(&response);
  cmc_buff_free(packet);
  cmc_conn_close(&conn);

  return 0;
err3:
  cmc_buff_free(packet);
  goto err1;
err2:
  cmc_free_C2S_handshake_handshake_packet(&handshake);
err1:
  cmc_conn_close(&conn);
err:
  printf("cmc errd at %s:%d with error %s", conn.err.file, conn.err.line,
         cmc_err_as_str(conn.err.err));

  return 1;
}