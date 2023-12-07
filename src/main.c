#include "crypto.h"
#include "file_utils.h"
#include "mcbuffer.h"
#include "mconn.h"
#include "mctypes.h"
#include "nbt.h"
#include "packets.h"
#include "heap_utils.h"
#include "text_color.h"
#include <curses.h>
#include <jansson.h>
#include <openssl/rsa.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

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

  conn->shared_secret = generate_random_bytes(16);
  ERR_CHECK_VOID
  RSA *rsa = pubkeyDER_to_RSA(packet.public_key->data,
                              packet.public_key->length, errmsg);
  ERR_CHECK_VOID

  MCbuffer *verify_token_enc = MCbuffer_init_w_size(RSA_size(rsa));
  RSA_public_encrypt(4, packet.verify_token->data, verify_token_enc->data, rsa,
                     RSA_PKCS1_PADDING);
  MCbuffer_free(packet.verify_token);

  MCbuffer *shared_secred_enc = MCbuffer_init_w_size(RSA_size(rsa));

  RSA_public_encrypt(16, conn->shared_secret, shared_secred_enc->data, rsa,
                     RSA_PKCS1_PADDING);

  RSA_free(rsa);
  MCbuffer_free(
      packet.public_key); // this will cause all exits after this to segfault
  send_packet_C2S_login_encryption_response(conn, shared_secred_enc,
                                            verify_token_enc, errmsg);
  MCbuffer_free(shared_secred_enc);
  MCbuffer_free(verify_token_enc);
}

void S2C_login_disconnect(MConn *conn, MCbuffer *buff, char **errmsg) {
  char *json_str = MCbuffer_unpack_string(buff, errmsg);
  printf("Server kicked you while loging in becouse %s\n", json_str);
  FREE(json_str);
  MConn_close(conn, errmsg);
}

void on_S2C_login_success(MConn *conn, const S2C_login_success_packet_t packet,
                          char **errmsg) {}

void S2C_login_success(MConn *conn, MCbuffer *buff, char **errmsg) {
  S2C_login_success_packet_t packet =
      unpack_S2C_login_success_packet(buff, errmsg);
  conn->state = CONN_STATE_PLAY;
}

void S2C_play_keep_alive(MConn *conn, MCbuffer *buff, char **errmsg) {
  S2C_play_keep_alive_packet_t packet =
      unpack_S2C_play_keep_alive_packet(buff, errmsg);
  send_packet_C2S_play_keep_alive(conn, packet.keep_alive_id, errmsg);
}

void S2C_play_chat_message(MConn *conn, MCbuffer *buff, char **errmsg) {
  S2C_play_chat_message_packet_t packet =
      unpack_S2C_play_chat_message_packet(buff, errmsg);
  ERR_CHECK_VOID
  char *bruh = json_dumps(packet.message, 0);
  puts(bruh);
  FREE(bruh);
  json_decref(packet.message);
}

void S2C_play_disconnect(MConn *conn, MCbuffer *buff, char **errmsg) {
  S2C_play_disconnect_packet_t packet =
      unpack_S2C_play_disconnect_packet(buff, errmsg);
  ERR_CHECK_VOID
  printf("Kicked becouse:" TEXT_COLOR_RED TEXT_BOLD " %s" NLR, packet.reason);
  FREE(packet.reason);
  MConn_close(conn, errmsg);
}

void S2C_play_entity_status(MConn *conn, MCbuffer *buff, char **errmsg) {
  S2C_play_entity_status_packet_t packet =
      unpack_S2C_play_entity_status_packet(buff, errmsg);
  ERR_CHECK_VOID
  // printf("entity status id: %i %d\n", packet.entity_id,
  // packet.entity_status);
}

void S2C_play_join_game(MConn *conn, MCbuffer *buff, char **errmsg) {
  MCbuffer_print_info(buff);
  S2C_play_join_game_packet_t packet =
      unpack_S2C_play_join_game_packet(buff, errmsg);
  ERR_CHECK_VOID
  conn->player.eid = packet.entity_id;
  FREE(packet.level_type);
}

void S2C_play_entity_metadata(MConn *conn, MCbuffer *buff, char **errmsg) {
  ERR_CHECK_VOID
  MCbuffer_print_info(buff);
  S2C_play_entity_metadata_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff, errmsg);
  packet.meta_data = MCbuffer_unpack_entity_metadata(buff, errmsg);
  ERR_CHECK_VOID

  printf("meta data: %i %i\n", packet.entity_id, conn->player.eid);
  if (packet.entity_id != conn->player.eid)
    goto deffer;

  entity_metadata_entry_t *current_element;

  for (int i = 0; i < packet.meta_data.size; i++) {
    current_element =
        packet.meta_data.entries +
        i; // No need to multiply by sizeof(entity_metadata_entry_t)

    if (current_element->index == 6 &&
        current_element->type ==
            ENTITY_METADATA_ENTRY_TYPE_FLOAT) { // entity health
      conn->player.health = current_element->payload.float_data;
      printf("Health %f\n", conn->player.health);
    }
  }

deffer:
  FREE(packet.meta_data.entries);
}

