#include <cmc/packets.h>

#include <cmc/buff.h>
#include <cmc/conn.h>
#include <cmc/err.h>
#include <cmc/heap_utils.h>
#include <cmc/nbt.h>
#include <cmc/packet_types.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "err_macros.h"

#define UNPACK_ERR_HANDELER                                                    \
  ERR_CHECK(return packet;);                                                   \
  if (buff->position != buff->length)                                          \
    ERR(ERR_BUFFER_UNDERUN, return packet;);

cmc_packet_name_id cmc_packet_id_to_packet_name_id(int packet_id,
                                                   cmc_conn_state state,
                                                   packet_direction direction,
                                                   int protocol_version) {
#define COMBINE_VALUES(packet_id, state, direction, protocol_version)          \
  ((((uint64_t)(packet_id)&0xFF) << 56) | ((uint64_t)(state) << 48) |          \
   ((uint64_t)(direction) << 47) | ((uint64_t)(protocol_version) << 32))

  switch (COMBINE_VALUES(packet_id, state, direction, protocol_version)) {
    // CGSS: packet_id_to_packet_name_id
  case (COMBINE_VALUES(0x00, CMC_CONN_STATE_HANDSHAKE, CMC_DIRECTION_C2S, 765)):
    return CMC_C2S_HANDSHAKE_HANDSHAKE_NAME_ID;
  case (COMBINE_VALUES(0x00, CMC_CONN_STATE_HANDSHAKE, CMC_DIRECTION_C2S, 47)):
    return CMC_C2S_HANDSHAKE_HANDSHAKE_NAME_ID;
  case (COMBINE_VALUES(0x00, CMC_CONN_STATE_STATUS, CMC_DIRECTION_S2C, 765)):
    return CMC_S2C_STATUS_RESPONSE_NAME_ID;
  case (COMBINE_VALUES(0x00, CMC_CONN_STATE_STATUS, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_STATUS_RESPONSE_NAME_ID;
  case (COMBINE_VALUES(0x01, CMC_CONN_STATE_STATUS, CMC_DIRECTION_S2C, 765)):
    return CMC_S2C_STATUS_PONG_NAME_ID;
  case (COMBINE_VALUES(0x01, CMC_CONN_STATE_STATUS, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_STATUS_PONG_NAME_ID;
  case (COMBINE_VALUES(0x00, CMC_CONN_STATE_STATUS, CMC_DIRECTION_C2S, 765)):
    return CMC_C2S_STATUS_REQUEST_NAME_ID;
  case (COMBINE_VALUES(0x00, CMC_CONN_STATE_STATUS, CMC_DIRECTION_C2S, 47)):
    return CMC_C2S_STATUS_REQUEST_NAME_ID;
  case (COMBINE_VALUES(0x01, CMC_CONN_STATE_STATUS, CMC_DIRECTION_C2S, 765)):
    return CMC_C2S_STATUS_PING_NAME_ID;
  case (COMBINE_VALUES(0x01, CMC_CONN_STATE_STATUS, CMC_DIRECTION_C2S, 47)):
    return CMC_C2S_STATUS_PING_NAME_ID;
  case (COMBINE_VALUES(0x00, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_S2C, 765)):
    return CMC_S2C_LOGIN_DISCONNECT_NAME_ID;
  case (COMBINE_VALUES(0x00, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_LOGIN_DISCONNECT_NAME_ID;
  case (COMBINE_VALUES(0x01, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_S2C, 765)):
    return CMC_S2C_LOGIN_ENCRYPTION_REQUEST_NAME_ID;
  case (COMBINE_VALUES(0x01, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_LOGIN_ENCRYPTION_REQUEST_NAME_ID;
  case (COMBINE_VALUES(0x02, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_S2C, 765)):
    return CMC_S2C_LOGIN_SUCCESS_NAME_ID;
  case (COMBINE_VALUES(0x02, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_LOGIN_SUCCESS_NAME_ID;
  case (COMBINE_VALUES(0x03, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_S2C, 765)):
    return CMC_S2C_LOGIN_SET_COMPRESSION_NAME_ID;
  case (COMBINE_VALUES(0x03, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_LOGIN_SET_COMPRESSION_NAME_ID;
  case (COMBINE_VALUES(0x00, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_C2S, 765)):
    return CMC_C2S_LOGIN_START_NAME_ID;
  case (COMBINE_VALUES(0x00, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_C2S, 47)):
    return CMC_C2S_LOGIN_START_NAME_ID;
  case (COMBINE_VALUES(0x01, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_C2S, 765)):
    return CMC_C2S_LOGIN_ENCRYPTION_RESPONSE_NAME_ID;
  case (COMBINE_VALUES(0x01, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_C2S, 47)):
    return CMC_C2S_LOGIN_ENCRYPTION_RESPONSE_NAME_ID;
  case (COMBINE_VALUES(0x03, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_C2S, 765)):
    return CMC_C2S_LOGIN_ACKNOWLEDGED_NAME_ID;
  case (COMBINE_VALUES(0x00, CMC_CONN_STATE_CONFIG, CMC_DIRECTION_S2C, 765)):
    return CMC_S2C_CONFIG_PLUGIN_MESSAGE_NAME_ID;
  case (COMBINE_VALUES(0x01, CMC_CONN_STATE_CONFIG, CMC_DIRECTION_S2C, 765)):
    return CMC_S2C_CONFIG_DISCONNECT_NAME_ID;
  case (COMBINE_VALUES(0x02, CMC_CONN_STATE_CONFIG, CMC_DIRECTION_S2C, 765)):
    return CMC_S2C_CONFIG_FINISH_NAME_ID;
  case (COMBINE_VALUES(0x03, CMC_CONN_STATE_CONFIG, CMC_DIRECTION_S2C, 765)):
    return CMC_S2C_CONFIG_KEEP_ALIVE_NAME_ID;
  case (COMBINE_VALUES(0x04, CMC_CONN_STATE_CONFIG, CMC_DIRECTION_S2C, 765)):
    return CMC_S2C_CONFIG_PING_NAME_ID;
  case (COMBINE_VALUES(0x05, CMC_CONN_STATE_CONFIG, CMC_DIRECTION_S2C, 765)):
    return CMC_S2C_CONFIG_REGISTRY_DATA_NAME_ID;
  case (COMBINE_VALUES(0x06, CMC_CONN_STATE_CONFIG, CMC_DIRECTION_S2C, 765)):
    return CMC_S2C_CONFIG_REMOVE_RESOURCE_PACK_NAME_ID;
  case (COMBINE_VALUES(0x07, CMC_CONN_STATE_CONFIG, CMC_DIRECTION_S2C, 765)):
    return CMC_S2C_CONFIG_ADD_RESOURCE_PACK_NAME_ID;
  case (COMBINE_VALUES(0x15, CMC_CONN_STATE_PLAY, CMC_DIRECTION_C2S, 765)):
    return CMC_C2S_PLAY_KEEP_ALIVE_NAME_ID;
  case (COMBINE_VALUES(0x00, CMC_CONN_STATE_PLAY, CMC_DIRECTION_C2S, 47)):
    return CMC_C2S_PLAY_KEEP_ALIVE_NAME_ID;
  case (COMBINE_VALUES(0x24, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 765)):
    return CMC_S2C_PLAY_KEEP_ALIVE_NAME_ID;
  case (COMBINE_VALUES(0x00, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_KEEP_ALIVE_NAME_ID;
  case (COMBINE_VALUES(0x1B, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 765)):
    return CMC_S2C_PLAY_DISCONNECT_NAME_ID;
  case (COMBINE_VALUES(0x40, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_DISCONNECT_NAME_ID;
  case (COMBINE_VALUES(0x01, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_JOIN_GAME_NAME_ID;
  case (COMBINE_VALUES(0x02, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_CHAT_MESSAGE_NAME_ID;
  case (COMBINE_VALUES(0x03, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_TIME_UPDATE_NAME_ID;
  case (COMBINE_VALUES(0x04, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_ENTITY_EQUIPMENT_NAME_ID;
  case (COMBINE_VALUES(0x05, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_SPAWN_POSITION_NAME_ID;
  case (COMBINE_VALUES(0x06, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_UPDATE_HEALTH_NAME_ID;
  case (COMBINE_VALUES(0x07, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_RESPAWN_NAME_ID;
  case (COMBINE_VALUES(0x08, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_PLAYER_LOOK_AND_POSITION_NAME_ID;
  case (COMBINE_VALUES(0x09, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_HELD_ITEM_CHANGE_NAME_ID;
  case (COMBINE_VALUES(0x0A, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_USE_BED_NAME_ID;
  case (COMBINE_VALUES(0x0B, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_ANIMATION_NAME_ID;
  case (COMBINE_VALUES(0x0C, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_SPAWN_PLAYER_NAME_ID;
  case (COMBINE_VALUES(0x0D, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_COLLECT_ITEM_NAME_ID;
  case (COMBINE_VALUES(0x0F, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_SPAWN_MOB_NAME_ID;
  case (COMBINE_VALUES(0x10, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_SPAWN_PAINTING_NAME_ID;
  case (COMBINE_VALUES(0x11, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_SPAWN_EXPERIENCE_ORB_NAME_ID;
  case (COMBINE_VALUES(0x12, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_ENTITY_VELOCITY_NAME_ID;
  case (COMBINE_VALUES(0x14, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_ENTITY_NAME_ID;
  case (COMBINE_VALUES(0x15, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_ENTITY_RELATIVE_MOVE_NAME_ID;
  case (COMBINE_VALUES(0x16, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_ENTITY_LOOK_NAME_ID;
  case (COMBINE_VALUES(0x17, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_ENTITY_LOOK_AND_RELATIVE_MOVE_NAME_ID;
  case (COMBINE_VALUES(0x18, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_ENTITY_TELEPORT_NAME_ID;
  case (COMBINE_VALUES(0x19, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_ENTITY_HEAD_LOOK_NAME_ID;
  case (COMBINE_VALUES(0x1A, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_ENTITY_STATUS_NAME_ID;
  case (COMBINE_VALUES(0x1B, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_ATTACH_ENTITY_NAME_ID;
  case (COMBINE_VALUES(0x1C, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_ENTITY_METADATA_NAME_ID;
  case (COMBINE_VALUES(0x1D, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_ENTITY_EFFECT_NAME_ID;
  case (COMBINE_VALUES(0x1E, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_REMOVE_ENTITY_EFFECT_NAME_ID;
  case (COMBINE_VALUES(0x1F, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_SET_EXPERIENCE_NAME_ID;
  case (COMBINE_VALUES(0x20, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_ENTITY_PROPERTIES_NAME_ID;
  case (COMBINE_VALUES(0x21, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_CHUNK_DATA_NAME_ID;
  case (COMBINE_VALUES(0x22, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_MULTI_BLOCK_CHANGE_NAME_ID;
  case (COMBINE_VALUES(0x23, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_BLOCK_CHANGE_NAME_ID;
  case (COMBINE_VALUES(0x24, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_BLOCK_ACTION_NAME_ID;
  case (COMBINE_VALUES(0x25, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_BLOCK_BREAK_ANIMATION_NAME_ID;
  case (COMBINE_VALUES(0x26, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_MAP_CHUNK_BULK_NAME_ID;
  case (COMBINE_VALUES(0x27, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_EXPLOSION_NAME_ID;
  case (COMBINE_VALUES(0x28, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_EFFECT_NAME_ID;
  case (COMBINE_VALUES(0x29, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_SOUND_EFFECT_NAME_ID;
  case (COMBINE_VALUES(0x2B, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_CHANGE_GAME_STATE_NAME_ID;
  case (COMBINE_VALUES(0x39, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_PLAYER_ABILITIES_NAME_ID;
  case (COMBINE_VALUES(0x3F, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_PLUGIN_MESSAGE_NAME_ID;
  case (COMBINE_VALUES(0x41, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C, 47)):
    return CMC_S2C_PLAY_CHANGE_DIFFICULTY_NAME_ID;
    // CGSE: packet_id_to_packet_name_id
  default:
    return CMC_UNKOWN_NAME_ID;
  }
}

const char *cmc_packet_name_id_string(cmc_packet_name_id id) {
#define HELPER(pnid)                                                           \
  case pnid:                                                                   \
    return #pnid;
  switch (id) {
    HELPER(CMC_UNKOWN_NAME_ID);
    // CGSS: packet_name_id_string
    HELPER(CMC_C2S_HANDSHAKE_HANDSHAKE_NAME_ID);
    HELPER(CMC_S2C_STATUS_RESPONSE_NAME_ID);
    HELPER(CMC_S2C_STATUS_PONG_NAME_ID);
    HELPER(CMC_C2S_STATUS_REQUEST_NAME_ID);
    HELPER(CMC_C2S_STATUS_PING_NAME_ID);
    HELPER(CMC_S2C_LOGIN_DISCONNECT_NAME_ID);
    HELPER(CMC_S2C_LOGIN_ENCRYPTION_REQUEST_NAME_ID);
    HELPER(CMC_S2C_LOGIN_SUCCESS_NAME_ID);
    HELPER(CMC_S2C_LOGIN_SET_COMPRESSION_NAME_ID);
    HELPER(CMC_C2S_LOGIN_START_NAME_ID);
    HELPER(CMC_C2S_LOGIN_ENCRYPTION_RESPONSE_NAME_ID);
    HELPER(CMC_S2C_PLAY_KEEP_ALIVE_NAME_ID);
    HELPER(CMC_S2C_PLAY_JOIN_GAME_NAME_ID);
    HELPER(CMC_S2C_PLAY_CHAT_MESSAGE_NAME_ID);
    HELPER(CMC_S2C_PLAY_TIME_UPDATE_NAME_ID);
    HELPER(CMC_S2C_PLAY_ENTITY_EQUIPMENT_NAME_ID);
    HELPER(CMC_S2C_PLAY_SPAWN_POSITION_NAME_ID);
    HELPER(CMC_S2C_PLAY_UPDATE_HEALTH_NAME_ID);
    HELPER(CMC_S2C_PLAY_RESPAWN_NAME_ID);
    HELPER(CMC_S2C_PLAY_PLAYER_LOOK_AND_POSITION_NAME_ID);
    HELPER(CMC_S2C_PLAY_HELD_ITEM_CHANGE_NAME_ID);
    HELPER(CMC_S2C_PLAY_USE_BED_NAME_ID);
    HELPER(CMC_S2C_PLAY_ANIMATION_NAME_ID);
    HELPER(CMC_S2C_PLAY_SPAWN_PLAYER_NAME_ID);
    HELPER(CMC_S2C_PLAY_COLLECT_ITEM_NAME_ID);
    HELPER(CMC_S2C_PLAY_SPAWN_MOB_NAME_ID);
    HELPER(CMC_S2C_PLAY_SPAWN_PAINTING_NAME_ID);
    HELPER(CMC_S2C_PLAY_SPAWN_EXPERIENCE_ORB_NAME_ID);
    HELPER(CMC_S2C_PLAY_ENTITY_VELOCITY_NAME_ID);
    HELPER(CMC_S2C_PLAY_ENTITY_NAME_ID);
    HELPER(CMC_S2C_PLAY_ENTITY_RELATIVE_MOVE_NAME_ID);
    HELPER(CMC_S2C_PLAY_ENTITY_LOOK_NAME_ID);
    HELPER(CMC_S2C_PLAY_ENTITY_LOOK_AND_RELATIVE_MOVE_NAME_ID);
    HELPER(CMC_S2C_PLAY_ENTITY_TELEPORT_NAME_ID);
    HELPER(CMC_S2C_PLAY_ENTITY_HEAD_LOOK_NAME_ID);
    HELPER(CMC_S2C_PLAY_ENTITY_STATUS_NAME_ID);
    HELPER(CMC_S2C_PLAY_ATTACH_ENTITY_NAME_ID);
    HELPER(CMC_S2C_PLAY_ENTITY_METADATA_NAME_ID);
    HELPER(CMC_S2C_PLAY_ENTITY_EFFECT_NAME_ID);
    HELPER(CMC_S2C_PLAY_REMOVE_ENTITY_EFFECT_NAME_ID);
    HELPER(CMC_S2C_PLAY_SET_EXPERIENCE_NAME_ID);
    HELPER(CMC_S2C_PLAY_ENTITY_PROPERTIES_NAME_ID);
    HELPER(CMC_S2C_PLAY_CHUNK_DATA_NAME_ID);
    HELPER(CMC_S2C_PLAY_MULTI_BLOCK_CHANGE_NAME_ID);
    HELPER(CMC_S2C_PLAY_BLOCK_CHANGE_NAME_ID);
    HELPER(CMC_S2C_PLAY_BLOCK_ACTION_NAME_ID);
    HELPER(CMC_S2C_PLAY_BLOCK_BREAK_ANIMATION_NAME_ID);
    HELPER(CMC_S2C_PLAY_MAP_CHUNK_BULK_NAME_ID);
    HELPER(CMC_S2C_PLAY_EXPLOSION_NAME_ID);
    HELPER(CMC_S2C_PLAY_EFFECT_NAME_ID);
    HELPER(CMC_S2C_PLAY_SOUND_EFFECT_NAME_ID);
    HELPER(CMC_S2C_PLAY_CHANGE_GAME_STATE_NAME_ID);
    HELPER(CMC_S2C_PLAY_PLAYER_ABILITIES_NAME_ID);
    HELPER(CMC_S2C_PLAY_PLUGIN_MESSAGE_NAME_ID);
    HELPER(CMC_S2C_PLAY_DISCONNECT_NAME_ID);
    HELPER(CMC_S2C_PLAY_CHANGE_DIFFICULTY_NAME_ID);
    HELPER(CMC_C2S_PLAY_KEEP_ALIVE_NAME_ID);
    HELPER(CMC_C2S_LOGIN_ACKNOWLEDGED_NAME_ID);
    HELPER(CMC_S2C_CONFIG_PLUGIN_MESSAGE_NAME_ID);
    HELPER(CMC_S2C_CONFIG_DISCONNECT_NAME_ID);
    HELPER(CMC_S2C_CONFIG_FINISH_NAME_ID);
    HELPER(CMC_S2C_CONFIG_KEEP_ALIVE_NAME_ID);
    HELPER(CMC_S2C_CONFIG_PING_NAME_ID);
    HELPER(CMC_S2C_CONFIG_REGISTRY_DATA_NAME_ID);
    HELPER(CMC_S2C_CONFIG_REMOVE_RESOURCE_PACK_NAME_ID);
    HELPER(CMC_S2C_CONFIG_ADD_RESOURCE_PACK_NAME_ID);
    // CGSE: packet_name_id_string
  }
#undef HELPER

  return "CMC_INVALID_NAME_ID";
}

// CGSS: free_methods_c
void cmc_free_C2S_handshake_handshake_packet(
    C2S_handshake_handshake_packet *packet, cmc_err_extra *err) {
  cmc_string_free(packet->server_addr);
  (void)err;
}
void cmc_free_S2C_status_response_packet(S2C_status_response_packet *packet,
                                         cmc_err_extra *err) {
  cmc_string_free(packet->response);
  (void)err;
}
void cmc_free_S2C_status_pong_packet(S2C_status_pong_packet *packet,
                                     cmc_err_extra *err) {
  (void)packet;
  (void)err;
}

void cmc_free_C2S_status_ping_packet(C2S_status_ping_packet *packet,
                                     cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_login_disconnect_packet(S2C_login_disconnect_packet *packet,
                                          cmc_err_extra *err) {
  cmc_string_free(packet->reason);
  (void)err;
}
void cmc_free_S2C_login_encryption_request_packet(
    S2C_login_encryption_request_packet *packet, cmc_err_extra *err) {
  cmc_string_free(packet->server_id);
  cmc_buff_free(packet->public_key);
  cmc_buff_free(packet->verify_token);
  (void)err;
}
void cmc_free_S2C_login_success_packet(S2C_login_success_packet *packet,
                                       cmc_err_extra *err) {
  cmc_string_free(packet->uuid_str);
  cmc_string_free(packet->name);
  (void)err;
}
void cmc_free_S2C_login_set_compression_packet(
    S2C_login_set_compression_packet *packet, cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_C2S_login_start_packet(C2S_login_start_packet *packet,
                                     cmc_err_extra *err) {
  cmc_string_free(packet->name);
  (void)err;
}
void cmc_free_C2S_login_encryption_response_packet(
    C2S_login_encryption_response_packet *packet, cmc_err_extra *err) {
  cmc_buff_free(packet->shared_secret);
  cmc_buff_free(packet->verify_token);
  (void)err;
}
void cmc_free_S2C_play_keep_alive_packet(S2C_play_keep_alive_packet *packet,
                                         cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_join_game_packet(S2C_play_join_game_packet *packet,
                                        cmc_err_extra *err) {
  cmc_string_free(packet->level_type);
  (void)err;
}
void cmc_free_S2C_play_chat_message_packet(S2C_play_chat_message_packet *packet,
                                           cmc_err_extra *err) {
  cmc_string_free(packet->message);
  (void)err;
}
void cmc_free_S2C_play_time_update_packet(S2C_play_time_update_packet *packet,
                                          cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_entity_equipment_packet(
    S2C_play_entity_equipment_packet *packet, cmc_err_extra *err) {
  cmc_slot_free(packet->item, err);
  (void)err;
}
void cmc_free_S2C_play_spawn_position_packet(
    S2C_play_spawn_position_packet *packet, cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_update_health_packet(
    S2C_play_update_health_packet *packet, cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_respawn_packet(S2C_play_respawn_packet *packet,
                                      cmc_err_extra *err) {
  cmc_string_free(packet->level_type);
  (void)err;
}
void cmc_free_S2C_play_player_look_and_position_packet(
    S2C_play_player_look_and_position_packet *packet, cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_held_item_change_packet(
    S2C_play_held_item_change_packet *packet, cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_use_bed_packet(S2C_play_use_bed_packet *packet,
                                      cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_animation_packet(S2C_play_animation_packet *packet,
                                        cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_spawn_player_packet(S2C_play_spawn_player_packet *packet,
                                           cmc_err_extra *err) {
  cmc_entity_metadata_free(packet->meta_data, err);
  (void)err;
}
void cmc_free_S2C_play_collect_item_packet(S2C_play_collect_item_packet *packet,
                                           cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_spawn_mob_packet(S2C_play_spawn_mob_packet *packet,
                                        cmc_err_extra *err) {
  cmc_entity_metadata_free(packet->meta_data, err);
  (void)err;
}
void cmc_free_S2C_play_spawn_painting_packet(
    S2C_play_spawn_painting_packet *packet, cmc_err_extra *err) {
  cmc_string_free(packet->title);
  (void)err;
}
void cmc_free_S2C_play_spawn_experience_orb_packet(
    S2C_play_spawn_experience_orb_packet *packet, cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_entity_velocity_packet(
    S2C_play_entity_velocity_packet *packet, cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_entity_packet(S2C_play_entity_packet *packet,
                                     cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_entity_relative_move_packet(
    S2C_play_entity_relative_move_packet *packet, cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_entity_look_packet(S2C_play_entity_look_packet *packet,
                                          cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_entity_look_and_relative_move_packet(
    S2C_play_entity_look_and_relative_move_packet *packet, cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_entity_teleport_packet(
    S2C_play_entity_teleport_packet *packet, cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_entity_head_look_packet(
    S2C_play_entity_head_look_packet *packet, cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_entity_status_packet(
    S2C_play_entity_status_packet *packet, cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_attach_entity_packet(
    S2C_play_attach_entity_packet *packet, cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_entity_metadata_packet(
    S2C_play_entity_metadata_packet *packet, cmc_err_extra *err) {
  cmc_entity_metadata_free(packet->meta_data, err);
  (void)err;
}
void cmc_free_S2C_play_entity_effect_packet(
    S2C_play_entity_effect_packet *packet, cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_remove_entity_effect_packet(
    S2C_play_remove_entity_effect_packet *packet, cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_set_experience_packet(
    S2C_play_set_experience_packet *packet, cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_entity_properties_packet(
    S2C_play_entity_properties_packet *packet, cmc_err_extra *err) {
  for (int i = 0; i < packet->properties.len; ++i) {
    S2C_play_entity_properties_properties *p_properties =
        &((S2C_play_entity_properties_properties *)packet->properties.data)[i];
    cmc_string_free(p_properties->key);
    for (int j = 0; j < p_properties->modifiers.len; ++j) {
      S2C_play_entity_properties_modifiers *p_modifiers =
          &((S2C_play_entity_properties_modifiers *)
                p_properties->modifiers.data)[j];
    }
    free(p_properties->modifiers.data);
    p_properties->modifiers.len = 0;
  }
  free(packet->properties.data);
  packet->properties.len = 0;
  (void)err;
}
void cmc_free_S2C_play_chunk_data_packet(S2C_play_chunk_data_packet *packet,
                                         cmc_err_extra *err) {
  cmc_buff_free(packet->chunk);
  (void)err;
}
void cmc_free_S2C_play_multi_block_change_packet(
    S2C_play_multi_block_change_packet *packet, cmc_err_extra *err) {
  for (int i = 0; i < packet->records.len; ++i) {
    S2C_play_multi_block_change_records *p_records =
        &((S2C_play_multi_block_change_records *)packet->records.data)[i];
  }
  free(packet->records.data);
  packet->records.len = 0;
  (void)err;
}
void cmc_free_S2C_play_block_change_packet(S2C_play_block_change_packet *packet,
                                           cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_block_action_packet(S2C_play_block_action_packet *packet,
                                           cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_block_break_animation_packet(
    S2C_play_block_break_animation_packet *packet, cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_map_chunk_bulk_packet(
    S2C_play_map_chunk_bulk_packet *packet, cmc_err_extra *err) {
  for (int i = 0; i < packet->chunk_columns.len; ++i) {
    S2C_play_map_chunk_bulk_chunk_columns *p_chunk_columns = &(
        (S2C_play_map_chunk_bulk_chunk_columns *)packet->chunk_columns.data)[i];
  }
  free(packet->chunk_columns.data);
  packet->chunk_columns.len = 0;
  cmc_buff_free(packet->chunk);
  (void)err;
}
void cmc_free_S2C_play_explosion_packet(S2C_play_explosion_packet *packet,
                                        cmc_err_extra *err) {
  for (int i = 0; i < packet->records.len; ++i) {
    S2C_play_explosion_records *p_records =
        &((S2C_play_explosion_records *)packet->records.data)[i];
  }
  free(packet->records.data);
  packet->records.len = 0;
  (void)err;
}
void cmc_free_S2C_play_effect_packet(S2C_play_effect_packet *packet,
                                     cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_sound_effect_packet(S2C_play_sound_effect_packet *packet,
                                           cmc_err_extra *err) {
  cmc_string_free(packet->sound_name);
  (void)err;
}
void cmc_free_S2C_play_change_game_state_packet(
    S2C_play_change_game_state_packet *packet, cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_player_abilities_packet(
    S2C_play_player_abilities_packet *packet, cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_play_plugin_message_packet(
    S2C_play_plugin_message_packet *packet, cmc_err_extra *err) {
  cmc_string_free(packet->channel);
  cmc_buff_free(packet->data);
  (void)err;
}
void cmc_free_S2C_play_disconnect_packet(S2C_play_disconnect_packet *packet,
                                         cmc_err_extra *err) {
  cmc_string_free(packet->reason);
  cmc_nbt_free(packet->reason_nbt, err);
  (void)err;
}
void cmc_free_S2C_play_change_difficulty_packet(
    S2C_play_change_difficulty_packet *packet, cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_C2S_play_keep_alive_packet(C2S_play_keep_alive_packet *packet,
                                         cmc_err_extra *err) {
  (void)packet;
  (void)err;
}

void cmc_free_S2C_config_plugin_message_packet(
    S2C_config_plugin_message_packet *packet, cmc_err_extra *err) {
  cmc_string_free(packet->channel);
  cmc_buff_free(packet->data);
  (void)err;
}
void cmc_free_S2C_config_disconnect_packet(S2C_config_disconnect_packet *packet,
                                           cmc_err_extra *err) {
  cmc_string_free(packet->reason);
  (void)err;
}

void cmc_free_S2C_config_keep_alive_packet(S2C_config_keep_alive_packet *packet,
                                           cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_config_ping_packet(S2C_config_ping_packet *packet,
                                     cmc_err_extra *err) {
  (void)packet;
  (void)err;
}
void cmc_free_S2C_config_registry_data_packet(
    S2C_config_registry_data_packet *packet, cmc_err_extra *err) {
  cmc_nbt_free(packet->registry_codec, err);
  (void)err;
}

// CGSE: free_methods_c

// CGSS: send_methods_c

cmc_err cmc_send_C2S_handshake_handshake_packet(
    cmc_conn *conn, C2S_handshake_handshake_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x00);
    cmc_buff_pack_varint(buff, packet->protocole_version);
    cmc_buff_pack_string(buff, packet->server_addr);
    cmc_buff_pack_ushort(buff, packet->server_port);
    cmc_buff_pack_varint(buff, packet->next_state);
    break;
  }

  case 765: {
    cmc_buff_pack_varint(buff, 0x00);
    cmc_buff_pack_varint(buff, packet->protocole_version);
    cmc_buff_pack_string(buff, packet->server_addr);
    cmc_buff_pack_ushort(buff, packet->server_port);
    cmc_buff_pack_varint(buff, packet->next_state);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err
cmc_send_S2C_status_response_packet(cmc_conn *conn,
                                    S2C_status_response_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x00);
    cmc_buff_pack_string(buff, packet->response);
    break;
  }

  case 765: {
    cmc_buff_pack_varint(buff, 0x00);
    cmc_buff_pack_string(buff, packet->response);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_status_pong_packet(cmc_conn *conn,
                                        S2C_status_pong_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x01);
    cmc_buff_pack_long(buff, packet->payload);
    break;
  }

  case 765: {
    cmc_buff_pack_varint(buff, 0x01);
    cmc_buff_pack_long(buff, packet->payload);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_C2S_status_request_packet(cmc_conn *conn) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x00);

    break;
  }

  case 765: {
    cmc_buff_pack_varint(buff, 0x00);

    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_C2S_status_ping_packet(cmc_conn *conn,
                                        C2S_status_ping_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x01);
    cmc_buff_pack_long(buff, packet->payload);
    break;
  }

  case 765: {
    cmc_buff_pack_varint(buff, 0x01);
    cmc_buff_pack_long(buff, packet->payload);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err
cmc_send_S2C_login_disconnect_packet(cmc_conn *conn,
                                     S2C_login_disconnect_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x00);
    cmc_buff_pack_string(buff, packet->reason);
    break;
  }

  case 765: {
    cmc_buff_pack_varint(buff, 0x00);
    cmc_buff_pack_string(buff, packet->reason);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_login_encryption_request_packet(
    cmc_conn *conn, S2C_login_encryption_request_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x01);
    cmc_buff_pack_string(buff, packet->server_id);
    cmc_buff_pack_buff(buff, packet->public_key);
    cmc_buff_pack_buff(buff, packet->verify_token);
    break;
  }

  case 765: {
    cmc_buff_pack_varint(buff, 0x01);
    cmc_buff_pack_string(buff, packet->server_id);
    cmc_buff_pack_buff(buff, packet->public_key);
    cmc_buff_pack_buff(buff, packet->verify_token);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_login_success_packet(cmc_conn *conn,
                                          S2C_login_success_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x02);
    cmc_buff_pack_string(buff, packet->uuid_str);
    cmc_buff_pack_string(buff, packet->name);
    break;
  }

  case 765: {
    cmc_buff_pack_varint(buff, 0x02);
    cmc_buff_pack_uuid(buff, packet->uuid);
    cmc_buff_pack_string(buff, packet->name);
    cmc_buff_pack_varint(buff, packet->properties_count);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_login_set_compression_packet(
    cmc_conn *conn, S2C_login_set_compression_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x03);
    cmc_buff_pack_varint(buff, packet->threshold);
    break;
  }

  case 765: {
    cmc_buff_pack_varint(buff, 0x03);
    cmc_buff_pack_varint(buff, packet->threshold);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_C2S_login_start_packet(cmc_conn *conn,
                                        C2S_login_start_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x00);
    cmc_buff_pack_string(buff, packet->name);
    break;
  }

  case 765: {
    cmc_buff_pack_varint(buff, 0x00);
    cmc_buff_pack_string(buff, packet->name);
    cmc_buff_pack_uuid(buff, packet->uuid);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_C2S_login_encryption_response_packet(
    cmc_conn *conn, C2S_login_encryption_response_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x01);
    cmc_buff_pack_buff(buff, packet->shared_secret);
    cmc_buff_pack_buff(buff, packet->verify_token);
    break;
  }

  case 765: {
    cmc_buff_pack_varint(buff, 0x01);
    cmc_buff_pack_buff(buff, packet->shared_secret);
    cmc_buff_pack_buff(buff, packet->verify_token);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err
cmc_send_S2C_play_keep_alive_packet(cmc_conn *conn,
                                    S2C_play_keep_alive_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x00);
    cmc_buff_pack_varint(buff, packet->keep_alive_id);
    break;
  }

  case 765: {
    cmc_buff_pack_varint(buff, 0x24);
    cmc_buff_pack_long(buff, packet->keep_alive_id_long);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_join_game_packet(cmc_conn *conn,
                                           S2C_play_join_game_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x01);
    cmc_buff_pack_int(buff, packet->entity_id);
    cmc_buff_pack_byte(buff, packet->gamemode);
    cmc_buff_pack_char(buff, packet->dimension);
    cmc_buff_pack_byte(buff, packet->difficulty);
    cmc_buff_pack_byte(buff, packet->max_players);
    cmc_buff_pack_string(buff, packet->level_type);
    cmc_buff_pack_bool(buff, packet->reduced_debug_info);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err
cmc_send_S2C_play_chat_message_packet(cmc_conn *conn,
                                      S2C_play_chat_message_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x02);
    cmc_buff_pack_string(buff, packet->message);
    cmc_buff_pack_char(buff, packet->position);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err
cmc_send_S2C_play_time_update_packet(cmc_conn *conn,
                                     S2C_play_time_update_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x03);
    cmc_buff_pack_long(buff, packet->world_age);
    cmc_buff_pack_long(buff, packet->time_of_day);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_entity_equipment_packet(
    cmc_conn *conn, S2C_play_entity_equipment_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x04);
    cmc_buff_pack_varint(buff, packet->entity_id);
    cmc_buff_pack_short(buff, packet->slot);
    cmc_buff_pack_slot(buff, packet->item);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_spawn_position_packet(
    cmc_conn *conn, S2C_play_spawn_position_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x05);
    cmc_buff_pack_position(buff, packet->location);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err
cmc_send_S2C_play_update_health_packet(cmc_conn *conn,
                                       S2C_play_update_health_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x06);
    cmc_buff_pack_float(buff, packet->health);
    cmc_buff_pack_varint(buff, packet->food);
    cmc_buff_pack_float(buff, packet->food_saturation);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_respawn_packet(cmc_conn *conn,
                                         S2C_play_respawn_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x07);
    cmc_buff_pack_int(buff, packet->dimesion);
    cmc_buff_pack_byte(buff, packet->difficulty);
    cmc_buff_pack_byte(buff, packet->gamemode);
    cmc_buff_pack_string(buff, packet->level_type);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_player_look_and_position_packet(
    cmc_conn *conn, S2C_play_player_look_and_position_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x08);
    cmc_buff_pack_double(buff, packet->x);
    cmc_buff_pack_double(buff, packet->y);
    cmc_buff_pack_double(buff, packet->z);
    cmc_buff_pack_float(buff, packet->yaw);
    cmc_buff_pack_float(buff, packet->pitch);
    cmc_buff_pack_byte(buff, packet->flags);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_held_item_change_packet(
    cmc_conn *conn, S2C_play_held_item_change_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x09);
    cmc_buff_pack_char(buff, packet->slot);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_use_bed_packet(cmc_conn *conn,
                                         S2C_play_use_bed_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x0A);
    cmc_buff_pack_varint(buff, packet->entity_id);
    cmc_buff_pack_position(buff, packet->location);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_animation_packet(cmc_conn *conn,
                                           S2C_play_animation_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x0B);
    cmc_buff_pack_varint(buff, packet->entity_id);
    cmc_buff_pack_byte(buff, packet->animation);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err
cmc_send_S2C_play_spawn_player_packet(cmc_conn *conn,
                                      S2C_play_spawn_player_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x0C);
    cmc_buff_pack_varint(buff, packet->entity_id);
    cmc_buff_pack_uuid(buff, packet->uuid);
    cmc_buff_pack_int(buff, packet->x);
    cmc_buff_pack_int(buff, packet->y);
    cmc_buff_pack_int(buff, packet->z);
    cmc_buff_pack_byte(buff, packet->yaw);
    cmc_buff_pack_byte(buff, packet->pitch);
    cmc_buff_pack_short(buff, packet->current_item);
    cmc_buff_pack_entity_metadata(buff, packet->meta_data);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err
cmc_send_S2C_play_collect_item_packet(cmc_conn *conn,
                                      S2C_play_collect_item_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x0D);
    cmc_buff_pack_varint(buff, packet->collected_entity_id);
    cmc_buff_pack_varint(buff, packet->collector_entity_id);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_spawn_mob_packet(cmc_conn *conn,
                                           S2C_play_spawn_mob_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x0F);
    cmc_buff_pack_varint(buff, packet->entity_id);
    cmc_buff_pack_byte(buff, packet->type);
    cmc_buff_pack_int(buff, packet->x);
    cmc_buff_pack_int(buff, packet->y);
    cmc_buff_pack_int(buff, packet->z);
    cmc_buff_pack_byte(buff, packet->yaw);
    cmc_buff_pack_byte(buff, packet->pitch);
    cmc_buff_pack_byte(buff, packet->head_pitch);
    cmc_buff_pack_short(buff, packet->x_vel);
    cmc_buff_pack_short(buff, packet->y_vel);
    cmc_buff_pack_short(buff, packet->z_vel);
    cmc_buff_pack_entity_metadata(buff, packet->meta_data);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_spawn_painting_packet(
    cmc_conn *conn, S2C_play_spawn_painting_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x10);
    cmc_buff_pack_varint(buff, packet->entity_id);
    cmc_buff_pack_string(buff, packet->title);
    cmc_buff_pack_position(buff, packet->location);
    cmc_buff_pack_byte(buff, packet->direction);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_spawn_experience_orb_packet(
    cmc_conn *conn, S2C_play_spawn_experience_orb_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x11);
    cmc_buff_pack_varint(buff, packet->entity_id);
    cmc_buff_pack_int(buff, packet->x);
    cmc_buff_pack_int(buff, packet->y);
    cmc_buff_pack_int(buff, packet->z);
    cmc_buff_pack_short(buff, packet->count);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_entity_velocity_packet(
    cmc_conn *conn, S2C_play_entity_velocity_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x12);
    cmc_buff_pack_varint(buff, packet->entity_id);
    cmc_buff_pack_short(buff, packet->x_vel);
    cmc_buff_pack_short(buff, packet->y_vel);
    cmc_buff_pack_short(buff, packet->z_vel);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_entity_packet(cmc_conn *conn,
                                        S2C_play_entity_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x14);
    cmc_buff_pack_varint(buff, packet->entity_id);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_entity_relative_move_packet(
    cmc_conn *conn, S2C_play_entity_relative_move_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x15);
    cmc_buff_pack_varint(buff, packet->entity_id);
    cmc_buff_pack_char(buff, packet->delta_x);
    cmc_buff_pack_char(buff, packet->delta_y);
    cmc_buff_pack_char(buff, packet->delta_z);
    cmc_buff_pack_bool(buff, packet->on_ground);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err
cmc_send_S2C_play_entity_look_packet(cmc_conn *conn,
                                     S2C_play_entity_look_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x16);
    cmc_buff_pack_varint(buff, packet->entity_id);
    cmc_buff_pack_byte(buff, packet->yaw);
    cmc_buff_pack_byte(buff, packet->pitch);
    cmc_buff_pack_bool(buff, packet->on_ground);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_entity_look_and_relative_move_packet(
    cmc_conn *conn, S2C_play_entity_look_and_relative_move_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x17);
    cmc_buff_pack_varint(buff, packet->entity_id);
    cmc_buff_pack_char(buff, packet->delta_x);
    cmc_buff_pack_char(buff, packet->delta_y);
    cmc_buff_pack_char(buff, packet->delta_z);
    cmc_buff_pack_byte(buff, packet->yaw);
    cmc_buff_pack_byte(buff, packet->pitch);
    cmc_buff_pack_bool(buff, packet->on_ground);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_entity_teleport_packet(
    cmc_conn *conn, S2C_play_entity_teleport_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x18);
    cmc_buff_pack_varint(buff, packet->entity_id);
    cmc_buff_pack_int(buff, packet->x);
    cmc_buff_pack_int(buff, packet->y);
    cmc_buff_pack_int(buff, packet->z);
    cmc_buff_pack_byte(buff, packet->yaw);
    cmc_buff_pack_byte(buff, packet->pitch);
    cmc_buff_pack_bool(buff, packet->on_ground);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_entity_head_look_packet(
    cmc_conn *conn, S2C_play_entity_head_look_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x19);
    cmc_buff_pack_varint(buff, packet->entity_id);
    cmc_buff_pack_byte(buff, packet->head_yaw);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err
cmc_send_S2C_play_entity_status_packet(cmc_conn *conn,
                                       S2C_play_entity_status_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x1A);
    cmc_buff_pack_int(buff, packet->entity_id);
    cmc_buff_pack_char(buff, packet->entity_status);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err
cmc_send_S2C_play_attach_entity_packet(cmc_conn *conn,
                                       S2C_play_attach_entity_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x1B);
    cmc_buff_pack_int(buff, packet->entity_id);
    cmc_buff_pack_int(buff, packet->vehicle_id);
    cmc_buff_pack_bool(buff, packet->leash);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_entity_metadata_packet(
    cmc_conn *conn, S2C_play_entity_metadata_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x1C);
    cmc_buff_pack_varint(buff, packet->entity_id);
    cmc_buff_pack_entity_metadata(buff, packet->meta_data);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err
cmc_send_S2C_play_entity_effect_packet(cmc_conn *conn,
                                       S2C_play_entity_effect_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x1D);
    cmc_buff_pack_varint(buff, packet->entity_id);
    cmc_buff_pack_char(buff, packet->effect_id);
    cmc_buff_pack_char(buff, packet->amplifier);
    cmc_buff_pack_varint(buff, packet->duration);
    cmc_buff_pack_bool(buff, packet->hide_particles);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_remove_entity_effect_packet(
    cmc_conn *conn, S2C_play_remove_entity_effect_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x1E);
    cmc_buff_pack_varint(buff, packet->entity_id);
    cmc_buff_pack_char(buff, packet->effect_id);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_set_experience_packet(
    cmc_conn *conn, S2C_play_set_experience_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x1F);
    cmc_buff_pack_float(buff, packet->experience_bar);
    cmc_buff_pack_varint(buff, packet->level);
    cmc_buff_pack_varint(buff, packet->total_experience);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_entity_properties_packet(
    cmc_conn *conn, S2C_play_entity_properties_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x20);
    cmc_buff_pack_varint(buff, packet->entity_id);
    cmc_buff_pack_int(buff, packet->properties_count);
    for (int i = 0; i < packet->properties.len; ++i) {
      S2C_play_entity_properties_properties *p_properties = &(
          (S2C_play_entity_properties_properties *)packet->properties.data)[i];
      cmc_buff_pack_string(buff, p_properties->key);
      cmc_buff_pack_double(buff, p_properties->value);
      cmc_buff_pack_varint(buff, p_properties->num_of_modifiers);
      for (int j = 0; j < p_properties->modifiers.len; ++j) {
        S2C_play_entity_properties_modifiers *p_modifiers =
            &((S2C_play_entity_properties_modifiers *)
                  p_properties->modifiers.data)[j];
        cmc_buff_pack_double(buff, p_modifiers->amount);
        cmc_buff_pack_char(buff, p_modifiers->operation);
      }
    }

    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err
cmc_send_S2C_play_chunk_data_packet(cmc_conn *conn,
                                    S2C_play_chunk_data_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x21);
    cmc_buff_pack_int(buff, packet->chunk_x);
    cmc_buff_pack_int(buff, packet->chunk_z);
    cmc_buff_pack_bool(buff, packet->ground_up_continuous);
    cmc_buff_pack_ushort(buff, packet->primary_bitmask);
    cmc_buff_pack_buff(buff, packet->chunk);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_multi_block_change_packet(
    cmc_conn *conn, S2C_play_multi_block_change_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x22);
    cmc_buff_pack_int(buff, packet->chunk_x);
    cmc_buff_pack_int(buff, packet->chunk_z);
    cmc_buff_pack_varint(buff, packet->record_count);
    for (int i = 0; i < packet->records.len; ++i) {
      S2C_play_multi_block_change_records *p_records =
          &((S2C_play_multi_block_change_records *)packet->records.data)[i];
      cmc_buff_pack_byte(buff, p_records->horizontal_position);
      cmc_buff_pack_byte(buff, p_records->vertical_position);
      cmc_buff_pack_varint(buff, p_records->block_id);
    }

    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err
cmc_send_S2C_play_block_change_packet(cmc_conn *conn,
                                      S2C_play_block_change_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x23);
    cmc_buff_pack_position(buff, packet->location);
    cmc_buff_pack_varint(buff, packet->block_id);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err
cmc_send_S2C_play_block_action_packet(cmc_conn *conn,
                                      S2C_play_block_action_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x24);
    cmc_buff_pack_position(buff, packet->location);
    cmc_buff_pack_byte(buff, packet->block_data_1);
    cmc_buff_pack_byte(buff, packet->block_data_2);
    cmc_buff_pack_varint(buff, packet->block_type);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_block_break_animation_packet(
    cmc_conn *conn, S2C_play_block_break_animation_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x25);
    cmc_buff_pack_varint(buff, packet->entity_id);
    cmc_buff_pack_position(buff, packet->location);
    cmc_buff_pack_char(buff, packet->destroy_stage);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_map_chunk_bulk_packet(
    cmc_conn *conn, S2C_play_map_chunk_bulk_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x26);
    cmc_buff_pack_bool(buff, packet->sky_light_sent);
    cmc_buff_pack_varint(buff, packet->chunk_column_count);
    for (int i = 0; i < packet->chunk_columns.len; ++i) {
      S2C_play_map_chunk_bulk_chunk_columns *p_chunk_columns =
          &((S2C_play_map_chunk_bulk_chunk_columns *)
                packet->chunk_columns.data)[i];
      cmc_buff_pack_int(buff, p_chunk_columns->chunk_x);
      cmc_buff_pack_int(buff, p_chunk_columns->chunk_z);
      cmc_buff_pack_ushort(buff, p_chunk_columns->bit_mask);
    }
    cmc_buff_pack_buff(buff, packet->chunk);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_explosion_packet(cmc_conn *conn,
                                           S2C_play_explosion_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x27);
    cmc_buff_pack_float(buff, packet->x);
    cmc_buff_pack_float(buff, packet->y);
    cmc_buff_pack_float(buff, packet->z);
    cmc_buff_pack_float(buff, packet->radius);
    cmc_buff_pack_int(buff, packet->record_count);
    for (int i = 0; i < packet->records.len; ++i) {
      S2C_play_explosion_records *p_records =
          &((S2C_play_explosion_records *)packet->records.data)[i];
      cmc_buff_pack_char(buff, p_records->x_offset);
      cmc_buff_pack_char(buff, p_records->y_offset);
      cmc_buff_pack_char(buff, p_records->z_offset);
    }
    cmc_buff_pack_float(buff, packet->x_player_vel);
    cmc_buff_pack_float(buff, packet->y_player_vel);
    cmc_buff_pack_float(buff, packet->z_player_vel);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_effect_packet(cmc_conn *conn,
                                        S2C_play_effect_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x28);
    cmc_buff_pack_int(buff, packet->effect_id);
    cmc_buff_pack_position(buff, packet->location);
    cmc_buff_pack_int(buff, packet->data);
    cmc_buff_pack_bool(buff, packet->d);
    cmc_buff_pack_int(buff, packet->particle_id);
    cmc_buff_pack_bool(buff, packet->long_distances);
    cmc_buff_pack_float(buff, packet->x);
    cmc_buff_pack_float(buff, packet->y);
    cmc_buff_pack_float(buff, packet->z);
    cmc_buff_pack_float(buff, packet->x_offset);
    cmc_buff_pack_float(buff, packet->y_offset);
    cmc_buff_pack_float(buff, packet->z_offset);
    cmc_buff_pack_float(buff, packet->particle_data);
    cmc_buff_pack_int(buff, packet->particle_count);
    cmc_buff_pack_int(buff, packet->sable_relative_volume);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err
cmc_send_S2C_play_sound_effect_packet(cmc_conn *conn,
                                      S2C_play_sound_effect_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x29);
    cmc_buff_pack_string(buff, packet->sound_name);
    cmc_buff_pack_int(buff, packet->x);
    cmc_buff_pack_int(buff, packet->y);
    cmc_buff_pack_int(buff, packet->z);
    cmc_buff_pack_float(buff, packet->volume);
    cmc_buff_pack_byte(buff, packet->pitch);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_change_game_state_packet(
    cmc_conn *conn, S2C_play_change_game_state_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x2B);
    cmc_buff_pack_byte(buff, packet->reason);
    cmc_buff_pack_float(buff, packet->value);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_player_abilities_packet(
    cmc_conn *conn, S2C_play_player_abilities_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x39);
    cmc_buff_pack_char(buff, packet->flags);
    cmc_buff_pack_float(buff, packet->flying_speed);
    cmc_buff_pack_float(buff, packet->fov_modifier);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_plugin_message_packet(
    cmc_conn *conn, S2C_play_plugin_message_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x3F);
    cmc_buff_pack_string(buff, packet->channel);
    cmc_buff_pack_buff(buff, packet->data);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err
cmc_send_S2C_play_disconnect_packet(cmc_conn *conn,
                                    S2C_play_disconnect_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x40);
    cmc_buff_pack_string(buff, packet->reason);
    break;
  }

  case 765: {
    cmc_buff_pack_varint(buff, 0x1B);
    cmc_buff_pack_nbt(buff, packet->reason_nbt);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_play_change_difficulty_packet(
    cmc_conn *conn, S2C_play_change_difficulty_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x41);
    cmc_buff_pack_byte(buff, packet->difficulty);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err
cmc_send_C2S_play_keep_alive_packet(cmc_conn *conn,
                                    C2S_play_keep_alive_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 47: {
    cmc_buff_pack_varint(buff, 0x00);
    cmc_buff_pack_varint(buff, packet->keep_alive_id);
    break;
  }

  case 765: {
    cmc_buff_pack_varint(buff, 0x15);
    cmc_buff_pack_long(buff, packet->keep_alive_id_long);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_C2S_login_acknowledged_packet(cmc_conn *conn) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 765: {
    cmc_buff_pack_varint(buff, 0x03);

    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_config_plugin_message_packet(
    cmc_conn *conn, S2C_config_plugin_message_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 765: {
    cmc_buff_pack_varint(buff, 0x00);
    cmc_buff_pack_string(buff, packet->channel);
    cmc_buff_pack_buff(buff, packet->data);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err
cmc_send_S2C_config_disconnect_packet(cmc_conn *conn,
                                      S2C_config_disconnect_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 765: {
    cmc_buff_pack_varint(buff, 0x01);
    cmc_buff_pack_string(buff, packet->reason);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_config_finish_packet(cmc_conn *conn) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 765: {
    cmc_buff_pack_varint(buff, 0x02);

    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err
cmc_send_S2C_config_keep_alive_packet(cmc_conn *conn,
                                      S2C_config_keep_alive_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 765: {
    cmc_buff_pack_varint(buff, 0x03);
    cmc_buff_pack_long(buff, packet->keep_alive_id);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_config_ping_packet(cmc_conn *conn,
                                        S2C_config_ping_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 765: {
    cmc_buff_pack_varint(buff, 0x04);
    cmc_buff_pack_int(buff, packet->id);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_config_registry_data_packet(
    cmc_conn *conn, S2C_config_registry_data_packet *packet) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 765: {
    cmc_buff_pack_varint(buff, 0x05);
    cmc_buff_pack_nbt(buff, packet->registry_codec);
    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_config_remove_resource_pack_packet(cmc_conn *conn) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 765: {
    cmc_buff_pack_varint(buff, 0x06);

    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

cmc_err cmc_send_S2C_config_add_resource_pack_packet(cmc_conn *conn) {
  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  switch (conn->protocol_version) {

  case 765: {
    cmc_buff_pack_varint(buff, 0x07);

    break;
  }

  default:
    cmc_buff_free(buff);
    CMC_ERRRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION);
  }
  cmc_conn_send_packet(conn, buff);
  cmc_buff_free(buff);
  return CMC_ERR_NO;
}

// CGSE: send_methods_c

// CGSS: unpack_methods_c

C2S_handshake_handshake_packet
unpack_C2S_handshake_handshake_packet(cmc_buff *buff) {
  C2S_handshake_handshake_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.protocole_version = cmc_buff_unpack_varint(buff);
    packet.server_addr = cmc_buff_unpack_string(buff);
    packet.server_port = cmc_buff_unpack_ushort(buff);
    packet.next_state = cmc_buff_unpack_varint(buff);
    break;
  }

  case 765: {
    packet.protocole_version = cmc_buff_unpack_varint(buff);
    packet.server_addr = cmc_buff_unpack_string(buff);
    packet.server_port = cmc_buff_unpack_ushort(buff);
    packet.next_state = cmc_buff_unpack_varint(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_C2S_handshake_handshake_packet(&packet, &buff->err);
  return (C2S_handshake_handshake_packet){};
}

S2C_status_response_packet unpack_S2C_status_response_packet(cmc_buff *buff) {
  S2C_status_response_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.response = cmc_buff_unpack_string(buff);
    break;
  }

  case 765: {
    packet.response = cmc_buff_unpack_string(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_status_response_packet(&packet, &buff->err);
  return (S2C_status_response_packet){};
}

S2C_status_pong_packet unpack_S2C_status_pong_packet(cmc_buff *buff) {
  S2C_status_pong_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.payload = cmc_buff_unpack_long(buff);
    break;
  }

  case 765: {
    packet.payload = cmc_buff_unpack_long(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_status_pong_packet(&packet, &buff->err);
  return (S2C_status_pong_packet){};
}

C2S_status_ping_packet unpack_C2S_status_ping_packet(cmc_buff *buff) {
  C2S_status_ping_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.payload = cmc_buff_unpack_long(buff);
    break;
  }

  case 765: {
    packet.payload = cmc_buff_unpack_long(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_C2S_status_ping_packet(&packet, &buff->err);
  return (C2S_status_ping_packet){};
}

S2C_login_disconnect_packet unpack_S2C_login_disconnect_packet(cmc_buff *buff) {
  S2C_login_disconnect_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.reason = cmc_buff_unpack_string(buff);
    break;
  }

  case 765: {
    packet.reason = cmc_buff_unpack_string(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_login_disconnect_packet(&packet, &buff->err);
  return (S2C_login_disconnect_packet){};
}

S2C_login_encryption_request_packet
unpack_S2C_login_encryption_request_packet(cmc_buff *buff) {
  S2C_login_encryption_request_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.server_id = cmc_buff_unpack_string(buff);
    packet.public_key = cmc_buff_unpack_buff(buff);
    packet.verify_token = cmc_buff_unpack_buff(buff);
    break;
  }

  case 765: {
    packet.server_id = cmc_buff_unpack_string(buff);
    packet.public_key = cmc_buff_unpack_buff(buff);
    packet.verify_token = cmc_buff_unpack_buff(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_login_encryption_request_packet(&packet, &buff->err);
  return (S2C_login_encryption_request_packet){};
}

S2C_login_success_packet unpack_S2C_login_success_packet(cmc_buff *buff) {
  S2C_login_success_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.uuid_str = cmc_buff_unpack_string(buff);
    packet.name = cmc_buff_unpack_string(buff);
    break;
  }

  case 765: {
    packet.uuid = cmc_buff_unpack_uuid(buff);
    packet.name = cmc_buff_unpack_string(buff);
    packet.properties_count = cmc_buff_unpack_varint(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_login_success_packet(&packet, &buff->err);
  return (S2C_login_success_packet){};
}

S2C_login_set_compression_packet
unpack_S2C_login_set_compression_packet(cmc_buff *buff) {
  S2C_login_set_compression_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.threshold = cmc_buff_unpack_varint(buff);
    break;
  }

  case 765: {
    packet.threshold = cmc_buff_unpack_varint(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_login_set_compression_packet(&packet, &buff->err);
  return (S2C_login_set_compression_packet){};
}

C2S_login_start_packet unpack_C2S_login_start_packet(cmc_buff *buff) {
  C2S_login_start_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.name = cmc_buff_unpack_string(buff);
    break;
  }

  case 765: {
    packet.name = cmc_buff_unpack_string(buff);
    packet.uuid = cmc_buff_unpack_uuid(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_C2S_login_start_packet(&packet, &buff->err);
  return (C2S_login_start_packet){};
}

C2S_login_encryption_response_packet
unpack_C2S_login_encryption_response_packet(cmc_buff *buff) {
  C2S_login_encryption_response_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.shared_secret = cmc_buff_unpack_buff(buff);
    packet.verify_token = cmc_buff_unpack_buff(buff);
    break;
  }

  case 765: {
    packet.shared_secret = cmc_buff_unpack_buff(buff);
    packet.verify_token = cmc_buff_unpack_buff(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_C2S_login_encryption_response_packet(&packet, &buff->err);
  return (C2S_login_encryption_response_packet){};
}

S2C_play_keep_alive_packet unpack_S2C_play_keep_alive_packet(cmc_buff *buff) {
  S2C_play_keep_alive_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.keep_alive_id = cmc_buff_unpack_varint(buff);
    break;
  }

  case 765: {
    packet.keep_alive_id_long = cmc_buff_unpack_long(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_keep_alive_packet(&packet, &buff->err);
  return (S2C_play_keep_alive_packet){};
}

S2C_play_join_game_packet unpack_S2C_play_join_game_packet(cmc_buff *buff) {
  S2C_play_join_game_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.entity_id = cmc_buff_unpack_int(buff);
    packet.gamemode = cmc_buff_unpack_byte(buff);
    packet.dimension = cmc_buff_unpack_char(buff);
    packet.difficulty = cmc_buff_unpack_byte(buff);
    packet.max_players = cmc_buff_unpack_byte(buff);
    packet.level_type = cmc_buff_unpack_string(buff);
    packet.reduced_debug_info = cmc_buff_unpack_bool(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_join_game_packet(&packet, &buff->err);
  return (S2C_play_join_game_packet){};
}

S2C_play_chat_message_packet
unpack_S2C_play_chat_message_packet(cmc_buff *buff) {
  S2C_play_chat_message_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.message = cmc_buff_unpack_string(buff);
    packet.position = cmc_buff_unpack_char(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_chat_message_packet(&packet, &buff->err);
  return (S2C_play_chat_message_packet){};
}

S2C_play_time_update_packet unpack_S2C_play_time_update_packet(cmc_buff *buff) {
  S2C_play_time_update_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.world_age = cmc_buff_unpack_long(buff);
    packet.time_of_day = cmc_buff_unpack_long(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_time_update_packet(&packet, &buff->err);
  return (S2C_play_time_update_packet){};
}

S2C_play_entity_equipment_packet
unpack_S2C_play_entity_equipment_packet(cmc_buff *buff) {
  S2C_play_entity_equipment_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.entity_id = cmc_buff_unpack_varint(buff);
    packet.slot = cmc_buff_unpack_short(buff);
    packet.item = cmc_buff_unpack_slot(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_entity_equipment_packet(&packet, &buff->err);
  return (S2C_play_entity_equipment_packet){};
}

S2C_play_spawn_position_packet
unpack_S2C_play_spawn_position_packet(cmc_buff *buff) {
  S2C_play_spawn_position_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.location = cmc_buff_unpack_position(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_spawn_position_packet(&packet, &buff->err);
  return (S2C_play_spawn_position_packet){};
}

S2C_play_update_health_packet
unpack_S2C_play_update_health_packet(cmc_buff *buff) {
  S2C_play_update_health_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.health = cmc_buff_unpack_float(buff);
    packet.food = cmc_buff_unpack_varint(buff);
    packet.food_saturation = cmc_buff_unpack_float(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_update_health_packet(&packet, &buff->err);
  return (S2C_play_update_health_packet){};
}

S2C_play_respawn_packet unpack_S2C_play_respawn_packet(cmc_buff *buff) {
  S2C_play_respawn_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.dimesion = cmc_buff_unpack_int(buff);
    packet.difficulty = cmc_buff_unpack_byte(buff);
    packet.gamemode = cmc_buff_unpack_byte(buff);
    packet.level_type = cmc_buff_unpack_string(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_respawn_packet(&packet, &buff->err);
  return (S2C_play_respawn_packet){};
}

S2C_play_player_look_and_position_packet
unpack_S2C_play_player_look_and_position_packet(cmc_buff *buff) {
  S2C_play_player_look_and_position_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.x = cmc_buff_unpack_double(buff);
    packet.y = cmc_buff_unpack_double(buff);
    packet.z = cmc_buff_unpack_double(buff);
    packet.yaw = cmc_buff_unpack_float(buff);
    packet.pitch = cmc_buff_unpack_float(buff);
    packet.flags = cmc_buff_unpack_byte(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_player_look_and_position_packet(&packet, &buff->err);
  return (S2C_play_player_look_and_position_packet){};
}

S2C_play_held_item_change_packet
unpack_S2C_play_held_item_change_packet(cmc_buff *buff) {
  S2C_play_held_item_change_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.slot = cmc_buff_unpack_char(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_held_item_change_packet(&packet, &buff->err);
  return (S2C_play_held_item_change_packet){};
}

S2C_play_use_bed_packet unpack_S2C_play_use_bed_packet(cmc_buff *buff) {
  S2C_play_use_bed_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.entity_id = cmc_buff_unpack_varint(buff);
    packet.location = cmc_buff_unpack_position(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_use_bed_packet(&packet, &buff->err);
  return (S2C_play_use_bed_packet){};
}

S2C_play_animation_packet unpack_S2C_play_animation_packet(cmc_buff *buff) {
  S2C_play_animation_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.entity_id = cmc_buff_unpack_varint(buff);
    packet.animation = cmc_buff_unpack_byte(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_animation_packet(&packet, &buff->err);
  return (S2C_play_animation_packet){};
}

S2C_play_spawn_player_packet
unpack_S2C_play_spawn_player_packet(cmc_buff *buff) {
  S2C_play_spawn_player_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.entity_id = cmc_buff_unpack_varint(buff);
    packet.uuid = cmc_buff_unpack_uuid(buff);
    packet.x = cmc_buff_unpack_int(buff);
    packet.y = cmc_buff_unpack_int(buff);
    packet.z = cmc_buff_unpack_int(buff);
    packet.yaw = cmc_buff_unpack_byte(buff);
    packet.pitch = cmc_buff_unpack_byte(buff);
    packet.current_item = cmc_buff_unpack_short(buff);
    packet.meta_data = cmc_buff_unpack_entity_metadata(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_spawn_player_packet(&packet, &buff->err);
  return (S2C_play_spawn_player_packet){};
}

S2C_play_collect_item_packet
unpack_S2C_play_collect_item_packet(cmc_buff *buff) {
  S2C_play_collect_item_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.collected_entity_id = cmc_buff_unpack_varint(buff);
    packet.collector_entity_id = cmc_buff_unpack_varint(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_collect_item_packet(&packet, &buff->err);
  return (S2C_play_collect_item_packet){};
}

S2C_play_spawn_mob_packet unpack_S2C_play_spawn_mob_packet(cmc_buff *buff) {
  S2C_play_spawn_mob_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.entity_id = cmc_buff_unpack_varint(buff);
    packet.type = cmc_buff_unpack_byte(buff);
    packet.x = cmc_buff_unpack_int(buff);
    packet.y = cmc_buff_unpack_int(buff);
    packet.z = cmc_buff_unpack_int(buff);
    packet.yaw = cmc_buff_unpack_byte(buff);
    packet.pitch = cmc_buff_unpack_byte(buff);
    packet.head_pitch = cmc_buff_unpack_byte(buff);
    packet.x_vel = cmc_buff_unpack_short(buff);
    packet.y_vel = cmc_buff_unpack_short(buff);
    packet.z_vel = cmc_buff_unpack_short(buff);
    packet.meta_data = cmc_buff_unpack_entity_metadata(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_spawn_mob_packet(&packet, &buff->err);
  return (S2C_play_spawn_mob_packet){};
}

S2C_play_spawn_painting_packet
unpack_S2C_play_spawn_painting_packet(cmc_buff *buff) {
  S2C_play_spawn_painting_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.entity_id = cmc_buff_unpack_varint(buff);
    packet.title = cmc_buff_unpack_string(buff);
    packet.location = cmc_buff_unpack_position(buff);
    packet.direction = cmc_buff_unpack_byte(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_spawn_painting_packet(&packet, &buff->err);
  return (S2C_play_spawn_painting_packet){};
}

S2C_play_spawn_experience_orb_packet
unpack_S2C_play_spawn_experience_orb_packet(cmc_buff *buff) {
  S2C_play_spawn_experience_orb_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.entity_id = cmc_buff_unpack_varint(buff);
    packet.x = cmc_buff_unpack_int(buff);
    packet.y = cmc_buff_unpack_int(buff);
    packet.z = cmc_buff_unpack_int(buff);
    packet.count = cmc_buff_unpack_short(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_spawn_experience_orb_packet(&packet, &buff->err);
  return (S2C_play_spawn_experience_orb_packet){};
}

S2C_play_entity_velocity_packet
unpack_S2C_play_entity_velocity_packet(cmc_buff *buff) {
  S2C_play_entity_velocity_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.entity_id = cmc_buff_unpack_varint(buff);
    packet.x_vel = cmc_buff_unpack_short(buff);
    packet.y_vel = cmc_buff_unpack_short(buff);
    packet.z_vel = cmc_buff_unpack_short(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_entity_velocity_packet(&packet, &buff->err);
  return (S2C_play_entity_velocity_packet){};
}

S2C_play_entity_packet unpack_S2C_play_entity_packet(cmc_buff *buff) {
  S2C_play_entity_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.entity_id = cmc_buff_unpack_varint(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_entity_packet(&packet, &buff->err);
  return (S2C_play_entity_packet){};
}

S2C_play_entity_relative_move_packet
unpack_S2C_play_entity_relative_move_packet(cmc_buff *buff) {
  S2C_play_entity_relative_move_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.entity_id = cmc_buff_unpack_varint(buff);
    packet.delta_x = cmc_buff_unpack_char(buff);
    packet.delta_y = cmc_buff_unpack_char(buff);
    packet.delta_z = cmc_buff_unpack_char(buff);
    packet.on_ground = cmc_buff_unpack_bool(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_entity_relative_move_packet(&packet, &buff->err);
  return (S2C_play_entity_relative_move_packet){};
}

S2C_play_entity_look_packet unpack_S2C_play_entity_look_packet(cmc_buff *buff) {
  S2C_play_entity_look_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.entity_id = cmc_buff_unpack_varint(buff);
    packet.yaw = cmc_buff_unpack_byte(buff);
    packet.pitch = cmc_buff_unpack_byte(buff);
    packet.on_ground = cmc_buff_unpack_bool(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_entity_look_packet(&packet, &buff->err);
  return (S2C_play_entity_look_packet){};
}

S2C_play_entity_look_and_relative_move_packet
unpack_S2C_play_entity_look_and_relative_move_packet(cmc_buff *buff) {
  S2C_play_entity_look_and_relative_move_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.entity_id = cmc_buff_unpack_varint(buff);
    packet.delta_x = cmc_buff_unpack_char(buff);
    packet.delta_y = cmc_buff_unpack_char(buff);
    packet.delta_z = cmc_buff_unpack_char(buff);
    packet.yaw = cmc_buff_unpack_byte(buff);
    packet.pitch = cmc_buff_unpack_byte(buff);
    packet.on_ground = cmc_buff_unpack_bool(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_entity_look_and_relative_move_packet(&packet, &buff->err);
  return (S2C_play_entity_look_and_relative_move_packet){};
}

S2C_play_entity_teleport_packet
unpack_S2C_play_entity_teleport_packet(cmc_buff *buff) {
  S2C_play_entity_teleport_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.entity_id = cmc_buff_unpack_varint(buff);
    packet.x = cmc_buff_unpack_int(buff);
    packet.y = cmc_buff_unpack_int(buff);
    packet.z = cmc_buff_unpack_int(buff);
    packet.yaw = cmc_buff_unpack_byte(buff);
    packet.pitch = cmc_buff_unpack_byte(buff);
    packet.on_ground = cmc_buff_unpack_bool(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_entity_teleport_packet(&packet, &buff->err);
  return (S2C_play_entity_teleport_packet){};
}

S2C_play_entity_head_look_packet
unpack_S2C_play_entity_head_look_packet(cmc_buff *buff) {
  S2C_play_entity_head_look_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.entity_id = cmc_buff_unpack_varint(buff);
    packet.head_yaw = cmc_buff_unpack_byte(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_entity_head_look_packet(&packet, &buff->err);
  return (S2C_play_entity_head_look_packet){};
}

S2C_play_entity_status_packet
unpack_S2C_play_entity_status_packet(cmc_buff *buff) {
  S2C_play_entity_status_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.entity_id = cmc_buff_unpack_int(buff);
    packet.entity_status = cmc_buff_unpack_char(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_entity_status_packet(&packet, &buff->err);
  return (S2C_play_entity_status_packet){};
}

S2C_play_attach_entity_packet
unpack_S2C_play_attach_entity_packet(cmc_buff *buff) {
  S2C_play_attach_entity_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.entity_id = cmc_buff_unpack_int(buff);
    packet.vehicle_id = cmc_buff_unpack_int(buff);
    packet.leash = cmc_buff_unpack_bool(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_attach_entity_packet(&packet, &buff->err);
  return (S2C_play_attach_entity_packet){};
}

S2C_play_entity_metadata_packet
unpack_S2C_play_entity_metadata_packet(cmc_buff *buff) {
  S2C_play_entity_metadata_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.entity_id = cmc_buff_unpack_varint(buff);
    packet.meta_data = cmc_buff_unpack_entity_metadata(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_entity_metadata_packet(&packet, &buff->err);
  return (S2C_play_entity_metadata_packet){};
}

S2C_play_entity_effect_packet
unpack_S2C_play_entity_effect_packet(cmc_buff *buff) {
  S2C_play_entity_effect_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.entity_id = cmc_buff_unpack_varint(buff);
    packet.effect_id = cmc_buff_unpack_char(buff);
    packet.amplifier = cmc_buff_unpack_char(buff);
    packet.duration = cmc_buff_unpack_varint(buff);
    packet.hide_particles = cmc_buff_unpack_bool(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_entity_effect_packet(&packet, &buff->err);
  return (S2C_play_entity_effect_packet){};
}

S2C_play_remove_entity_effect_packet
unpack_S2C_play_remove_entity_effect_packet(cmc_buff *buff) {
  S2C_play_remove_entity_effect_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.entity_id = cmc_buff_unpack_varint(buff);
    packet.effect_id = cmc_buff_unpack_char(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_remove_entity_effect_packet(&packet, &buff->err);
  return (S2C_play_remove_entity_effect_packet){};
}

S2C_play_set_experience_packet
unpack_S2C_play_set_experience_packet(cmc_buff *buff) {
  S2C_play_set_experience_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.experience_bar = cmc_buff_unpack_float(buff);
    packet.level = cmc_buff_unpack_varint(buff);
    packet.total_experience = cmc_buff_unpack_varint(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_set_experience_packet(&packet, &buff->err);
  return (S2C_play_set_experience_packet){};
}

S2C_play_entity_properties_packet
unpack_S2C_play_entity_properties_packet(cmc_buff *buff) {
  S2C_play_entity_properties_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.entity_id = cmc_buff_unpack_varint(buff);
    packet.properties_count = cmc_buff_unpack_int(buff);
    packet.properties.len = packet.properties_count;
    packet.properties.data = CMC_ERRB_ABLE(
        cmc_malloc(packet.properties.len *
                       sizeof(S2C_play_entity_properties_properties),
                   &buff->err),
        goto err;);
    for (int i = 0; i < packet.properties.len; ++i) {
      S2C_play_entity_properties_properties *p_properties =
          &((S2C_play_entity_properties_properties *)packet.properties.data)[i];
      p_properties->key = cmc_buff_unpack_string(buff);
      p_properties->value = cmc_buff_unpack_double(buff);
      p_properties->num_of_modifiers = cmc_buff_unpack_varint(buff);
      p_properties->modifiers.len = p_properties->num_of_modifiers;
      p_properties->modifiers.data = CMC_ERRB_ABLE(
          cmc_malloc(p_properties->modifiers.len *
                         sizeof(S2C_play_entity_properties_modifiers),
                     &buff->err),
          goto err;);
      for (int j = 0; i < p_properties->modifiers.len; ++j) {
        S2C_play_entity_properties_modifiers *p_modifiers =
            &((S2C_play_entity_properties_modifiers *)
                  p_properties->modifiers.data)[j];
        p_modifiers->amount = cmc_buff_unpack_double(buff);
        p_modifiers->operation = cmc_buff_unpack_char(buff);
      }
    }

    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_entity_properties_packet(&packet, &buff->err);
  return (S2C_play_entity_properties_packet){};
}

S2C_play_chunk_data_packet unpack_S2C_play_chunk_data_packet(cmc_buff *buff) {
  S2C_play_chunk_data_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.chunk_x = cmc_buff_unpack_int(buff);
    packet.chunk_z = cmc_buff_unpack_int(buff);
    packet.ground_up_continuous = cmc_buff_unpack_bool(buff);
    packet.primary_bitmask = cmc_buff_unpack_ushort(buff);
    packet.chunk = cmc_buff_unpack_buff(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_chunk_data_packet(&packet, &buff->err);
  return (S2C_play_chunk_data_packet){};
}

S2C_play_multi_block_change_packet
unpack_S2C_play_multi_block_change_packet(cmc_buff *buff) {
  S2C_play_multi_block_change_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.chunk_x = cmc_buff_unpack_int(buff);
    packet.chunk_z = cmc_buff_unpack_int(buff);
    packet.record_count = cmc_buff_unpack_varint(buff);
    packet.records.len = packet.record_count;
    packet.records.data = CMC_ERRB_ABLE(
        cmc_malloc(packet.records.len *
                       sizeof(S2C_play_multi_block_change_records),
                   &buff->err),
        goto err;);
    for (int i = 0; i < packet.records.len; ++i) {
      S2C_play_multi_block_change_records *p_records =
          &((S2C_play_multi_block_change_records *)packet.records.data)[i];
      p_records->horizontal_position = cmc_buff_unpack_byte(buff);
      p_records->vertical_position = cmc_buff_unpack_byte(buff);
      p_records->block_id = cmc_buff_unpack_varint(buff);
    }

    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_multi_block_change_packet(&packet, &buff->err);
  return (S2C_play_multi_block_change_packet){};
}

S2C_play_block_change_packet
unpack_S2C_play_block_change_packet(cmc_buff *buff) {
  S2C_play_block_change_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.location = cmc_buff_unpack_position(buff);
    packet.block_id = cmc_buff_unpack_varint(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_block_change_packet(&packet, &buff->err);
  return (S2C_play_block_change_packet){};
}

S2C_play_block_action_packet
unpack_S2C_play_block_action_packet(cmc_buff *buff) {
  S2C_play_block_action_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.location = cmc_buff_unpack_position(buff);
    packet.block_data_1 = cmc_buff_unpack_byte(buff);
    packet.block_data_2 = cmc_buff_unpack_byte(buff);
    packet.block_type = cmc_buff_unpack_varint(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_block_action_packet(&packet, &buff->err);
  return (S2C_play_block_action_packet){};
}

S2C_play_block_break_animation_packet
unpack_S2C_play_block_break_animation_packet(cmc_buff *buff) {
  S2C_play_block_break_animation_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.entity_id = cmc_buff_unpack_varint(buff);
    packet.location = cmc_buff_unpack_position(buff);
    packet.destroy_stage = cmc_buff_unpack_char(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_block_break_animation_packet(&packet, &buff->err);
  return (S2C_play_block_break_animation_packet){};
}

S2C_play_map_chunk_bulk_packet
unpack_S2C_play_map_chunk_bulk_packet(cmc_buff *buff) {
  S2C_play_map_chunk_bulk_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.sky_light_sent = cmc_buff_unpack_bool(buff);
    packet.chunk_column_count = cmc_buff_unpack_varint(buff);
    packet.chunk_columns.len = packet.chunk_column_count;
    packet.chunk_columns.data = CMC_ERRB_ABLE(
        cmc_malloc(packet.chunk_columns.len *
                       sizeof(S2C_play_map_chunk_bulk_chunk_columns),
                   &buff->err),
        goto err;);
    for (int i = 0; i < packet.chunk_columns.len; ++i) {
      S2C_play_map_chunk_bulk_chunk_columns *p_chunk_columns =
          &((S2C_play_map_chunk_bulk_chunk_columns *)
                packet.chunk_columns.data)[i];
      p_chunk_columns->chunk_x = cmc_buff_unpack_int(buff);
      p_chunk_columns->chunk_z = cmc_buff_unpack_int(buff);
      p_chunk_columns->bit_mask = cmc_buff_unpack_ushort(buff);
    }
    packet.chunk = cmc_buff_unpack_buff(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_map_chunk_bulk_packet(&packet, &buff->err);
  return (S2C_play_map_chunk_bulk_packet){};
}

S2C_play_explosion_packet unpack_S2C_play_explosion_packet(cmc_buff *buff) {
  S2C_play_explosion_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.x = cmc_buff_unpack_float(buff);
    packet.y = cmc_buff_unpack_float(buff);
    packet.z = cmc_buff_unpack_float(buff);
    packet.radius = cmc_buff_unpack_float(buff);
    packet.record_count = cmc_buff_unpack_int(buff);
    packet.records.len = packet.record_count;
    packet.records.data = CMC_ERRB_ABLE(
        cmc_malloc(packet.records.len * sizeof(S2C_play_explosion_records),
                   &buff->err),
        goto err;);
    for (int i = 0; i < packet.records.len; ++i) {
      S2C_play_explosion_records *p_records =
          &((S2C_play_explosion_records *)packet.records.data)[i];
      p_records->x_offset = cmc_buff_unpack_char(buff);
      p_records->y_offset = cmc_buff_unpack_char(buff);
      p_records->z_offset = cmc_buff_unpack_char(buff);
    }
    packet.x_player_vel = cmc_buff_unpack_float(buff);
    packet.y_player_vel = cmc_buff_unpack_float(buff);
    packet.z_player_vel = cmc_buff_unpack_float(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_explosion_packet(&packet, &buff->err);
  return (S2C_play_explosion_packet){};
}

S2C_play_effect_packet unpack_S2C_play_effect_packet(cmc_buff *buff) {
  S2C_play_effect_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.effect_id = cmc_buff_unpack_int(buff);
    packet.location = cmc_buff_unpack_position(buff);
    packet.data = cmc_buff_unpack_int(buff);
    packet.d = cmc_buff_unpack_bool(buff);
    packet.particle_id = cmc_buff_unpack_int(buff);
    packet.long_distances = cmc_buff_unpack_bool(buff);
    packet.x = cmc_buff_unpack_float(buff);
    packet.y = cmc_buff_unpack_float(buff);
    packet.z = cmc_buff_unpack_float(buff);
    packet.x_offset = cmc_buff_unpack_float(buff);
    packet.y_offset = cmc_buff_unpack_float(buff);
    packet.z_offset = cmc_buff_unpack_float(buff);
    packet.particle_data = cmc_buff_unpack_float(buff);
    packet.particle_count = cmc_buff_unpack_int(buff);
    packet.sable_relative_volume = cmc_buff_unpack_int(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_effect_packet(&packet, &buff->err);
  return (S2C_play_effect_packet){};
}

S2C_play_sound_effect_packet
unpack_S2C_play_sound_effect_packet(cmc_buff *buff) {
  S2C_play_sound_effect_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.sound_name = cmc_buff_unpack_string(buff);
    packet.x = cmc_buff_unpack_int(buff);
    packet.y = cmc_buff_unpack_int(buff);
    packet.z = cmc_buff_unpack_int(buff);
    packet.volume = cmc_buff_unpack_float(buff);
    packet.pitch = cmc_buff_unpack_byte(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_sound_effect_packet(&packet, &buff->err);
  return (S2C_play_sound_effect_packet){};
}

S2C_play_change_game_state_packet
unpack_S2C_play_change_game_state_packet(cmc_buff *buff) {
  S2C_play_change_game_state_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.reason = cmc_buff_unpack_byte(buff);
    packet.value = cmc_buff_unpack_float(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_change_game_state_packet(&packet, &buff->err);
  return (S2C_play_change_game_state_packet){};
}

S2C_play_player_abilities_packet
unpack_S2C_play_player_abilities_packet(cmc_buff *buff) {
  S2C_play_player_abilities_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.flags = cmc_buff_unpack_char(buff);
    packet.flying_speed = cmc_buff_unpack_float(buff);
    packet.fov_modifier = cmc_buff_unpack_float(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_player_abilities_packet(&packet, &buff->err);
  return (S2C_play_player_abilities_packet){};
}

S2C_play_plugin_message_packet
unpack_S2C_play_plugin_message_packet(cmc_buff *buff) {
  S2C_play_plugin_message_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.channel = cmc_buff_unpack_string(buff);
    packet.data = cmc_buff_unpack_buff(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_plugin_message_packet(&packet, &buff->err);
  return (S2C_play_plugin_message_packet){};
}

S2C_play_disconnect_packet unpack_S2C_play_disconnect_packet(cmc_buff *buff) {
  S2C_play_disconnect_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.reason = cmc_buff_unpack_string(buff);
    break;
  }

  case 765: {
    packet.reason_nbt = cmc_buff_unpack_nbt(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_disconnect_packet(&packet, &buff->err);
  return (S2C_play_disconnect_packet){};
}

S2C_play_change_difficulty_packet
unpack_S2C_play_change_difficulty_packet(cmc_buff *buff) {
  S2C_play_change_difficulty_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.difficulty = cmc_buff_unpack_byte(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_play_change_difficulty_packet(&packet, &buff->err);
  return (S2C_play_change_difficulty_packet){};
}

C2S_play_keep_alive_packet unpack_C2S_play_keep_alive_packet(cmc_buff *buff) {
  C2S_play_keep_alive_packet packet = {};
  switch (buff->protocol_version) {

  case 47: {
    packet.keep_alive_id = cmc_buff_unpack_varint(buff);
    break;
  }

  case 765: {
    packet.keep_alive_id_long = cmc_buff_unpack_long(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_C2S_play_keep_alive_packet(&packet, &buff->err);
  return (C2S_play_keep_alive_packet){};
}

S2C_config_plugin_message_packet
unpack_S2C_config_plugin_message_packet(cmc_buff *buff) {
  S2C_config_plugin_message_packet packet = {};
  switch (buff->protocol_version) {

  case 765: {
    packet.channel = cmc_buff_unpack_string(buff);
    packet.data = cmc_buff_unpack_buff(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_config_plugin_message_packet(&packet, &buff->err);
  return (S2C_config_plugin_message_packet){};
}

S2C_config_disconnect_packet
unpack_S2C_config_disconnect_packet(cmc_buff *buff) {
  S2C_config_disconnect_packet packet = {};
  switch (buff->protocol_version) {

  case 765: {
    packet.reason = cmc_buff_unpack_string(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_config_disconnect_packet(&packet, &buff->err);
  return (S2C_config_disconnect_packet){};
}

S2C_config_keep_alive_packet
unpack_S2C_config_keep_alive_packet(cmc_buff *buff) {
  S2C_config_keep_alive_packet packet = {};
  switch (buff->protocol_version) {

  case 765: {
    packet.keep_alive_id = cmc_buff_unpack_long(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_config_keep_alive_packet(&packet, &buff->err);
  return (S2C_config_keep_alive_packet){};
}

S2C_config_ping_packet unpack_S2C_config_ping_packet(cmc_buff *buff) {
  S2C_config_ping_packet packet = {};
  switch (buff->protocol_version) {

  case 765: {
    packet.id = cmc_buff_unpack_int(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_config_ping_packet(&packet, &buff->err);
  return (S2C_config_ping_packet){};
}

S2C_config_registry_data_packet
unpack_S2C_config_registry_data_packet(cmc_buff *buff) {
  S2C_config_registry_data_packet packet = {};
  switch (buff->protocol_version) {

  case 765: {
    packet.registry_codec = cmc_buff_unpack_nbt(buff);
    break;
  }

  default:
    CMC_ERRB(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION, return packet;);
  }
  CMC_ERRB_ABLE(, goto err);
  if (buff->position != buff->length)
    CMC_ERRB(CMC_ERR_BUFF_UNDERUN, goto err;);
  return packet;
err:
  cmc_free_S2C_config_registry_data_packet(&packet, &buff->err);
  return (S2C_config_registry_data_packet){};
}

// CGSE: unpack_methods_c
