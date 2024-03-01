#include "packets.h"
#include "buffer.h"
#include "conn.h"
#include "err.h"
#include "heap_utils.h"
#include "packet_types.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNPACK_ERR_HANDELER                                                    \
  ERR_CHECK(return packet;);                                                   \
  if (buff->position != buff->length)                                          \
    ERR(ERR_BUFFER_UNDERUN, return packet;);

cmc_packet_name_id cmc_packet_id_to_packet_name_id(int packet_id,
                                                   cmc_conn_state state,
                                                   packet_direction direction,
                                                   int protocol_version) {
  switch ((protocol_version | ((uint64_t)state) << 32 |
           ((uint64_t)direction) << 35 | ((uint64_t)packet_id) << 36)) {
  // CGSS: packet_id_to_packet_name_id
  case ((47 | ((uint64_t)CMC_CONN_STATE_HANDSHAKE) << 32 |
         ((uint64_t)CMC_DIRECTION_C2S) << 35 | ((uint64_t)0x00) << 36)):
    return CMC_C2S_HANDSHAKE_HANDSHAKE_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_STATUS) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x00) << 36)):
    return CMC_S2C_STATUS_RESPONSE_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_STATUS) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x01) << 36)):
    return CMC_S2C_STATUS_PONG_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_STATUS) << 32 |
         ((uint64_t)CMC_DIRECTION_C2S) << 35 | ((uint64_t)0x00) << 36)):
    return CMC_C2S_STATUS_REQUEST_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_STATUS) << 32 |
         ((uint64_t)CMC_DIRECTION_C2S) << 35 | ((uint64_t)0x01) << 36)):
    return CMC_C2S_STATUS_PING_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_LOGIN) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x00) << 36)):
    return CMC_S2C_LOGIN_DISCONNECT_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_LOGIN) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x01) << 36)):
    return CMC_S2C_LOGIN_ENCRYPTION_REQUEST_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_LOGIN) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x02) << 36)):
    return CMC_S2C_LOGIN_SUCCESS_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_LOGIN) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x03) << 36)):
    return CMC_S2C_LOGIN_SET_COMPRESSION_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_LOGIN) << 32 |
         ((uint64_t)CMC_DIRECTION_C2S) << 35 | ((uint64_t)0x00) << 36)):
    return CMC_C2S_LOGIN_START_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_LOGIN) << 32 |
         ((uint64_t)CMC_DIRECTION_C2S) << 35 | ((uint64_t)0x01) << 36)):
    return CMC_C2S_LOGIN_ENCRYPTION_RESPONSE_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x00) << 36)):
    return CMC_S2C_PLAY_KEEP_ALIVE_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x01) << 36)):
    return CMC_S2C_PLAY_JOIN_GAME_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x02) << 36)):
    return CMC_S2C_PLAY_CHAT_MESSAGE_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x03) << 36)):
    return CMC_S2C_PLAY_TIME_UPDATE_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x04) << 36)):
    return CMC_S2C_PLAY_ENTITY_EQUIPMENT_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x05) << 36)):
    return CMC_S2C_PLAY_SPAWN_POSITION_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x06) << 36)):
    return CMC_S2C_PLAY_UPDATE_HEALTH_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x07) << 36)):
    return CMC_S2C_PLAY_RESPAWN_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x08) << 36)):
    return CMC_S2C_PLAY_PLAYER_LOOK_AND_POSITION_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x09) << 36)):
    return CMC_S2C_PLAY_HELD_ITEM_CHANGE_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x0A) << 36)):
    return CMC_S2C_PLAY_USE_BED_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x0B) << 36)):
    return CMC_S2C_PLAY_ANIMATION_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x0C) << 36)):
    return CMC_S2C_PLAY_SPAWN_PLAYER_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x0D) << 36)):
    return CMC_S2C_PLAY_COLLECT_ITEM_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x0F) << 36)):
    return CMC_S2C_PLAY_SPAWN_MOB_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x10) << 36)):
    return CMC_S2C_PLAY_SPAWN_PAINTING_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x11) << 36)):
    return CMC_S2C_PLAY_SPAWN_EXPERIENCE_ORB_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x12) << 36)):
    return CMC_S2C_PLAY_ENTITY_VELOCITY_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x14) << 36)):
    return CMC_S2C_PLAY_ENTITY_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x15) << 36)):
    return CMC_S2C_PLAY_ENTITY_RELATIVE_MOVE_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x16) << 36)):
    return CMC_S2C_PLAY_ENTITY_LOOK_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x17) << 36)):
    return CMC_S2C_PLAY_ENTITY_LOOK_AND_RELATIVE_MOVE_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x18) << 36)):
    return CMC_S2C_PLAY_ENTITY_TELEPORT_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x19) << 36)):
    return CMC_S2C_PLAY_ENTITY_HEAD_LOOK_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x1A) << 36)):
    return CMC_S2C_PLAY_ENTITY_STATUS_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x1B) << 36)):
    return CMC_S2C_PLAY_ATTACH_ENTITY_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x1C) << 36)):
    return CMC_S2C_PLAY_ENTITY_METADATA_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x1D) << 36)):
    return CMC_S2C_PLAY_ENTITY_EFFECT_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x1E) << 36)):
    return CMC_S2C_PLAY_REMOVE_ENTITY_EFFECT_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x1F) << 36)):
    return CMC_S2C_PLAY_SET_EXPERIENCE_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x21) << 36)):
    return CMC_S2C_PLAY_CHUNK_DATA_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x23) << 36)):
    return CMC_S2C_PLAY_BLOCK_CHANGE_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x24) << 36)):
    return CMC_S2C_PLAY_BLOCK_ACTION_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x25) << 36)):
    return CMC_S2C_PLAY_BLOCK_BREAK_ANIMATION_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x28) << 36)):
    return CMC_S2C_PLAY_EFFECT_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x29) << 36)):
    return CMC_S2C_PLAY_SOUND_EFFECT_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x2B) << 36)):
    return CMC_S2C_PLAY_CHANGE_GAME_STATE_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x39) << 36)):
    return CMC_S2C_PLAY_PLAYER_ABILITIES_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x3F) << 36)):
    return CMC_S2C_PLAY_PLUGIN_MESSAGE_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x40) << 36)):
    return CMC_S2C_PLAY_DISCONNECT_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_S2C) << 35 | ((uint64_t)0x41) << 36)):
    return CMC_S2C_PLAY_CHANGE_DIFFICULTY_NAME_ID;
  case ((47 | ((uint64_t)CMC_CONN_STATE_PLAY) << 32 |
         ((uint64_t)CMC_DIRECTION_C2S) << 35 | ((uint64_t)0x00) << 36)):
    return CMC_C2S_PLAY_KEEP_ALIVE_NAME_ID;
    // CGSE: packet_id_to_packet_name_id
  default:
    return CMC_INVALID_NAME_ID;
  }
}

