//This code is generated.

#include "Packets.h"
#include "MCbuffer.h"
#include "MConn.h"
#include "MCtypes.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

/*
If you ask yourself WHY THE FUCK is it that instead of this:
sfprintf(*errmsg, "error while unpacking/packing " #pack_name/unpack_name ":
%s", *errmsg); wellll compiler does this if i dont make it like this: argument 3
[-Wrestrict] 11 |     sprintf(*errmsg, "error while packing " #pack_name ": %s",
*errmsg);             |             ^~~~~~~ ~~~~~~~ src/Packets.c:26:3: note: in
expansion of macro 'PACK_ERR_HANDELER' 26 |   PACK_ERR_HANDELER(keep_alive)
MConn_send_and_free_buffer(conn, buff, errmsg); |   ^~~~~~~~~~~~~~~~~

and i dont like that so made it "safer" ig...

*/

#define PACK_ERR_HANDELER(pack_name)                                           \
  if (*errmsg != NULL) {                                                       \
    char error_message[256];                                                   \
    sprintf(error_message, "error while packing " #pack_name ": %s", *errmsg); \
    free(buff);                                                                \
    *errmsg = strdup(error_message);                                           \
    return;                                                                    \
  }

#define UNPACK_ERR_HANDELER(unpack_name)                                       \
  if (buff->position != buff->length)                                          \
    *errmsg = "unpack function didnt do the entire buffer";                    \
  if (*errmsg != NULL) {                                                       \
    char error_message[256];                                                   \
    sprintf(error_message, "error while unpacking " #unpack_name ": %s",       \
            *errmsg);                                                          \
    *errmsg = strdup(error_message);                                           \
    return packet;                                                             \
  }

char *packet_data_to_string(int packet_id, MConn_state state,
                            packet_direction direction) {
#define PACKET_DATA_TO_STRING_UTIL(packet_id, state, direction, packet_name)   \
  case (packet_id & 0x00FFFFFF) | (state << 24) | (direction << 23):           \
    return packet_name;

  int combined_packet_data =
      (packet_id & 0x00FFFFFF) | (state << 24) | (direction << 23);

  switch (combined_packet_data) {
  PACKET_DATA_TO_STRING_UTIL(0x00, CONN_STATE_HANDSHAKE, DIRECTION_C2S, "C2S_HANDSHAKE");
  PACKET_DATA_TO_STRING_UTIL(0x00, CONN_STATE_STATUS, DIRECTION_S2C, "S2C_STATUS_RESPONSE");
  PACKET_DATA_TO_STRING_UTIL(0x00, CONN_STATE_STATUS, DIRECTION_C2S, "C2S_STATUS_REQUEST");
  PACKET_DATA_TO_STRING_UTIL(0x00, CONN_STATE_LOGIN, DIRECTION_S2C, "S2C_LOGIN_DISCONNECT");
  PACKET_DATA_TO_STRING_UTIL(0x01, CONN_STATE_LOGIN, DIRECTION_S2C, "S2C_LOGIN_ENCRYPTION_REQUEST");
  PACKET_DATA_TO_STRING_UTIL(0x02, CONN_STATE_LOGIN, DIRECTION_S2C, "S2C_LOGIN_SUCCESS");
  PACKET_DATA_TO_STRING_UTIL(0x03, CONN_STATE_LOGIN, DIRECTION_S2C, "S2C_LOGIN_SET_COMPRESSION");
  PACKET_DATA_TO_STRING_UTIL(0x00, CONN_STATE_LOGIN, DIRECTION_C2S, "C2S_LOGIN_START");
  PACKET_DATA_TO_STRING_UTIL(0x01, CONN_STATE_LOGIN, DIRECTION_C2S, "C2S_LOGIN_ENCRYPTION_RESPONSE");
  PACKET_DATA_TO_STRING_UTIL(0x00, CONN_STATE_PLAY, DIRECTION_S2C, "S2C_PLAY_KEEP_ALIVE");
  PACKET_DATA_TO_STRING_UTIL(0x02, CONN_STATE_PLAY, DIRECTION_S2C, "S2C_PLAY_JOIN_GAME");
  PACKET_DATA_TO_STRING_UTIL(0x00, CONN_STATE_PLAY, DIRECTION_C2S, "C2S_PLAY_KEEP_ALIVE");
  default:
    return "PACKET_UNKNOWN";
  }

#undef PACKET_DATA_TO_STRING_UTIL
}

void send_packet_C2S_handshake(MConn *conn, varint_t protocole_version, char *server_addr, unsigned short server_port, varint_t next_state, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, 0x00, errmsg);
  MCbuffer_pack_varint(buff, protocole_version, errmsg);
  MCbuffer_pack_string(buff, server_addr, errmsg);
  MCbuffer_pack_ushort(buff, server_port, errmsg);
  MCbuffer_pack_varint(buff, next_state, errmsg);
  PACK_ERR_HANDELER(C2S_handshake);
  MConn_send_packet(conn, buff, errmsg);
}