int main2() {
  srand(time(NULL));
  char *errmsg = NULL;
  MConn *conn = MConn_init("127.0.0.1", 25565, &errmsg);

  MConn_connect(conn, &errmsg);
  EXIT_IF_ERR("can connect: %s\n")

  char *name = MALLOC(7); // BotXXX\0
  sprintf(name, "Bot%03d", rand() % 999);
  send_packet_C2S_handshake(conn, 47, "127.0.0.1", 25565, CONN_STATE_LOGIN,
                            &errmsg);
  send_packet_C2S_login_start(conn, name, &errmsg);
  FREE(name);

  conn->state = CONN_STATE_LOGIN;
  while (conn->state != CONN_STATE_OFFLINE) {
    MCbuffer *buff = MConn_recive_packet(conn, &errmsg);
    EXIT_IF_ERR("err while reciving packet: %s\n")
    int packet_id = MCbuffer_unpack_varint(buff, &errmsg);
    EXIT_IF_ERR("err while looking at packet id: %s\n");
    char *packet_name =
        packet_data_to_string(packet_id, conn->state, DIRECTION_S2C);
    if (!strcmp(packet_name, "PACKET_UNKNOWN")) {
      printf("Packet unknown: 0x%02X, DIR: S2C, STATE: %hhu\n", packet_id,
             conn->state);
    } else {
      puts(packet_name);
    }
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
      case PACKETID_S2C_LOGIN_SUCCESS:
        S2C_login_success(conn, buff, &errmsg);
        break;
      }
      break;
    case CONN_STATE_PLAY:
      switch (packet_id) {
      case PACKETID_S2C_PLAY_KEEP_ALIVE:
        S2C_play_keep_alive(conn, buff, &errmsg);
        break;
      case PACKETID_S2C_PLAY_CHAT_MESSAGE:
        S2C_play_chat_message(conn, buff, &errmsg);
        break;
      case PACKETID_S2C_PLAY_DISCONNECT:
        S2C_play_disconnect(conn, buff, &errmsg);
        break;
      case PACKETID_S2C_PLAY_ENTITY_STATUS:
        S2C_play_entity_status(conn, buff, &errmsg);
        break;
      case PACKETID_S2C_PLAY_ENTITY_METADATA:
        S2C_play_entity_metadata(conn, buff, &errmsg);
        break;
      case PACKETID_S2C_PLAY_JOIN_GAME:
        S2C_play_join_game(conn, buff, &errmsg);
        break;
      }
      break;
    case CONN_STATE_OFFLINE:
    case CONN_STATE_STATUS:
    case CONN_STATE_HANDSHAKE:
      fprintf(stderr, "invalid state\n");
      break;
    }
    MCbuffer_free(buff);
    EXIT_IF_ERR("error in packet loop %s\n")
  }
  return 0;
}

int packet_loop(MConn *conn, char **errmsg) {
  while (conn->state != CONN_STATE_OFFLINE) {
    MCbuffer *buff = MConn_recive_packet(conn, errmsg);
    if (*errmsg != ((void *)0)) {
      fprintf(stderr, "err while reciving packet: %s\n", *errmsg);
      return 1;
    }

    int packet_id = MCbuffer_unpack_varint(buff, errmsg);
    if (*errmsg != ((void *)0)) {
      fprintf(stderr, "err while looking at packet id: %s\n", *errmsg);
      return 1;
    }
    switch (conn->state) {
    case CONN_STATE_LOGIN:
      switch (packet_id) {
      case PACKETID_S2C_LOGIN_SET_COMPRESSION:
        S2C_login_set_compression(conn, buff, errmsg);
        break;
      case PACKETID_S2C_LOGIN_DISCONNECT:
        S2C_login_disconnect(conn, buff, errmsg);
        break;
      case PACKETID_S2C_LOGIN_SUCCESS:
        S2C_login_success(conn, buff, errmsg);
        break;
      }
      break;
    case CONN_STATE_PLAY:
      switch (packet_id) {
      case PACKETID_S2C_PLAY_KEEP_ALIVE:
        S2C_play_keep_alive(conn, buff, errmsg);
        break;
      }
      break;
    case CONN_STATE_OFFLINE:
    case CONN_STATE_STATUS:
    case CONN_STATE_HANDSHAKE:
      *errmsg = "invalid state";
      return 1;
    }
  }

  return 0;
}

int main() {
  char *errmsg = NULL;
  MConn *conn = MConn_init("193.123.108.138", 25565, &errmsg);

  int processes_created = 0;

  while (processes_created < 100) {
    pid_t pid = fork();

    if (pid < 0) {
      perror("Fork failed");
      exit(EXIT_FAILURE);
    } else if (pid == 0) {
      // Name gen
      srand(time(NULL) ^ getpid());
      char *name = MALLOC(9); // BotXXX\0
      sprintf(name, "Bot%06d", rand() % 999999);

      MConn_connect(conn, &errmsg);
      EXIT_IF_ERR("cant connect: %s\n");

      send_packet_C2S_handshake(conn, 47, "127.0.0.1", 25565, CONN_STATE_LOGIN,
                                &errmsg);
      send_packet_C2S_login_start(conn, name, &errmsg);
      EXIT_IF_ERR("handshake went wrong: %s\n");

      FREE(name);
      conn->state = CONN_STATE_LOGIN;

      packet_loop(conn, &errmsg);

      exit(EXIT_SUCCESS);
    } else {
      processes_created++;
      printf("[SPAWNER] created child process %d (PID %d)\n", processes_created,
             pid);
    }
  }

  // Parent process waits for all child processes to exit
  while (wait(NULL) > 0)
    ;

  printf("All processes have exited.\n");

  return 0;
}