// CGSS: send_methods_c
void cmc_send_C2S_handshake_handshake_packet(
    cmc_conn *conn, C2S_handshake_handshake_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x00);
    packet->protocole_version = cmc_buffer_unpack_varint(buff);
    packet->server_addr = cmc_buffer_unpack_string(buff);
    packet->server_port = cmc_buffer_unpack_ushort(buff);
    packet->next_state = cmc_buffer_unpack_varint(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_status_response_packet(cmc_conn *conn,
                                         S2C_status_response_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x00);
    packet->response = cmc_buffer_unpack_string(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_status_pong_packet(cmc_conn *conn,
                                     S2C_status_pong_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x01);
    packet->payload = cmc_buffer_unpack_long(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_C2S_status_request_packet(cmc_conn *conn) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x00);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_C2S_status_ping_packet(cmc_conn *conn,
                                     C2S_status_ping_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x01);
    packet->payload = cmc_buffer_unpack_long(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_login_disconnect_packet(cmc_conn *conn,
                                          S2C_login_disconnect_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x00);
    packet->reason = cmc_buffer_unpack_string(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_login_encryption_request_packet(
    cmc_conn *conn, S2C_login_encryption_request_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x01);
    packet->server_id = cmc_buffer_unpack_string(buff);
    packet->public_key = cmc_buffer_unpack_byte_array(buff);
    packet->verify_token = cmc_buffer_unpack_byte_array(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_login_success_packet(cmc_conn *conn,
                                       S2C_login_success_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x02);
    packet->uuid = cmc_buffer_unpack_string(buff);
    packet->name = cmc_buffer_unpack_string(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_login_set_compression_packet(
    cmc_conn *conn, S2C_login_set_compression_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x03);
    packet->threshold = cmc_buffer_unpack_varint(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_C2S_login_start_packet(cmc_conn *conn,
                                     C2S_login_start_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x00);
    packet->name = cmc_buffer_unpack_string(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_C2S_login_encryption_response_packet(
    cmc_conn *conn, C2S_login_encryption_response_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x01);
    packet->shared_secret = cmc_buffer_unpack_byte_array(buff);
    packet->verify_token = cmc_buffer_unpack_byte_array(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_keep_alive_packet(cmc_conn *conn,
                                         S2C_play_keep_alive_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x00);
    packet->keep_alive_id = cmc_buffer_unpack_varint(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_join_game_packet(cmc_conn *conn,
                                        S2C_play_join_game_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x01);
    packet->entity_id = cmc_buffer_unpack_int(buff);
    packet->gamemode = cmc_buffer_unpack_byte(buff);
    packet->dimension = cmc_buffer_unpack_char(buff);
    packet->difficulty = cmc_buffer_unpack_byte(buff);
    packet->max_players = cmc_buffer_unpack_byte(buff);
    packet->level_type = cmc_buffer_unpack_string(buff);
    packet->reduced_debug_info = cmc_buffer_unpack_bool(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_chat_message_packet(
    cmc_conn *conn, S2C_play_chat_message_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x02);
    packet->message = cmc_buffer_unpack_string(buff);
    packet->position = cmc_buffer_unpack_char(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_time_update_packet(cmc_conn *conn,
                                          S2C_play_time_update_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x03);
    packet->world_age = cmc_buffer_unpack_long(buff);
    packet->time_of_day = cmc_buffer_unpack_long(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_entity_equipment_packet(
    cmc_conn *conn, S2C_play_entity_equipment_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x04);
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->slot = cmc_buffer_unpack_short(buff);
    packet->item = cmc_buffer_unpack_slot(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_spawn_position_packet(
    cmc_conn *conn, S2C_play_spawn_position_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x05);
    packet->location = cmc_buffer_unpack_position(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_update_health_packet(
    cmc_conn *conn, S2C_play_update_health_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x06);
    packet->health = cmc_buffer_unpack_float(buff);
    packet->food = cmc_buffer_unpack_varint(buff);
    packet->food_saturation = cmc_buffer_unpack_float(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_respawn_packet(cmc_conn *conn,
                                      S2C_play_respawn_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x07);
    packet->dimesion = cmc_buffer_unpack_int(buff);
    packet->difficulty = cmc_buffer_unpack_byte(buff);
    packet->gamemode = cmc_buffer_unpack_byte(buff);
    packet->level_type = cmc_buffer_unpack_string(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_player_look_and_position_packet(
    cmc_conn *conn, S2C_play_player_look_and_position_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x08);
    packet->x = cmc_buffer_unpack_double(buff);
    packet->y = cmc_buffer_unpack_double(buff);
    packet->z = cmc_buffer_unpack_double(buff);
    packet->yaw = cmc_buffer_unpack_float(buff);
    packet->pitch = cmc_buffer_unpack_float(buff);
    packet->flags = cmc_buffer_unpack_byte(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_held_item_change_packet(
    cmc_conn *conn, S2C_play_held_item_change_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x09);
    packet->slot = cmc_buffer_unpack_char(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_use_bed_packet(cmc_conn *conn,
                                      S2C_play_use_bed_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x0A);
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->location = cmc_buffer_unpack_position(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_animation_packet(cmc_conn *conn,
                                        S2C_play_animation_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x0B);
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->animation = cmc_buffer_unpack_byte(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_spawn_player_packet(
    cmc_conn *conn, S2C_play_spawn_player_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x0C);
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->uuid = cmc_buffer_unpack_ullong(buff);
    packet->x = cmc_buffer_unpack_int(buff);
    packet->y = cmc_buffer_unpack_int(buff);
    packet->z = cmc_buffer_unpack_int(buff);
    packet->yaw = cmc_buffer_unpack_byte(buff);
    packet->pitch = cmc_buffer_unpack_byte(buff);
    packet->current_item = cmc_buffer_unpack_short(buff);
    packet->meta_data = cmc_buffer_unpack_entity_metadata(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_collect_item_packet(
    cmc_conn *conn, S2C_play_collect_item_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x0D);
    packet->collected_entity_id = cmc_buffer_unpack_varint(buff);
    packet->collector_entity_id = cmc_buffer_unpack_varint(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_spawn_mob_packet(cmc_conn *conn,
                                        S2C_play_spawn_mob_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x0F);
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->type = cmc_buffer_unpack_byte(buff);
    packet->x = cmc_buffer_unpack_int(buff);
    packet->y = cmc_buffer_unpack_int(buff);
    packet->z = cmc_buffer_unpack_int(buff);
    packet->yaw = cmc_buffer_unpack_byte(buff);
    packet->pitch = cmc_buffer_unpack_byte(buff);
    packet->head_pitch = cmc_buffer_unpack_byte(buff);
    packet->x_vel = cmc_buffer_unpack_short(buff);
    packet->y_vel = cmc_buffer_unpack_short(buff);
    packet->z_vel = cmc_buffer_unpack_short(buff);
    packet->meta_data = cmc_buffer_unpack_entity_metadata(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_spawn_painting_packet(
    cmc_conn *conn, S2C_play_spawn_painting_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x10);
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->title = cmc_buffer_unpack_string(buff);
    packet->location = cmc_buffer_unpack_position(buff);
    packet->direction = cmc_buffer_unpack_byte(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_spawn_experience_orb_packet(
    cmc_conn *conn, S2C_play_spawn_experience_orb_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x11);
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->x = cmc_buffer_unpack_int(buff);
    packet->y = cmc_buffer_unpack_int(buff);
    packet->z = cmc_buffer_unpack_int(buff);
    packet->count = cmc_buffer_unpack_short(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_entity_velocity_packet(
    cmc_conn *conn, S2C_play_entity_velocity_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x12);
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->x_vel = cmc_buffer_unpack_short(buff);
    packet->y_vel = cmc_buffer_unpack_short(buff);
    packet->z_vel = cmc_buffer_unpack_short(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_entity_packet(cmc_conn *conn,
                                     S2C_play_entity_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x14);
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_entity_relative_move_packet(
    cmc_conn *conn, S2C_play_entity_relative_move_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x15);
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->delta_x = cmc_buffer_unpack_char(buff);
    packet->delta_y = cmc_buffer_unpack_char(buff);
    packet->delta_z = cmc_buffer_unpack_char(buff);
    packet->on_ground = cmc_buffer_unpack_bool(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_entity_look_packet(cmc_conn *conn,
                                          S2C_play_entity_look_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x16);
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->yaw = cmc_buffer_unpack_byte(buff);
    packet->pitch = cmc_buffer_unpack_byte(buff);
    packet->on_ground = cmc_buffer_unpack_bool(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_entity_look_and_relative_move_packet(
    cmc_conn *conn, S2C_play_entity_look_and_relative_move_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x17);
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->delta_x = cmc_buffer_unpack_char(buff);
    packet->delta_y = cmc_buffer_unpack_char(buff);
    packet->delta_z = cmc_buffer_unpack_char(buff);
    packet->yaw = cmc_buffer_unpack_byte(buff);
    packet->pitch = cmc_buffer_unpack_byte(buff);
    packet->on_ground = cmc_buffer_unpack_bool(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_entity_teleport_packet(
    cmc_conn *conn, S2C_play_entity_teleport_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x18);
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->x = cmc_buffer_unpack_int(buff);
    packet->y = cmc_buffer_unpack_int(buff);
    packet->z = cmc_buffer_unpack_int(buff);
    packet->yaw = cmc_buffer_unpack_byte(buff);
    packet->pitch = cmc_buffer_unpack_byte(buff);
    packet->on_ground = cmc_buffer_unpack_bool(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_entity_head_look_packet(
    cmc_conn *conn, S2C_play_entity_head_look_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x19);
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->head_yaw = cmc_buffer_unpack_byte(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_entity_status_packet(
    cmc_conn *conn, S2C_play_entity_status_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x1A);
    packet->entity_id = cmc_buffer_unpack_int(buff);
    packet->entity_status = cmc_buffer_unpack_char(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_attach_entity_packet(
    cmc_conn *conn, S2C_play_attach_entity_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x1B);
    packet->entity_id = cmc_buffer_unpack_int(buff);
    packet->vehicle_id = cmc_buffer_unpack_int(buff);
    packet->leash = cmc_buffer_unpack_bool(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_entity_metadata_packet(
    cmc_conn *conn, S2C_play_entity_metadata_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x1C);
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->meta_data = cmc_buffer_unpack_entity_metadata(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_entity_effect_packet(
    cmc_conn *conn, S2C_play_entity_effect_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x1D);
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->effect_id = cmc_buffer_unpack_char(buff);
    packet->amplifier = cmc_buffer_unpack_char(buff);
    packet->duration = cmc_buffer_unpack_varint(buff);
    packet->hide_particles = cmc_buffer_unpack_bool(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_remove_entity_effect_packet(
    cmc_conn *conn, S2C_play_remove_entity_effect_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x1E);
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->effect_id = cmc_buffer_unpack_char(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_set_experience_packet(
    cmc_conn *conn, S2C_play_set_experience_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x1F);
    packet->experience_bar = cmc_buffer_unpack_float(buff);
    packet->level = cmc_buffer_unpack_varint(buff);
    packet->total_experience = cmc_buffer_unpack_varint(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_chunk_data_packet(cmc_conn *conn,
                                         S2C_play_chunk_data_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x21);
    packet->chunk_x = cmc_buffer_unpack_int(buff);
    packet->chunk_z = cmc_buffer_unpack_int(buff);
    packet->ground_up_continuous = cmc_buffer_unpack_bool(buff);
    packet->primary_bitmask = cmc_buffer_unpack_ushort(buff);
    packet->chunk = cmc_buffer_unpack_byte_array(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_block_change_packet(
    cmc_conn *conn, S2C_play_block_change_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x23);
    packet->location = cmc_buffer_unpack_position(buff);
    packet->block_id = cmc_buffer_unpack_varint(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_block_action_packet(
    cmc_conn *conn, S2C_play_block_action_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x24);
    packet->location = cmc_buffer_unpack_position(buff);
    packet->block_data_1 = cmc_buffer_unpack_byte(buff);
    packet->block_data_2 = cmc_buffer_unpack_byte(buff);
    packet->block_type = cmc_buffer_unpack_varint(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_block_break_animation_packet(
    cmc_conn *conn, S2C_play_block_break_animation_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x25);
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->location = cmc_buffer_unpack_position(buff);
    packet->destroy_stage = cmc_buffer_unpack_char(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_effect_packet(cmc_conn *conn,
                                     S2C_play_effect_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x28);
    packet->effect_id = cmc_buffer_unpack_int(buff);
    packet->location = cmc_buffer_unpack_position(buff);
    packet->data = cmc_buffer_unpack_int(buff);
    packet->d = cmc_buffer_unpack_bool(buff);
    packet->particle_id = cmc_buffer_unpack_int(buff);
    packet->long_distances = cmc_buffer_unpack_bool(buff);
    packet->x = cmc_buffer_unpack_float(buff);
    packet->y = cmc_buffer_unpack_float(buff);
    packet->z = cmc_buffer_unpack_float(buff);
    packet->x_offset = cmc_buffer_unpack_float(buff);
    packet->y_offset = cmc_buffer_unpack_float(buff);
    packet->z_offset = cmc_buffer_unpack_float(buff);
    packet->particle_data = cmc_buffer_unpack_float(buff);
    packet->particle_count = cmc_buffer_unpack_int(buff);
    packet->sable_relative_volume = cmc_buffer_unpack_int(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_sound_effect_packet(
    cmc_conn *conn, S2C_play_sound_effect_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x29);
    packet->sound_name = cmc_buffer_unpack_string(buff);
    packet->x = cmc_buffer_unpack_int(buff);
    packet->y = cmc_buffer_unpack_int(buff);
    packet->z = cmc_buffer_unpack_int(buff);
    packet->volume = cmc_buffer_unpack_float(buff);
    packet->pitch = cmc_buffer_unpack_byte(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_change_game_state_packet(
    cmc_conn *conn, S2C_play_change_game_state_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x2B);
    packet->reason = cmc_buffer_unpack_byte(buff);
    packet->value = cmc_buffer_unpack_float(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_player_abilities_packet(
    cmc_conn *conn, S2C_play_player_abilities_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x39);
    packet->flags = cmc_buffer_unpack_char(buff);
    packet->flying_speed = cmc_buffer_unpack_float(buff);
    packet->fov_modifier = cmc_buffer_unpack_float(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_plugin_message_packet(
    cmc_conn *conn, S2C_play_plugin_message_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x3F);
    packet->channel = cmc_buffer_unpack_string(buff);
    packet->data = cmc_buffer_unpack_byte_array(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_disconnect_packet(cmc_conn *conn,
                                         S2C_play_disconnect_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x40);
    packet->reason = cmc_buffer_unpack_string(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_S2C_play_change_difficulty_packet(
    cmc_conn *conn, S2C_play_change_difficulty_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x41);
    packet->difficulty = cmc_buffer_unpack_byte(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

void cmc_send_C2S_play_keep_alive_packet(cmc_conn *conn,
                                         C2S_play_keep_alive_packet *packet) {
  cmc_buffer *buff = cmc_buffer_init();
  switch (buff->protocol_version) {
  case 47: {
    cmc_buffer_pack_varint(buff, 0x00);
    packet->keep_alive_id = cmc_buffer_unpack_varint(buff);
    break;
  }
  default:
    cmc_buffer_free(buff);
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buffer_free(buff);
}

// CGSE: send_methods_c

// CGSS: unpack_methods_c
C2S_handshake_handshake_packet *
unpack_C2S_handshake_handshake_packet(cmc_buffer *buff) {
  C2S_handshake_handshake_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->protocole_version = cmc_buffer_unpack_varint(buff);
    packet->server_addr = cmc_buffer_unpack_string(buff);
    packet->server_port = cmc_buffer_unpack_ushort(buff);
    packet->next_state = cmc_buffer_unpack_varint(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_status_response_packet *
unpack_S2C_status_response_packet(cmc_buffer *buff) {
  S2C_status_response_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->response = cmc_buffer_unpack_string(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_status_pong_packet *unpack_S2C_status_pong_packet(cmc_buffer *buff) {
  S2C_status_pong_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->payload = cmc_buffer_unpack_long(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

C2S_status_ping_packet *unpack_C2S_status_ping_packet(cmc_buffer *buff) {
  C2S_status_ping_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->payload = cmc_buffer_unpack_long(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_login_disconnect_packet *
unpack_S2C_login_disconnect_packet(cmc_buffer *buff) {
  S2C_login_disconnect_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->reason = cmc_buffer_unpack_string(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_login_encryption_request_packet *
unpack_S2C_login_encryption_request_packet(cmc_buffer *buff) {
  S2C_login_encryption_request_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->server_id = cmc_buffer_unpack_string(buff);
    packet->public_key = cmc_buffer_unpack_byte_array(buff);
    packet->verify_token = cmc_buffer_unpack_byte_array(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_login_success_packet *unpack_S2C_login_success_packet(cmc_buffer *buff) {
  S2C_login_success_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->uuid = cmc_buffer_unpack_string(buff);
    packet->name = cmc_buffer_unpack_string(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_login_set_compression_packet *
unpack_S2C_login_set_compression_packet(cmc_buffer *buff) {
  S2C_login_set_compression_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->threshold = cmc_buffer_unpack_varint(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

C2S_login_start_packet *unpack_C2S_login_start_packet(cmc_buffer *buff) {
  C2S_login_start_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->name = cmc_buffer_unpack_string(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

C2S_login_encryption_response_packet *
unpack_C2S_login_encryption_response_packet(cmc_buffer *buff) {
  C2S_login_encryption_response_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->shared_secret = cmc_buffer_unpack_byte_array(buff);
    packet->verify_token = cmc_buffer_unpack_byte_array(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_keep_alive_packet *
unpack_S2C_play_keep_alive_packet(cmc_buffer *buff) {
  S2C_play_keep_alive_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->keep_alive_id = cmc_buffer_unpack_varint(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_join_game_packet *unpack_S2C_play_join_game_packet(cmc_buffer *buff) {
  S2C_play_join_game_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->entity_id = cmc_buffer_unpack_int(buff);
    packet->gamemode = cmc_buffer_unpack_byte(buff);
    packet->dimension = cmc_buffer_unpack_char(buff);
    packet->difficulty = cmc_buffer_unpack_byte(buff);
    packet->max_players = cmc_buffer_unpack_byte(buff);
    packet->level_type = cmc_buffer_unpack_string(buff);
    packet->reduced_debug_info = cmc_buffer_unpack_bool(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_chat_message_packet *
unpack_S2C_play_chat_message_packet(cmc_buffer *buff) {
  S2C_play_chat_message_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->message = cmc_buffer_unpack_string(buff);
    packet->position = cmc_buffer_unpack_char(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_time_update_packet *
unpack_S2C_play_time_update_packet(cmc_buffer *buff) {
  S2C_play_time_update_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->world_age = cmc_buffer_unpack_long(buff);
    packet->time_of_day = cmc_buffer_unpack_long(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_equipment_packet *
unpack_S2C_play_entity_equipment_packet(cmc_buffer *buff) {
  S2C_play_entity_equipment_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->slot = cmc_buffer_unpack_short(buff);
    packet->item = cmc_buffer_unpack_slot(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_spawn_position_packet *
unpack_S2C_play_spawn_position_packet(cmc_buffer *buff) {
  S2C_play_spawn_position_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->location = cmc_buffer_unpack_position(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_update_health_packet *
unpack_S2C_play_update_health_packet(cmc_buffer *buff) {
  S2C_play_update_health_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->health = cmc_buffer_unpack_float(buff);
    packet->food = cmc_buffer_unpack_varint(buff);
    packet->food_saturation = cmc_buffer_unpack_float(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_respawn_packet *unpack_S2C_play_respawn_packet(cmc_buffer *buff) {
  S2C_play_respawn_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->dimesion = cmc_buffer_unpack_int(buff);
    packet->difficulty = cmc_buffer_unpack_byte(buff);
    packet->gamemode = cmc_buffer_unpack_byte(buff);
    packet->level_type = cmc_buffer_unpack_string(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_player_look_and_position_packet *
unpack_S2C_play_player_look_and_position_packet(cmc_buffer *buff) {
  S2C_play_player_look_and_position_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->x = cmc_buffer_unpack_double(buff);
    packet->y = cmc_buffer_unpack_double(buff);
    packet->z = cmc_buffer_unpack_double(buff);
    packet->yaw = cmc_buffer_unpack_float(buff);
    packet->pitch = cmc_buffer_unpack_float(buff);
    packet->flags = cmc_buffer_unpack_byte(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_held_item_change_packet *
unpack_S2C_play_held_item_change_packet(cmc_buffer *buff) {
  S2C_play_held_item_change_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->slot = cmc_buffer_unpack_char(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_use_bed_packet *unpack_S2C_play_use_bed_packet(cmc_buffer *buff) {
  S2C_play_use_bed_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->location = cmc_buffer_unpack_position(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_animation_packet *unpack_S2C_play_animation_packet(cmc_buffer *buff) {
  S2C_play_animation_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->animation = cmc_buffer_unpack_byte(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_spawn_player_packet *
unpack_S2C_play_spawn_player_packet(cmc_buffer *buff) {
  S2C_play_spawn_player_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->uuid = cmc_buffer_unpack_ullong(buff);
    packet->x = cmc_buffer_unpack_int(buff);
    packet->y = cmc_buffer_unpack_int(buff);
    packet->z = cmc_buffer_unpack_int(buff);
    packet->yaw = cmc_buffer_unpack_byte(buff);
    packet->pitch = cmc_buffer_unpack_byte(buff);
    packet->current_item = cmc_buffer_unpack_short(buff);
    packet->meta_data = cmc_buffer_unpack_entity_metadata(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_collect_item_packet *
unpack_S2C_play_collect_item_packet(cmc_buffer *buff) {
  S2C_play_collect_item_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->collected_entity_id = cmc_buffer_unpack_varint(buff);
    packet->collector_entity_id = cmc_buffer_unpack_varint(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_spawn_mob_packet *unpack_S2C_play_spawn_mob_packet(cmc_buffer *buff) {
  S2C_play_spawn_mob_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->type = cmc_buffer_unpack_byte(buff);
    packet->x = cmc_buffer_unpack_int(buff);
    packet->y = cmc_buffer_unpack_int(buff);
    packet->z = cmc_buffer_unpack_int(buff);
    packet->yaw = cmc_buffer_unpack_byte(buff);
    packet->pitch = cmc_buffer_unpack_byte(buff);
    packet->head_pitch = cmc_buffer_unpack_byte(buff);
    packet->x_vel = cmc_buffer_unpack_short(buff);
    packet->y_vel = cmc_buffer_unpack_short(buff);
    packet->z_vel = cmc_buffer_unpack_short(buff);
    packet->meta_data = cmc_buffer_unpack_entity_metadata(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_spawn_painting_packet *
unpack_S2C_play_spawn_painting_packet(cmc_buffer *buff) {
  S2C_play_spawn_painting_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->title = cmc_buffer_unpack_string(buff);
    packet->location = cmc_buffer_unpack_position(buff);
    packet->direction = cmc_buffer_unpack_byte(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_spawn_experience_orb_packet *
unpack_S2C_play_spawn_experience_orb_packet(cmc_buffer *buff) {
  S2C_play_spawn_experience_orb_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->x = cmc_buffer_unpack_int(buff);
    packet->y = cmc_buffer_unpack_int(buff);
    packet->z = cmc_buffer_unpack_int(buff);
    packet->count = cmc_buffer_unpack_short(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_velocity_packet *
unpack_S2C_play_entity_velocity_packet(cmc_buffer *buff) {
  S2C_play_entity_velocity_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->x_vel = cmc_buffer_unpack_short(buff);
    packet->y_vel = cmc_buffer_unpack_short(buff);
    packet->z_vel = cmc_buffer_unpack_short(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_packet *unpack_S2C_play_entity_packet(cmc_buffer *buff) {
  S2C_play_entity_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_relative_move_packet *
unpack_S2C_play_entity_relative_move_packet(cmc_buffer *buff) {
  S2C_play_entity_relative_move_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->delta_x = cmc_buffer_unpack_char(buff);
    packet->delta_y = cmc_buffer_unpack_char(buff);
    packet->delta_z = cmc_buffer_unpack_char(buff);
    packet->on_ground = cmc_buffer_unpack_bool(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_look_packet *
unpack_S2C_play_entity_look_packet(cmc_buffer *buff) {
  S2C_play_entity_look_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->yaw = cmc_buffer_unpack_byte(buff);
    packet->pitch = cmc_buffer_unpack_byte(buff);
    packet->on_ground = cmc_buffer_unpack_bool(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_look_and_relative_move_packet *
unpack_S2C_play_entity_look_and_relative_move_packet(cmc_buffer *buff) {
  S2C_play_entity_look_and_relative_move_packet *packet =
      MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->delta_x = cmc_buffer_unpack_char(buff);
    packet->delta_y = cmc_buffer_unpack_char(buff);
    packet->delta_z = cmc_buffer_unpack_char(buff);
    packet->yaw = cmc_buffer_unpack_byte(buff);
    packet->pitch = cmc_buffer_unpack_byte(buff);
    packet->on_ground = cmc_buffer_unpack_bool(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_teleport_packet *
unpack_S2C_play_entity_teleport_packet(cmc_buffer *buff) {
  S2C_play_entity_teleport_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->x = cmc_buffer_unpack_int(buff);
    packet->y = cmc_buffer_unpack_int(buff);
    packet->z = cmc_buffer_unpack_int(buff);
    packet->yaw = cmc_buffer_unpack_byte(buff);
    packet->pitch = cmc_buffer_unpack_byte(buff);
    packet->on_ground = cmc_buffer_unpack_bool(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_head_look_packet *
unpack_S2C_play_entity_head_look_packet(cmc_buffer *buff) {
  S2C_play_entity_head_look_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->head_yaw = cmc_buffer_unpack_byte(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_status_packet *
unpack_S2C_play_entity_status_packet(cmc_buffer *buff) {
  S2C_play_entity_status_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->entity_id = cmc_buffer_unpack_int(buff);
    packet->entity_status = cmc_buffer_unpack_char(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_attach_entity_packet *
unpack_S2C_play_attach_entity_packet(cmc_buffer *buff) {
  S2C_play_attach_entity_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->entity_id = cmc_buffer_unpack_int(buff);
    packet->vehicle_id = cmc_buffer_unpack_int(buff);
    packet->leash = cmc_buffer_unpack_bool(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_metadata_packet *
unpack_S2C_play_entity_metadata_packet(cmc_buffer *buff) {
  S2C_play_entity_metadata_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->meta_data = cmc_buffer_unpack_entity_metadata(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_effect_packet *
unpack_S2C_play_entity_effect_packet(cmc_buffer *buff) {
  S2C_play_entity_effect_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->effect_id = cmc_buffer_unpack_char(buff);
    packet->amplifier = cmc_buffer_unpack_char(buff);
    packet->duration = cmc_buffer_unpack_varint(buff);
    packet->hide_particles = cmc_buffer_unpack_bool(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_remove_entity_effect_packet *
unpack_S2C_play_remove_entity_effect_packet(cmc_buffer *buff) {
  S2C_play_remove_entity_effect_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->effect_id = cmc_buffer_unpack_char(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_set_experience_packet *
unpack_S2C_play_set_experience_packet(cmc_buffer *buff) {
  S2C_play_set_experience_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->experience_bar = cmc_buffer_unpack_float(buff);
    packet->level = cmc_buffer_unpack_varint(buff);
    packet->total_experience = cmc_buffer_unpack_varint(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_chunk_data_packet *
unpack_S2C_play_chunk_data_packet(cmc_buffer *buff) {
  S2C_play_chunk_data_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->chunk_x = cmc_buffer_unpack_int(buff);
    packet->chunk_z = cmc_buffer_unpack_int(buff);
    packet->ground_up_continuous = cmc_buffer_unpack_bool(buff);
    packet->primary_bitmask = cmc_buffer_unpack_ushort(buff);
    packet->chunk = cmc_buffer_unpack_byte_array(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_block_change_packet *
unpack_S2C_play_block_change_packet(cmc_buffer *buff) {
  S2C_play_block_change_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->location = cmc_buffer_unpack_position(buff);
    packet->block_id = cmc_buffer_unpack_varint(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_block_action_packet *
unpack_S2C_play_block_action_packet(cmc_buffer *buff) {
  S2C_play_block_action_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->location = cmc_buffer_unpack_position(buff);
    packet->block_data_1 = cmc_buffer_unpack_byte(buff);
    packet->block_data_2 = cmc_buffer_unpack_byte(buff);
    packet->block_type = cmc_buffer_unpack_varint(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_block_break_animation_packet *
unpack_S2C_play_block_break_animation_packet(cmc_buffer *buff) {
  S2C_play_block_break_animation_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->entity_id = cmc_buffer_unpack_varint(buff);
    packet->location = cmc_buffer_unpack_position(buff);
    packet->destroy_stage = cmc_buffer_unpack_char(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_effect_packet *unpack_S2C_play_effect_packet(cmc_buffer *buff) {
  S2C_play_effect_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->effect_id = cmc_buffer_unpack_int(buff);
    packet->location = cmc_buffer_unpack_position(buff);
    packet->data = cmc_buffer_unpack_int(buff);
    packet->d = cmc_buffer_unpack_bool(buff);
    packet->particle_id = cmc_buffer_unpack_int(buff);
    packet->long_distances = cmc_buffer_unpack_bool(buff);
    packet->x = cmc_buffer_unpack_float(buff);
    packet->y = cmc_buffer_unpack_float(buff);
    packet->z = cmc_buffer_unpack_float(buff);
    packet->x_offset = cmc_buffer_unpack_float(buff);
    packet->y_offset = cmc_buffer_unpack_float(buff);
    packet->z_offset = cmc_buffer_unpack_float(buff);
    packet->particle_data = cmc_buffer_unpack_float(buff);
    packet->particle_count = cmc_buffer_unpack_int(buff);
    packet->sable_relative_volume = cmc_buffer_unpack_int(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_sound_effect_packet *
unpack_S2C_play_sound_effect_packet(cmc_buffer *buff) {
  S2C_play_sound_effect_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->sound_name = cmc_buffer_unpack_string(buff);
    packet->x = cmc_buffer_unpack_int(buff);
    packet->y = cmc_buffer_unpack_int(buff);
    packet->z = cmc_buffer_unpack_int(buff);
    packet->volume = cmc_buffer_unpack_float(buff);
    packet->pitch = cmc_buffer_unpack_byte(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_change_game_state_packet *
unpack_S2C_play_change_game_state_packet(cmc_buffer *buff) {
  S2C_play_change_game_state_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->reason = cmc_buffer_unpack_byte(buff);
    packet->value = cmc_buffer_unpack_float(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_player_abilities_packet *
unpack_S2C_play_player_abilities_packet(cmc_buffer *buff) {
  S2C_play_player_abilities_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->flags = cmc_buffer_unpack_char(buff);
    packet->flying_speed = cmc_buffer_unpack_float(buff);
    packet->fov_modifier = cmc_buffer_unpack_float(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_plugin_message_packet *
unpack_S2C_play_plugin_message_packet(cmc_buffer *buff) {
  S2C_play_plugin_message_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->channel = cmc_buffer_unpack_string(buff);
    packet->data = cmc_buffer_unpack_byte_array(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_disconnect_packet *
unpack_S2C_play_disconnect_packet(cmc_buffer *buff) {
  S2C_play_disconnect_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->reason = cmc_buffer_unpack_string(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_change_difficulty_packet *
unpack_S2C_play_change_difficulty_packet(cmc_buffer *buff) {
  S2C_play_change_difficulty_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->difficulty = cmc_buffer_unpack_byte(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

C2S_play_keep_alive_packet *
unpack_C2S_play_keep_alive_packet(cmc_buffer *buff) {
  C2S_play_keep_alive_packet *packet = MALLOC(sizeof(*packet));
  switch (buff->protocol_version) {
  case 47: {
    packet->keep_alive_id = cmc_buffer_unpack_varint(buff);
    break;
  }
  default:
    ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);
  }
  UNPACK_ERR_HANDELER;
  return packet;
}

// CGSE: unpack_methods_c

// CGSS: free_methods_c
void cmc_free_C2S_handshake_handshake_packet(
    C2S_handshake_handshake_packet packet) {
  free_string(packet.server_addr);
}
void cmc_free_S2C_status_response_packet(S2C_status_response_packet packet) {
  free_string(packet.response);
}

void cmc_free_S2C_login_disconnect_packet(S2C_login_disconnect_packet packet) {
  free_string(packet.reason);
}
void cmc_free_S2C_login_encryption_request_packet(
    S2C_login_encryption_request_packet packet) {
  free_string(packet.server_id);
  free_byte_array(packet.public_key);
  free_byte_array(packet.verify_token);
}
void cmc_free_S2C_login_success_packet(S2C_login_success_packet packet) {
  free_string(packet.name);
  free_string(packet.uuid);
}

void cmc_free_C2S_login_start_packet(C2S_login_start_packet packet) {
  free_string(packet.name);
}
void cmc_free_C2S_login_encryption_response_packet(
    C2S_login_encryption_response_packet packet) {
  free_byte_array(packet.shared_secret);
  free_byte_array(packet.verify_token);
}

void cmc_free_S2C_play_join_game_packet(S2C_play_join_game_packet packet) {
  free_string(packet.level_type);
}
void cmc_free_S2C_play_chat_message_packet(
    S2C_play_chat_message_packet packet) {
  free_string(packet.message);
}

void cmc_free_S2C_play_entity_equipment_packet(
    S2C_play_entity_equipment_packet packet) {
  free_slot(packet.item);
}

void cmc_free_S2C_play_respawn_packet(S2C_play_respawn_packet packet) {
  free_string(packet.level_type);
}

void cmc_free_S2C_play_spawn_player_packet(
    S2C_play_spawn_player_packet packet) {
  free_entity_metadata(packet.meta_data);
}

void cmc_free_S2C_play_spawn_mob_packet(S2C_play_spawn_mob_packet packet) {
  free_entity_metadata(packet.meta_data);
}
void cmc_free_S2C_play_spawn_painting_packet(
    S2C_play_spawn_painting_packet packet) {
  free_string(packet.title);
}

void cmc_free_S2C_play_entity_metadata_packet(
    S2C_play_entity_metadata_packet packet) {
  free_entity_metadata(packet.meta_data);
}

void cmc_free_S2C_play_chunk_data_packet(S2C_play_chunk_data_packet packet) {
  free_byte_array(packet.chunk);
}

void cmc_free_S2C_play_sound_effect_packet(
    S2C_play_sound_effect_packet packet) {
  free_string(packet.sound_name);
}

void cmc_free_S2C_play_plugin_message_packet(
    S2C_play_plugin_message_packet packet) {
  free_byte_array(packet.data);
  free_string(packet.channel);
}
void cmc_free_S2C_play_disconnect_packet(S2C_play_disconnect_packet packet) {
  free_string(packet.reason);
}

// CGSE: free_methods_c