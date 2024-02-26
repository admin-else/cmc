#pragma once

#include "buffer.h"
#include "conn.h"
#include "packet_types.h"
#include <jansson.h>
#include <stdbool.h>

char *packet_id_to_string(int packet_id, cmc_conn_state state,
                          packet_direction direction);

// CGSS: packet_ids
enum packetids_C2S_handshake {
  CMC_PACKETID_C2S_HANDSHAKE_HANDSHAKE = 0x00,
};
enum packetids_S2C_status {
  CMC_PACKETID_S2C_STATUS_RESPONSE = 0x00,
  CMC_PACKETID_S2C_STATUS_PONG = 0x01,
};
enum packetids_C2S_status {
  CMC_PACKETID_C2S_STATUS_REQUEST = 0x00,
  CMC_PACKETID_C2S_STATUS_PING = 0x01,
};
enum packetids_S2C_login {
  CMC_PACKETID_S2C_LOGIN_DISCONNECT = 0x00,
  CMC_PACKETID_S2C_LOGIN_ENCRYPTION_REQUEST = 0x01,
  CMC_PACKETID_S2C_LOGIN_SUCCESS = 0x02,
  CMC_PACKETID_S2C_LOGIN_SET_COMPRESSION = 0x03,
};
enum packetids_C2S_login {
  CMC_PACKETID_C2S_LOGIN_START = 0x00,
  CMC_PACKETID_C2S_LOGIN_ENCRYPTION_RESPONSE = 0x01,
};
enum packetids_S2C_play {
  CMC_PACKETID_S2C_PLAY_KEEP_ALIVE = 0x00,
  CMC_PACKETID_S2C_PLAY_JOIN_GAME = 0x01,
  CMC_PACKETID_S2C_PLAY_CHAT_MESSAGE = 0x02,
  CMC_PACKETID_S2C_PLAY_TIME_UPDATE = 0x03,
  CMC_PACKETID_S2C_PLAY_ENTITY_EQUIPMENT = 0x04,
  CMC_PACKETID_S2C_PLAY_SPAWN_POSITION = 0x05,
  CMC_PACKETID_S2C_PLAY_UPDATE_HEALTH = 0x06,
  CMC_PACKETID_S2C_PLAY_RESPAWN = 0x07,
  CMC_PACKETID_S2C_PLAY_PLAYER_LOOK_AND_POSITION = 0x08,
  CMC_PACKETID_S2C_PLAY_HELD_ITEM_CHANGE = 0x09,
  CMC_PACKETID_S2C_PLAY_USE_BED = 0x0A,
  CMC_PACKETID_S2C_PLAY_ANIMATION = 0x0B,
  CMC_PACKETID_S2C_PLAY_SPAWN_PLAYER = 0x0C,
  CMC_PACKETID_S2C_PLAY_COLLECT_ITEM = 0x0D,
  CMC_PACKETID_S2C_PLAY_SPAWN_MOB = 0x0F,
  CMC_PACKETID_S2C_PLAY_SPAWN_PAINTING = 0x10,
  CMC_PACKETID_S2C_PLAY_SPAWN_EXPERIENCE_ORB = 0x11,
  CMC_PACKETID_S2C_PLAY_ENTITY_VELOCITY = 0x12,
  CMC_PACKETID_S2C_PLAY_ENTITY = 0x14,
  CMC_PACKETID_S2C_PLAY_ENTITY_RELATIVE_MOVE = 0x15,
  CMC_PACKETID_S2C_PLAY_ENTITY_LOOK = 0x16,
  CMC_PACKETID_S2C_PLAY_ENTITY_LOOK_AND_RELATIVE_MOVE = 0x17,
  CMC_PACKETID_S2C_PLAY_ENTITY_TELEPORT = 0x18,
  CMC_PACKETID_S2C_PLAY_ENTITY_HEAD_LOOK = 0x19,
  CMC_PACKETID_S2C_PLAY_ENTITY_STATUS = 0x1A,
  CMC_PACKETID_S2C_PLAY_ATTACH_ENTITY = 0x1B,
  CMC_PACKETID_S2C_PLAY_ENTITY_METADATA = 0x1C,
  CMC_PACKETID_S2C_PLAY_ENTITY_EFFECT = 0x1D,
  CMC_PACKETID_S2C_PLAY_REMOVE_ENTITY_EFFECT = 0x1E,
  CMC_PACKETID_S2C_PLAY_SET_EXPERIENCE = 0x1F,
  CMC_PACKETID_S2C_PLAY_CHUNK_DATA = 0x21,
  CMC_PACKETID_S2C_PLAY_BLOCK_CHANGE = 0x23,
  CMC_PACKETID_S2C_PLAY_BLOCK_ACTION = 0x24,
  CMC_PACKETID_S2C_PLAY_BLOCK_BREAK_ANIMATION = 0x25,
  CMC_PACKETID_S2C_PLAY_EFFECT = 0x28,
  CMC_PACKETID_S2C_PLAY_SOUND_EFFECT = 0x29,
  CMC_PACKETID_S2C_PLAY_CHANGE_GAME_STATE = 0x2B,
  CMC_PACKETID_S2C_PLAY_PLAYER_ABILITIES = 0x39,
  CMC_PACKETID_S2C_PLAY_PLUGIN_MESSAGE = 0x3F,
  CMC_PACKETID_S2C_PLAY_DISCONNECT = 0x40,
  CMC_PACKETID_S2C_PLAY_CHANGE_DIFFICULTY = 0x41,
};
enum packetids_C2S_play {
  CMC_PACKETID_C2S_PLAY_KEEP_ALIVE = 0x00,
};
// CGSE: packet_ids

