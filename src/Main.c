#include "Crypto.h"
#include "FileUtils.h"
#include "MCbuffer.h"
#include "MConn.h"
#include "MCtypes.h"
#include "Nbt.h"
#include "Packets.h"
#include "heap-utils.h"
#include "textcolor.h"
#include <curses.h>
#include <jansson.h>
#include <openssl/rsa.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>

#define EXIT_IF_ERR(message)                                                   \
  if (errmsg != NULL) {                                                        \
    fprintf(stderr, message, errmsg);                                          \
    return 1;                                                                  \
  }

#define ERR_CHECK_NULL                                                         \
  if (*errmsg != NULL)                                                         \
    return NULL;

#define ERR_CHECK_VOID                                                         \
  if (*errmsg != NULL)                                                         \
    return;

json_t *get_server_status(char *ip, unsigned short port, char **errmsg) {
  MConn *conn = MConn_init(ip, port, errmsg);

  send_packet_C2S_handshake(conn, 47, ip, port, 1, errmsg);
  send_packet_C2S_status_request(conn, errmsg);
  MCbuffer *buff = MConn_recive_packet(conn, errmsg);

  int packet_id = MCbuffer_unpack_varint(buff, errmsg);
  printf("packet id: %i\n", packet_id);
  S2C_status_response_packet_t packet =
      unpack_S2C_status_response_packet(buff, errmsg);
  MCbuffer_free(buff);

  MConn_free(conn, errmsg);

  return packet.response;
}

void S2C_login_set_compression(MConn *conn, MCbuffer *buff, char **errmsg) {
  S2C_login_set_compression_packet_t packet =
      unpack_S2C_login_set_compression_packet(buff, errmsg);
  if (*errmsg != NULL)
    return;
  conn->compression_threshold = packet.threshold;
}

void S2C_login_encryption_request(MConn *conn, MCbuffer *buff, char **errmsg) {
  S2C_login_encryption_request_packet_t packet =
      unpack_S2C_login_encryption_request_packet(buff, errmsg);
  MCbuffer_free(buff);
  ERR_CHECK_VOID

  if (packet.verify_token->length != 4)
    *errmsg = "verify token is not len of 4 that is not good ):";

  ERR_CHECK_VOID

  RSA *rsa = pubkeyDER_to_RSA(packet.public_key->data,
                              packet.public_key->length, errmsg);
  ERR_CHECK_VOID

  MCbuffer *verify_token_enc = MCbuffer_init_w_size(RSA_size(rsa));
  RSA_public_encrypt(4, packet.verify_token->data, verify_token_enc->data, rsa,
                     RSA_PKCS1_PADDING);
  MCbuffer_free(packet.verify_token);

  conn->shared_secret = generate_random_bytes(16);
  MCbuffer *shared_secred_enc = MCbuffer_init_w_size(RSA_size(rsa));

  RSA_public_encrypt(16, conn->shared_secret, shared_secred_enc->data, rsa,
                     RSA_PKCS1_PADDING);

  RSA_free(rsa);
  MCbuffer_free(packet.public_key); // this will cause all exits after this to segfault
  send_packet_C2S_login_encryption_response(conn, shared_secred_enc,
                                            verify_token_enc, errmsg);
  MCbuffer_free(shared_secred_enc);
  MCbuffer_free(verify_token_enc);
}

void S2C_login_disconnect(MConn *conn, MCbuffer *buff, char **errmsg) {
  S2C_login_disconnect_packet_t packet =
      unpack_S2C_login_disconnect_packet(buff, errmsg);
  char *json_str = json_dumps(packet.reason, 0);
  printf("Server kicked you while loging in becouse %s\n", json_str);
  FREE(json_str);
  json_decref(packet.reason);
  MConn_close(conn, errmsg);
}

int main() {
  char *errmsg = NULL;
  MConn *conn = MConn_init("127.0.0.1", 25565, &errmsg);
  MConn_connect(conn, &errmsg);
  send_packet_C2S_handshake(conn, 47, "127.0.0.1", 25565, CONN_STATE_LOGIN,
                            &errmsg);
  send_packet_C2S_login_start(conn, "Admin_Else", &errmsg);
  conn->state = CONN_STATE_LOGIN;
  while (conn->state != CONN_STATE_OFFLINE) {
    MCbuffer *buff = MConn_recive_packet(conn, &errmsg);
    EXIT_IF_ERR("err while reciving packet: %s\n")
    int packet_id = MCbuffer_unpack_varint(buff, &errmsg);
    EXIT_IF_ERR("err while looking at packet id: %s\n")
    printf("packet_id %i\n", packet_id);
    switch (conn->state) {
    case CONN_STATE_LOGIN:
      switch (packet_id) {
      case PACKETID_S2C_LOGIN_SET_COMPRESSION:
        S2C_login_set_compression(conn, buff, &errmsg);
        break;
      case PACKETID_S2C_LOGIN_ENCRYPTION_REQUEST:
        S2C_login_encryption_request(conn, buff, &errmsg);
        break;
      case PACKETID_S2C_LOGIN_DISCONNECT:
        S2C_login_disconnect(conn, buff, &errmsg);
        break;
      }
    }
    printf("finished handeling packet id %i\n", packet_id);
    EXIT_IF_ERR("error in packet loop %s\n")
  }
}