C2S_handshake_packet_t unpack_C2S_handshake_packet(MCbuffer *buff, char **errmsg) {
  C2S_handshake_packet_t packet;
  packet.protocole_version=MCbuffer_unpack_varint(buff,errmsg);
  packet.server_addr=MCbuffer_unpack_string(buff,errmsg);
  packet.server_port=MCbuffer_unpack_ushort(buff,errmsg);
  packet.next_state=MCbuffer_unpack_varint(buff,errmsg);
  UNPACK_ERR_HANDELER(C2S_handshake);
  return packet;
}

void send_packet_S2C_status_response(MConn *conn, json_t *response, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, 0x00, errmsg);
  MCbuffer_pack_json(buff, response, errmsg);
  PACK_ERR_HANDELER(S2C_status_response);
  MConn_send_packet(conn, buff, errmsg);
}

S2C_status_response_packet_t unpack_S2C_status_response_packet(MCbuffer *buff, char **errmsg) {
  S2C_status_response_packet_t packet;
  packet.response=MCbuffer_unpack_json(buff,errmsg);
  UNPACK_ERR_HANDELER(S2C_status_response);
  return packet;
}

void send_packet_C2S_status_request(MConn *conn, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, 0x00, errmsg);
  PACK_ERR_HANDELER(C2S_status_request);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_login_disconnect(MConn *conn, json_t *reason, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, 0x00, errmsg);
  MCbuffer_pack_json(buff, reason, errmsg);
  PACK_ERR_HANDELER(S2C_login_disconnect);
  MConn_send_packet(conn, buff, errmsg);
}

S2C_login_disconnect_packet_t unpack_S2C_login_disconnect_packet(MCbuffer *buff, char **errmsg) {
  S2C_login_disconnect_packet_t packet;
  packet.reason=MCbuffer_unpack_json(buff,errmsg);
  UNPACK_ERR_HANDELER(S2C_login_disconnect);
  return packet;
}

void send_packet_S2C_login_encryption_request(MConn *conn, char *server_id, MCbuffer *public_key, MCbuffer *verify_token, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, 0x01, errmsg);
  MCbuffer_pack_string(buff, server_id, errmsg);
  MCbuffer_pack_byte_array(buff, public_key, errmsg);
  MCbuffer_pack_byte_array(buff, verify_token, errmsg);
  PACK_ERR_HANDELER(S2C_login_encryption_request);
  MConn_send_packet(conn, buff, errmsg);
}

S2C_login_encryption_request_packet_t unpack_S2C_login_encryption_request_packet(MCbuffer *buff, char **errmsg) {
  S2C_login_encryption_request_packet_t packet;
  packet.server_id=MCbuffer_unpack_string(buff,errmsg);
  packet.public_key=MCbuffer_unpack_byte_array(buff,errmsg);
  packet.verify_token=MCbuffer_unpack_byte_array(buff,errmsg);
  UNPACK_ERR_HANDELER(S2C_login_encryption_request);
  return packet;
}

void send_packet_S2C_login_success(MConn *conn, char *uuid, char *name, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, 0x02, errmsg);
  MCbuffer_pack_string(buff, uuid, errmsg);
  MCbuffer_pack_string(buff, name, errmsg);
  PACK_ERR_HANDELER(S2C_login_success);
  MConn_send_packet(conn, buff, errmsg);
}

S2C_login_success_packet_t unpack_S2C_login_success_packet(MCbuffer *buff, char **errmsg) {
  S2C_login_success_packet_t packet;
  packet.uuid=MCbuffer_unpack_string(buff,errmsg);
  packet.name=MCbuffer_unpack_string(buff,errmsg);
  UNPACK_ERR_HANDELER(S2C_login_success);
  return packet;
}

void send_packet_S2C_login_set_compression(MConn *conn, varint_t threshold, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, 0x03, errmsg);
  MCbuffer_pack_varint(buff, threshold, errmsg);
  PACK_ERR_HANDELER(S2C_login_set_compression);
  MConn_send_packet(conn, buff, errmsg);
}

S2C_login_set_compression_packet_t unpack_S2C_login_set_compression_packet(MCbuffer *buff, char **errmsg) {
  S2C_login_set_compression_packet_t packet;
  packet.threshold=MCbuffer_unpack_varint(buff,errmsg);
  UNPACK_ERR_HANDELER(S2C_login_set_compression);
  return packet;
}