// CGSS: send_methods_h
void cmc_send_C2S_handshake_handshake_packet(cmc_conn *conn, C2S_handshake_handshake_packet *packet);void cmc_send_S2C_status_response_packet(cmc_conn *conn, S2C_status_response_packet *packet);void cmc_send_S2C_status_pong_packet(cmc_conn *conn, S2C_status_pong_packet *packet);void cmc_send_C2S_status_request_packet(cmc_conn *conn);void cmc_send_C2S_status_ping_packet(cmc_conn *conn, C2S_status_ping_packet *packet);void cmc_send_S2C_login_disconnect_packet(cmc_conn *conn, S2C_login_disconnect_packet *packet);void cmc_send_S2C_login_encryption_request_packet(cmc_conn *conn, S2C_login_encryption_request_packet *packet);void cmc_send_S2C_login_success_packet(cmc_conn *conn, S2C_login_success_packet *packet);void cmc_send_S2C_login_set_compression_packet(cmc_conn *conn, S2C_login_set_compression_packet *packet);void cmc_send_C2S_login_start_packet(cmc_conn *conn, C2S_login_start_packet *packet);void cmc_send_C2S_login_encryption_response_packet(cmc_conn *conn, C2S_login_encryption_response_packet *packet);void cmc_send_S2C_play_keep_alive_packet(cmc_conn *conn, S2C_play_keep_alive_packet *packet);void cmc_send_S2C_play_join_game_packet(cmc_conn *conn, S2C_play_join_game_packet *packet);void cmc_send_S2C_play_chat_message_packet(cmc_conn *conn, S2C_play_chat_message_packet *packet);void cmc_send_S2C_play_time_update_packet(cmc_conn *conn, S2C_play_time_update_packet *packet);void cmc_send_S2C_play_entity_equipment_packet(cmc_conn *conn, S2C_play_entity_equipment_packet *packet);void cmc_send_S2C_play_spawn_position_packet(cmc_conn *conn, S2C_play_spawn_position_packet *packet);void cmc_send_S2C_play_update_health_packet(cmc_conn *conn, S2C_play_update_health_packet *packet);void cmc_send_S2C_play_respawn_packet(cmc_conn *conn, S2C_play_respawn_packet *packet);void cmc_send_S2C_play_player_look_and_position_packet(cmc_conn *conn, S2C_play_player_look_and_position_packet *packet);void cmc_send_S2C_play_held_item_change_packet(cmc_conn *conn, S2C_play_held_item_change_packet *packet);void cmc_send_S2C_play_use_bed_packet(cmc_conn *conn, S2C_play_use_bed_packet *packet);void cmc_send_S2C_play_animation_packet(cmc_conn *conn, S2C_play_animation_packet *packet);void cmc_send_S2C_play_spawn_player_packet(cmc_conn *conn, S2C_play_spawn_player_packet *packet);void cmc_send_S2C_play_collect_item_packet(cmc_conn *conn, S2C_play_collect_item_packet *packet);void cmc_send_S2C_play_spawn_mob_packet(cmc_conn *conn, S2C_play_spawn_mob_packet *packet);void cmc_send_S2C_play_spawn_painting_packet(cmc_conn *conn, S2C_play_spawn_painting_packet *packet);void cmc_send_S2C_play_spawn_experience_orb_packet(cmc_conn *conn, S2C_play_spawn_experience_orb_packet *packet);void cmc_send_S2C_play_entity_velocity_packet(cmc_conn *conn, S2C_play_entity_velocity_packet *packet);void cmc_send_S2C_play_entity_packet(cmc_conn *conn, S2C_play_entity_packet *packet);void cmc_send_S2C_play_entity_relative_move_packet(cmc_conn *conn, S2C_play_entity_relative_move_packet *packet);void cmc_send_S2C_play_entity_look_packet(cmc_conn *conn, S2C_play_entity_look_packet *packet);void cmc_send_S2C_play_entity_look_and_relative_move_packet(cmc_conn *conn, S2C_play_entity_look_and_relative_move_packet *packet);void cmc_send_S2C_play_entity_teleport_packet(cmc_conn *conn, S2C_play_entity_teleport_packet *packet);void cmc_send_S2C_play_entity_head_look_packet(cmc_conn *conn, S2C_play_entity_head_look_packet *packet);void cmc_send_S2C_play_entity_status_packet(cmc_conn *conn, S2C_play_entity_status_packet *packet);void cmc_send_S2C_play_attach_entity_packet(cmc_conn *conn, S2C_play_attach_entity_packet *packet);void cmc_send_S2C_play_entity_metadata_packet(cmc_conn *conn, S2C_play_entity_metadata_packet *packet);void cmc_send_S2C_play_entity_effect_packet(cmc_conn *conn, S2C_play_entity_effect_packet *packet);void cmc_send_S2C_play_remove_entity_effect_packet(cmc_conn *conn, S2C_play_remove_entity_effect_packet *packet);void cmc_send_S2C_play_set_experience_packet(cmc_conn *conn, S2C_play_set_experience_packet *packet);void cmc_send_S2C_play_chunk_data_packet(cmc_conn *conn, S2C_play_chunk_data_packet *packet);void cmc_send_S2C_play_block_change_packet(cmc_conn *conn, S2C_play_block_change_packet *packet);void cmc_send_S2C_play_block_action_packet(cmc_conn *conn, S2C_play_block_action_packet *packet);void cmc_send_S2C_play_block_break_animation_packet(cmc_conn *conn, S2C_play_block_break_animation_packet *packet);void cmc_send_S2C_play_effect_packet(cmc_conn *conn, S2C_play_effect_packet *packet);void cmc_send_S2C_play_sound_effect_packet(cmc_conn *conn, S2C_play_sound_effect_packet *packet);void cmc_send_S2C_play_change_game_state_packet(cmc_conn *conn, S2C_play_change_game_state_packet *packet);void cmc_send_S2C_play_player_abilities_packet(cmc_conn *conn, S2C_play_player_abilities_packet *packet);void cmc_send_S2C_play_plugin_message_packet(cmc_conn *conn, S2C_play_plugin_message_packet *packet);void cmc_send_S2C_play_disconnect_packet(cmc_conn *conn, S2C_play_disconnect_packet *packet);void cmc_send_S2C_play_change_difficulty_packet(cmc_conn *conn, S2C_play_change_difficulty_packet *packet);void cmc_send_C2S_play_keep_alive_packet(cmc_conn *conn, C2S_play_keep_alive_packet *packet);
// CGSE: send_methods_h

