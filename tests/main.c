#include <cmc/packets.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <cmc/err.h>
#include <cmc/conn.h>
#include <string.h>

#define TRY_CATCH(expr, action) if(expr != CMC_ERR_NO) {action;}

int main() {
  cmc_conn conn = cmc_conn_init(47);

  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(12345);
  inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

  TRY_CATCH(cmc_conn_connect(&conn, (struct sockaddr*)&serv_addr, sizeof(serv_addr)), goto err;);
  
  C2S_handshake_handshake_packet handshake;
  handshake.next_state = CMC_CONN_STATE_STATUS;
  handshake.protocole_version = 47;
  handshake.server_addr = strdup("127.0.0.1");
  handshake.server_port = 25565;

  TRY_CATCH(cmc_send_C2S_handshake_handshake_packet(&conn, &handshake), goto err1;);

  cmc_free_C2S_handshake_handshake_packet(&handshake);
  TRY_CATCH(cmc_send_C2S_status_request_packet(&conn), goto err;);

  cmc_buff *packet = cmc_conn_recive_packet(&conn);

  

  return 0;
err1:
  cmc_free_C2S_handshake_handshake_packet(&handshake);
err:
  printf("cmc errd at %s:%d with error %s", conn.err.file, conn.err.line, cmc_err_as_str(conn.err.err));
  
  return 1;
}