void send_packet_C2S_login_start(MConn *conn, char *name, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, 0x00, errmsg);
  MCbuffer_pack_string(buff, name, errmsg);
  PACK_ERR_HANDELER(C2S_login_start);
  MConn_send_packet(conn, buff, errmsg);
}

C2S_login_start_packet_t unpack_C2S_login_start_packet(MCbuffer *buff, char **errmsg) {
  C2S_login_start_packet_t packet;
  packet.name=MCbuffer_unpack_string(buff,errmsg);
  UNPACK_ERR_HANDELER(C2S_login_start);
  return packet;
}

void send_packet_C2S_login_encryption_response(MConn *conn, MCbuffer *shared_secret, MCbuffer *verify_token, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, 0x01, errmsg);
  MCbuffer_pack_byte_array(buff, shared_secret, errmsg);
  MCbuffer_pack_byte_array(buff, verify_token, errmsg);
  PACK_ERR_HANDELER(C2S_login_encryption_response);
  MConn_send_packet(conn, buff, errmsg);
}

C2S_login_encryption_response_packet_t unpack_C2S_login_encryption_response_packet(MCbuffer *buff, char **errmsg) {
  C2S_login_encryption_response_packet_t packet;
  packet.shared_secret=MCbuffer_unpack_byte_array(buff,errmsg);
  packet.verify_token=MCbuffer_unpack_byte_array(buff,errmsg);
  UNPACK_ERR_HANDELER(C2S_login_encryption_response);
  return packet;
}

void send_packet_S2C_play_keep_alive(MConn *conn, varint_t keep_alive_id, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, 0x00, errmsg);
  MCbuffer_pack_varint(buff, keep_alive_id, errmsg);
  PACK_ERR_HANDELER(S2C_play_keep_alive);
  MConn_send_packet(conn, buff, errmsg);
}

S2C_play_keep_alive_packet_t unpack_S2C_play_keep_alive_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_keep_alive_packet_t packet;
  packet.keep_alive_id=MCbuffer_unpack_varint(buff,errmsg);
  UNPACK_ERR_HANDELER(S2C_play_keep_alive);
  return packet;
}

void send_packet_S2C_play_join_game(MConn *conn, int entity_id, byte_t gamemode, char dimension, byte_t difficulty, byte_t max_players, char *level_type, bool reduced_debug_info, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, 0x02, errmsg);
  MCbuffer_pack_int(buff, entity_id, errmsg);
  MCbuffer_pack_byte(buff, gamemode, errmsg);
  MCbuffer_pack_char(buff, dimension, errmsg);
  MCbuffer_pack_byte(buff, difficulty, errmsg);
  MCbuffer_pack_byte(buff, max_players, errmsg);
  MCbuffer_pack_string(buff, level_type, errmsg);
  MCbuffer_pack_bool(buff, reduced_debug_info, errmsg);
  PACK_ERR_HANDELER(S2C_play_join_game);
  MConn_send_packet(conn, buff, errmsg);
}

S2C_play_join_game_packet_t unpack_S2C_play_join_game_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_join_game_packet_t packet;
  packet.entity_id=MCbuffer_unpack_int(buff,errmsg);
  packet.gamemode=MCbuffer_unpack_byte(buff,errmsg);
  packet.dimension=MCbuffer_unpack_char(buff,errmsg);
  packet.difficulty=MCbuffer_unpack_byte(buff,errmsg);
  packet.max_players=MCbuffer_unpack_byte(buff,errmsg);
  packet.level_type=MCbuffer_unpack_string(buff,errmsg);
  packet.reduced_debug_info=MCbuffer_unpack_bool(buff,errmsg);
  UNPACK_ERR_HANDELER(S2C_play_join_game);
  return packet;
}

void send_packet_C2S_play_keep_alive(MConn *conn, varint_t keep_alive_id, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, 0x00, errmsg);
  MCbuffer_pack_varint(buff, keep_alive_id, errmsg);
  PACK_ERR_HANDELER(C2S_play_keep_alive);
  MConn_send_packet(conn, buff, errmsg);
}

C2S_play_keep_alive_packet_t unpack_C2S_play_keep_alive_packet(MCbuffer *buff, char **errmsg) {
  C2S_play_keep_alive_packet_t packet;
  packet.keep_alive_id=MCbuffer_unpack_varint(buff,errmsg);
  UNPACK_ERR_HANDELER(C2S_play_keep_alive);
  return packet;
}