// CGSS: unpack_methods_h
C2S_handshake_handshake_packet *unpack_C2S_handshake_handshake_packet(cmc_buffer *buff);S2C_status_response_packet *unpack_S2C_status_response_packet(cmc_buffer *buff);S2C_status_pong_packet *unpack_S2C_status_pong_packet(cmc_buffer *buff);C2S_status_ping_packet *unpack_C2S_status_ping_packet(cmc_buffer *buff);S2C_login_disconnect_packet *unpack_S2C_login_disconnect_packet(cmc_buffer *buff);S2C_login_encryption_request_packet *unpack_S2C_login_encryption_request_packet(cmc_buffer *buff);S2C_login_success_packet *unpack_S2C_login_success_packet(cmc_buffer *buff);S2C_login_set_compression_packet *unpack_S2C_login_set_compression_packet(cmc_buffer *buff);C2S_login_start_packet *unpack_C2S_login_start_packet(cmc_buffer *buff);C2S_login_encryption_response_packet *unpack_C2S_login_encryption_response_packet(cmc_buffer *buff);S2C_play_keep_alive_packet *unpack_S2C_play_keep_alive_packet(cmc_buffer *buff);S2C_play_join_game_packet *unpack_S2C_play_join_game_packet(cmc_buffer *buff);S2C_play_chat_message_packet *unpack_S2C_play_chat_message_packet(cmc_buffer *buff);S2C_play_time_update_packet *unpack_S2C_play_time_update_packet(cmc_buffer *buff);S2C_play_entity_equipment_packet *unpack_S2C_play_entity_equipment_packet(cmc_buffer *buff);S2C_play_spawn_position_packet *unpack_S2C_play_spawn_position_packet(cmc_buffer *buff);S2C_play_update_health_packet *unpack_S2C_play_update_health_packet(cmc_buffer *buff);S2C_play_respawn_packet *unpack_S2C_play_respawn_packet(cmc_buffer *buff);S2C_play_player_look_and_position_packet *unpack_S2C_play_player_look_and_position_packet(cmc_buffer *buff);S2C_play_held_item_change_packet *unpack_S2C_play_held_item_change_packet(cmc_buffer *buff);S2C_play_use_bed_packet *unpack_S2C_play_use_bed_packet(cmc_buffer *buff);S2C_play_animation_packet *unpack_S2C_play_animation_packet(cmc_buffer *buff);S2C_play_spawn_player_packet *unpack_S2C_play_spawn_player_packet(cmc_buffer *buff);S2C_play_collect_item_packet *unpack_S2C_play_collect_item_packet(cmc_buffer *buff);S2C_play_spawn_mob_packet *unpack_S2C_play_spawn_mob_packet(cmc_buffer *buff);S2C_play_spawn_painting_packet *unpack_S2C_play_spawn_painting_packet(cmc_buffer *buff);S2C_play_spawn_experience_orb_packet *unpack_S2C_play_spawn_experience_orb_packet(cmc_buffer *buff);S2C_play_entity_velocity_packet *unpack_S2C_play_entity_velocity_packet(cmc_buffer *buff);S2C_play_entity_packet *unpack_S2C_play_entity_packet(cmc_buffer *buff);S2C_play_entity_relative_move_packet *unpack_S2C_play_entity_relative_move_packet(cmc_buffer *buff);S2C_play_entity_look_packet *unpack_S2C_play_entity_look_packet(cmc_buffer *buff);S2C_play_entity_look_and_relative_move_packet *unpack_S2C_play_entity_look_and_relative_move_packet(cmc_buffer *buff);S2C_play_entity_teleport_packet *unpack_S2C_play_entity_teleport_packet(cmc_buffer *buff);S2C_play_entity_head_look_packet *unpack_S2C_play_entity_head_look_packet(cmc_buffer *buff);S2C_play_entity_status_packet *unpack_S2C_play_entity_status_packet(cmc_buffer *buff);S2C_play_attach_entity_packet *unpack_S2C_play_attach_entity_packet(cmc_buffer *buff);S2C_play_entity_metadata_packet *unpack_S2C_play_entity_metadata_packet(cmc_buffer *buff);S2C_play_entity_effect_packet *unpack_S2C_play_entity_effect_packet(cmc_buffer *buff);S2C_play_remove_entity_effect_packet *unpack_S2C_play_remove_entity_effect_packet(cmc_buffer *buff);S2C_play_set_experience_packet *unpack_S2C_play_set_experience_packet(cmc_buffer *buff);S2C_play_chunk_data_packet *unpack_S2C_play_chunk_data_packet(cmc_buffer *buff);S2C_play_block_change_packet *unpack_S2C_play_block_change_packet(cmc_buffer *buff);S2C_play_block_action_packet *unpack_S2C_play_block_action_packet(cmc_buffer *buff);S2C_play_block_break_animation_packet *unpack_S2C_play_block_break_animation_packet(cmc_buffer *buff);S2C_play_effect_packet *unpack_S2C_play_effect_packet(cmc_buffer *buff);S2C_play_sound_effect_packet *unpack_S2C_play_sound_effect_packet(cmc_buffer *buff);S2C_play_change_game_state_packet *unpack_S2C_play_change_game_state_packet(cmc_buffer *buff);S2C_play_player_abilities_packet *unpack_S2C_play_player_abilities_packet(cmc_buffer *buff);S2C_play_plugin_message_packet *unpack_S2C_play_plugin_message_packet(cmc_buffer *buff);S2C_play_disconnect_packet *unpack_S2C_play_disconnect_packet(cmc_buffer *buff);S2C_play_change_difficulty_packet *unpack_S2C_play_change_difficulty_packet(cmc_buffer *buff);C2S_play_keep_alive_packet *unpack_C2S_play_keep_alive_packet(cmc_buffer *buff);
// CGSE: unpack_methods_h

// CGSS: free_methods_h
void cmc_free_C2S_handshake_handshake_packet(
    C2S_handshake_handshake_packet packet);
void cmc_free_S2C_status_response_packet(S2C_status_response_packet packet);
void cmc_free_S2C_login_disconnect_packet(S2C_login_disconnect_packet packet);
void cmc_free_S2C_login_encryption_request_packet(
    S2C_login_encryption_request_packet packet);
void cmc_free_S2C_login_success_packet(S2C_login_success_packet packet);
void cmc_free_C2S_login_start_packet(C2S_login_start_packet packet);
void cmc_free_C2S_login_encryption_response_packet(
    C2S_login_encryption_response_packet packet);
void cmc_free_S2C_play_join_game_packet(S2C_play_join_game_packet packet);
void cmc_free_S2C_play_chat_message_packet(S2C_play_chat_message_packet packet);
void cmc_free_S2C_play_entity_equipment_packet(
    S2C_play_entity_equipment_packet packet);
void cmc_free_S2C_play_respawn_packet(S2C_play_respawn_packet packet);
void cmc_free_S2C_play_spawn_player_packet(S2C_play_spawn_player_packet packet);
void cmc_free_S2C_play_spawn_mob_packet(S2C_play_spawn_mob_packet packet);
void cmc_free_S2C_play_spawn_painting_packet(
    S2C_play_spawn_painting_packet packet);
void cmc_free_S2C_play_entity_metadata_packet(
    S2C_play_entity_metadata_packet packet);
void cmc_free_S2C_play_chunk_data_packet(S2C_play_chunk_data_packet packet);
void cmc_free_S2C_play_sound_effect_packet(S2C_play_sound_effect_packet packet);
void cmc_free_S2C_play_plugin_message_packet(
    S2C_play_plugin_message_packet packet);
void cmc_free_S2C_play_disconnect_packet(S2C_play_disconnect_packet packet);
// CGSE: free_methods_h