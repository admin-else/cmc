#pragma once

#include <cmc/buff.h>
#include <cmc/conn.h>
#include <cmc/packet_types.h>

#include <stdbool.h>

typedef enum {
  CMC_UNKOWN_NAME_ID,
  // CGSS: packet_name_id_define
  CMC_C2S_HANDSHAKE_HANDSHAKE_NAME_ID,
  CMC_S2C_STATUS_RESPONSE_NAME_ID,
  CMC_S2C_STATUS_PONG_NAME_ID,
  CMC_C2S_STATUS_REQUEST_NAME_ID,
  CMC_C2S_STATUS_PING_NAME_ID,
  CMC_S2C_LOGIN_DISCONNECT_NAME_ID,
  CMC_S2C_LOGIN_ENCRYPTION_REQUEST_NAME_ID,
  CMC_S2C_LOGIN_SUCCESS_NAME_ID,
  CMC_S2C_LOGIN_SET_COMPRESSION_NAME_ID,
  CMC_C2S_LOGIN_START_NAME_ID,
  CMC_C2S_LOGIN_ENCRYPTION_RESPONSE_NAME_ID,
  CMC_S2C_PLAY_KEEP_ALIVE_NAME_ID,
  CMC_S2C_PLAY_JOIN_GAME_NAME_ID,
  CMC_S2C_PLAY_CHAT_MESSAGE_NAME_ID,
  CMC_S2C_PLAY_TIME_UPDATE_NAME_ID,
  CMC_S2C_PLAY_ENTITY_EQUIPMENT_NAME_ID,
  CMC_S2C_PLAY_SPAWN_POSITION_NAME_ID,
  CMC_S2C_PLAY_UPDATE_HEALTH_NAME_ID,
  CMC_S2C_PLAY_RESPAWN_NAME_ID,
  CMC_S2C_PLAY_PLAYER_LOOK_AND_POSITION_NAME_ID,
  CMC_S2C_PLAY_HELD_ITEM_CHANGE_NAME_ID,
  CMC_S2C_PLAY_USE_BED_NAME_ID,
  CMC_S2C_PLAY_ANIMATION_NAME_ID,
  CMC_S2C_PLAY_SPAWN_PLAYER_NAME_ID,
  CMC_S2C_PLAY_COLLECT_ITEM_NAME_ID,
  CMC_S2C_PLAY_SPAWN_MOB_NAME_ID,
  CMC_S2C_PLAY_SPAWN_PAINTING_NAME_ID,
  CMC_S2C_PLAY_SPAWN_EXPERIENCE_ORB_NAME_ID,
  CMC_S2C_PLAY_ENTITY_VELOCITY_NAME_ID,
  CMC_S2C_PLAY_ENTITY_NAME_ID,
  CMC_S2C_PLAY_ENTITY_RELATIVE_MOVE_NAME_ID,
  CMC_S2C_PLAY_ENTITY_LOOK_NAME_ID,
  CMC_S2C_PLAY_ENTITY_LOOK_AND_RELATIVE_MOVE_NAME_ID,
  CMC_S2C_PLAY_ENTITY_TELEPORT_NAME_ID,
  CMC_S2C_PLAY_ENTITY_HEAD_LOOK_NAME_ID,
  CMC_S2C_PLAY_ENTITY_STATUS_NAME_ID,
  CMC_S2C_PLAY_ATTACH_ENTITY_NAME_ID,
  CMC_S2C_PLAY_ENTITY_METADATA_NAME_ID,
  CMC_S2C_PLAY_ENTITY_EFFECT_NAME_ID,
  CMC_S2C_PLAY_REMOVE_ENTITY_EFFECT_NAME_ID,
  CMC_S2C_PLAY_SET_EXPERIENCE_NAME_ID,
  CMC_S2C_PLAY_ENTITY_PROPERTIES_NAME_ID,
  CMC_S2C_PLAY_CHUNK_DATA_NAME_ID,
  CMC_S2C_PLAY_MULTI_BLOCK_CHANGE_NAME_ID,
  CMC_S2C_PLAY_BLOCK_CHANGE_NAME_ID,
  CMC_S2C_PLAY_BLOCK_ACTION_NAME_ID,
  CMC_S2C_PLAY_BLOCK_BREAK_ANIMATION_NAME_ID,
  CMC_S2C_PLAY_MAP_CHUNK_BULK_NAME_ID,
  CMC_S2C_PLAY_EXPLOSION_NAME_ID,
  CMC_S2C_PLAY_EFFECT_NAME_ID,
  CMC_S2C_PLAY_SOUND_EFFECT_NAME_ID,
  CMC_S2C_PLAY_CHANGE_GAME_STATE_NAME_ID,
  CMC_S2C_PLAY_PLAYER_ABILITIES_NAME_ID,
  CMC_S2C_PLAY_PLUGIN_MESSAGE_NAME_ID,
  CMC_S2C_PLAY_DISCONNECT_NAME_ID,
  CMC_S2C_PLAY_CHANGE_DIFFICULTY_NAME_ID,
  CMC_C2S_PLAY_KEEP_ALIVE_NAME_ID,
  CMC_C2S_LOGIN_ACKNOWLEDGED_NAME_ID,
  CMC_S2C_CONFIG_PLUGIN_MESSAGE_NAME_ID,
  CMC_S2C_CONFIG_DISCONNECT_NAME_ID,
  CMC_S2C_CONFIG_FINISH_NAME_ID,
  CMC_S2C_CONFIG_KEEP_ALIVE_NAME_ID,
  CMC_S2C_CONFIG_PING_NAME_ID,
  CMC_S2C_CONFIG_REGISTRY_DATA_NAME_ID,
  CMC_S2C_CONFIG_REMOVE_RESOURCE_PACK_NAME_ID,
  CMC_S2C_CONFIG_ADD_RESOURCE_PACK_NAME_ID,
  // CGSE: packet_name_id_define
} cmc_packet_name_id;

cmc_packet_name_id
cmc_packet_id_to_packet_name_id(int packet_id, cmc_conn_state state,
                                cmc_packet_direction direction,
                                cmc_protocol_version protocol_version);

const char *cmc_packet_name_id_string(cmc_packet_name_id id);

// CGSS: send_methods_h
cmc_err
cmc_send_C2S_handshake_handshake_packet(cmc_conn *conn,
                                        C2S_handshake_handshake_packet *packet);
cmc_err cmc_send_S2C_status_response_packet(cmc_conn *conn,
                                            S2C_status_response_packet *packet);
cmc_err cmc_send_S2C_status_pong_packet(cmc_conn *conn,
                                        S2C_status_pong_packet *packet);
cmc_err cmc_send_C2S_status_request_packet(cmc_conn *conn);
cmc_err cmc_send_C2S_status_ping_packet(cmc_conn *conn,
                                        C2S_status_ping_packet *packet);
cmc_err
cmc_send_S2C_login_disconnect_packet(cmc_conn *conn,
                                     S2C_login_disconnect_packet *packet);
cmc_err cmc_send_S2C_login_encryption_request_packet(
    cmc_conn *conn, S2C_login_encryption_request_packet *packet);
cmc_err cmc_send_S2C_login_success_packet(cmc_conn *conn,
                                          S2C_login_success_packet *packet);
cmc_err cmc_send_S2C_login_set_compression_packet(
    cmc_conn *conn, S2C_login_set_compression_packet *packet);
cmc_err cmc_send_C2S_login_start_packet(cmc_conn *conn,
                                        C2S_login_start_packet *packet);
cmc_err cmc_send_C2S_login_encryption_response_packet(
    cmc_conn *conn, C2S_login_encryption_response_packet *packet);
cmc_err cmc_send_S2C_play_keep_alive_packet(cmc_conn *conn,
                                            S2C_play_keep_alive_packet *packet);
cmc_err cmc_send_S2C_play_join_game_packet(cmc_conn *conn,
                                           S2C_play_join_game_packet *packet);
cmc_err
cmc_send_S2C_play_chat_message_packet(cmc_conn *conn,
                                      S2C_play_chat_message_packet *packet);
cmc_err
cmc_send_S2C_play_time_update_packet(cmc_conn *conn,
                                     S2C_play_time_update_packet *packet);
cmc_err cmc_send_S2C_play_entity_equipment_packet(
    cmc_conn *conn, S2C_play_entity_equipment_packet *packet);
cmc_err
cmc_send_S2C_play_spawn_position_packet(cmc_conn *conn,
                                        S2C_play_spawn_position_packet *packet);
cmc_err
cmc_send_S2C_play_update_health_packet(cmc_conn *conn,
                                       S2C_play_update_health_packet *packet);
cmc_err cmc_send_S2C_play_respawn_packet(cmc_conn *conn,
                                         S2C_play_respawn_packet *packet);
cmc_err cmc_send_S2C_play_player_look_and_position_packet(
    cmc_conn *conn, S2C_play_player_look_and_position_packet *packet);
cmc_err cmc_send_S2C_play_held_item_change_packet(
    cmc_conn *conn, S2C_play_held_item_change_packet *packet);
cmc_err cmc_send_S2C_play_use_bed_packet(cmc_conn *conn,
                                         S2C_play_use_bed_packet *packet);
cmc_err cmc_send_S2C_play_animation_packet(cmc_conn *conn,
                                           S2C_play_animation_packet *packet);
cmc_err
cmc_send_S2C_play_spawn_player_packet(cmc_conn *conn,
                                      S2C_play_spawn_player_packet *packet);
cmc_err
cmc_send_S2C_play_collect_item_packet(cmc_conn *conn,
                                      S2C_play_collect_item_packet *packet);
cmc_err cmc_send_S2C_play_spawn_mob_packet(cmc_conn *conn,
                                           S2C_play_spawn_mob_packet *packet);
cmc_err
cmc_send_S2C_play_spawn_painting_packet(cmc_conn *conn,
                                        S2C_play_spawn_painting_packet *packet);
cmc_err cmc_send_S2C_play_spawn_experience_orb_packet(
    cmc_conn *conn, S2C_play_spawn_experience_orb_packet *packet);
cmc_err cmc_send_S2C_play_entity_velocity_packet(
    cmc_conn *conn, S2C_play_entity_velocity_packet *packet);
cmc_err cmc_send_S2C_play_entity_packet(cmc_conn *conn,
                                        S2C_play_entity_packet *packet);
cmc_err cmc_send_S2C_play_entity_relative_move_packet(
    cmc_conn *conn, S2C_play_entity_relative_move_packet *packet);
cmc_err
cmc_send_S2C_play_entity_look_packet(cmc_conn *conn,
                                     S2C_play_entity_look_packet *packet);
cmc_err cmc_send_S2C_play_entity_look_and_relative_move_packet(
    cmc_conn *conn, S2C_play_entity_look_and_relative_move_packet *packet);
cmc_err cmc_send_S2C_play_entity_teleport_packet(
    cmc_conn *conn, S2C_play_entity_teleport_packet *packet);
cmc_err cmc_send_S2C_play_entity_head_look_packet(
    cmc_conn *conn, S2C_play_entity_head_look_packet *packet);
cmc_err
cmc_send_S2C_play_entity_status_packet(cmc_conn *conn,
                                       S2C_play_entity_status_packet *packet);
cmc_err
cmc_send_S2C_play_attach_entity_packet(cmc_conn *conn,
                                       S2C_play_attach_entity_packet *packet);
cmc_err cmc_send_S2C_play_entity_metadata_packet(
    cmc_conn *conn, S2C_play_entity_metadata_packet *packet);
cmc_err
cmc_send_S2C_play_entity_effect_packet(cmc_conn *conn,
                                       S2C_play_entity_effect_packet *packet);
cmc_err cmc_send_S2C_play_remove_entity_effect_packet(
    cmc_conn *conn, S2C_play_remove_entity_effect_packet *packet);
cmc_err
cmc_send_S2C_play_set_experience_packet(cmc_conn *conn,
                                        S2C_play_set_experience_packet *packet);
cmc_err cmc_send_S2C_play_entity_properties_packet(
    cmc_conn *conn, S2C_play_entity_properties_packet *packet);
cmc_err cmc_send_S2C_play_chunk_data_packet(cmc_conn *conn,
                                            S2C_play_chunk_data_packet *packet);
cmc_err cmc_send_S2C_play_multi_block_change_packet(
    cmc_conn *conn, S2C_play_multi_block_change_packet *packet);
cmc_err
cmc_send_S2C_play_block_change_packet(cmc_conn *conn,
                                      S2C_play_block_change_packet *packet);
cmc_err
cmc_send_S2C_play_block_action_packet(cmc_conn *conn,
                                      S2C_play_block_action_packet *packet);
cmc_err cmc_send_S2C_play_block_break_animation_packet(
    cmc_conn *conn, S2C_play_block_break_animation_packet *packet);
cmc_err
cmc_send_S2C_play_map_chunk_bulk_packet(cmc_conn *conn,
                                        S2C_play_map_chunk_bulk_packet *packet);
cmc_err cmc_send_S2C_play_explosion_packet(cmc_conn *conn,
                                           S2C_play_explosion_packet *packet);
cmc_err cmc_send_S2C_play_effect_packet(cmc_conn *conn,
                                        S2C_play_effect_packet *packet);
cmc_err
cmc_send_S2C_play_sound_effect_packet(cmc_conn *conn,
                                      S2C_play_sound_effect_packet *packet);
cmc_err cmc_send_S2C_play_change_game_state_packet(
    cmc_conn *conn, S2C_play_change_game_state_packet *packet);
cmc_err cmc_send_S2C_play_player_abilities_packet(
    cmc_conn *conn, S2C_play_player_abilities_packet *packet);
cmc_err
cmc_send_S2C_play_plugin_message_packet(cmc_conn *conn,
                                        S2C_play_plugin_message_packet *packet);
cmc_err cmc_send_S2C_play_disconnect_packet(cmc_conn *conn,
                                            S2C_play_disconnect_packet *packet);
cmc_err cmc_send_S2C_play_change_difficulty_packet(
    cmc_conn *conn, S2C_play_change_difficulty_packet *packet);
cmc_err cmc_send_C2S_play_keep_alive_packet(cmc_conn *conn,
                                            C2S_play_keep_alive_packet *packet);
cmc_err cmc_send_C2S_login_acknowledged_packet(cmc_conn *conn);
cmc_err cmc_send_S2C_config_plugin_message_packet(
    cmc_conn *conn, S2C_config_plugin_message_packet *packet);
cmc_err
cmc_send_S2C_config_disconnect_packet(cmc_conn *conn,
                                      S2C_config_disconnect_packet *packet);
cmc_err cmc_send_S2C_config_finish_packet(cmc_conn *conn);
cmc_err
cmc_send_S2C_config_keep_alive_packet(cmc_conn *conn,
                                      S2C_config_keep_alive_packet *packet);
cmc_err cmc_send_S2C_config_ping_packet(cmc_conn *conn,
                                        S2C_config_ping_packet *packet);
cmc_err cmc_send_S2C_config_registry_data_packet(
    cmc_conn *conn, S2C_config_registry_data_packet *packet);
cmc_err cmc_send_S2C_config_remove_resource_pack_packet(cmc_conn *conn);
cmc_err cmc_send_S2C_config_add_resource_pack_packet(cmc_conn *conn);
// CGSE: send_methods_h

// CGSS: unpack_methods_h
C2S_handshake_handshake_packet
unpack_C2S_handshake_handshake_packet(cmc_buff *buff);
S2C_status_response_packet unpack_S2C_status_response_packet(cmc_buff *buff);
S2C_status_pong_packet unpack_S2C_status_pong_packet(cmc_buff *buff);
C2S_status_ping_packet unpack_C2S_status_ping_packet(cmc_buff *buff);
S2C_login_disconnect_packet unpack_S2C_login_disconnect_packet(cmc_buff *buff);
S2C_login_encryption_request_packet
unpack_S2C_login_encryption_request_packet(cmc_buff *buff);
S2C_login_success_packet unpack_S2C_login_success_packet(cmc_buff *buff);
S2C_login_set_compression_packet
unpack_S2C_login_set_compression_packet(cmc_buff *buff);
C2S_login_start_packet unpack_C2S_login_start_packet(cmc_buff *buff);
C2S_login_encryption_response_packet
unpack_C2S_login_encryption_response_packet(cmc_buff *buff);
S2C_play_keep_alive_packet unpack_S2C_play_keep_alive_packet(cmc_buff *buff);
S2C_play_join_game_packet unpack_S2C_play_join_game_packet(cmc_buff *buff);
S2C_play_chat_message_packet
unpack_S2C_play_chat_message_packet(cmc_buff *buff);
S2C_play_time_update_packet unpack_S2C_play_time_update_packet(cmc_buff *buff);
S2C_play_entity_equipment_packet
unpack_S2C_play_entity_equipment_packet(cmc_buff *buff);
S2C_play_spawn_position_packet
unpack_S2C_play_spawn_position_packet(cmc_buff *buff);
S2C_play_update_health_packet
unpack_S2C_play_update_health_packet(cmc_buff *buff);
S2C_play_respawn_packet unpack_S2C_play_respawn_packet(cmc_buff *buff);
S2C_play_player_look_and_position_packet
unpack_S2C_play_player_look_and_position_packet(cmc_buff *buff);
S2C_play_held_item_change_packet
unpack_S2C_play_held_item_change_packet(cmc_buff *buff);
S2C_play_use_bed_packet unpack_S2C_play_use_bed_packet(cmc_buff *buff);
S2C_play_animation_packet unpack_S2C_play_animation_packet(cmc_buff *buff);
S2C_play_spawn_player_packet
unpack_S2C_play_spawn_player_packet(cmc_buff *buff);
S2C_play_collect_item_packet
unpack_S2C_play_collect_item_packet(cmc_buff *buff);
S2C_play_spawn_mob_packet unpack_S2C_play_spawn_mob_packet(cmc_buff *buff);
S2C_play_spawn_painting_packet
unpack_S2C_play_spawn_painting_packet(cmc_buff *buff);
S2C_play_spawn_experience_orb_packet
unpack_S2C_play_spawn_experience_orb_packet(cmc_buff *buff);
S2C_play_entity_velocity_packet
unpack_S2C_play_entity_velocity_packet(cmc_buff *buff);
S2C_play_entity_packet unpack_S2C_play_entity_packet(cmc_buff *buff);
S2C_play_entity_relative_move_packet
unpack_S2C_play_entity_relative_move_packet(cmc_buff *buff);
S2C_play_entity_look_packet unpack_S2C_play_entity_look_packet(cmc_buff *buff);
S2C_play_entity_look_and_relative_move_packet
unpack_S2C_play_entity_look_and_relative_move_packet(cmc_buff *buff);
S2C_play_entity_teleport_packet
unpack_S2C_play_entity_teleport_packet(cmc_buff *buff);
S2C_play_entity_head_look_packet
unpack_S2C_play_entity_head_look_packet(cmc_buff *buff);
S2C_play_entity_status_packet
unpack_S2C_play_entity_status_packet(cmc_buff *buff);
S2C_play_attach_entity_packet
unpack_S2C_play_attach_entity_packet(cmc_buff *buff);
S2C_play_entity_metadata_packet
unpack_S2C_play_entity_metadata_packet(cmc_buff *buff);
S2C_play_entity_effect_packet
unpack_S2C_play_entity_effect_packet(cmc_buff *buff);
S2C_play_remove_entity_effect_packet
unpack_S2C_play_remove_entity_effect_packet(cmc_buff *buff);
S2C_play_set_experience_packet
unpack_S2C_play_set_experience_packet(cmc_buff *buff);
S2C_play_entity_properties_packet
unpack_S2C_play_entity_properties_packet(cmc_buff *buff);
S2C_play_chunk_data_packet unpack_S2C_play_chunk_data_packet(cmc_buff *buff);
S2C_play_multi_block_change_packet
unpack_S2C_play_multi_block_change_packet(cmc_buff *buff);
S2C_play_block_change_packet
unpack_S2C_play_block_change_packet(cmc_buff *buff);
S2C_play_block_action_packet
unpack_S2C_play_block_action_packet(cmc_buff *buff);
S2C_play_block_break_animation_packet
unpack_S2C_play_block_break_animation_packet(cmc_buff *buff);
S2C_play_map_chunk_bulk_packet
unpack_S2C_play_map_chunk_bulk_packet(cmc_buff *buff);
S2C_play_explosion_packet unpack_S2C_play_explosion_packet(cmc_buff *buff);
S2C_play_effect_packet unpack_S2C_play_effect_packet(cmc_buff *buff);
S2C_play_sound_effect_packet
unpack_S2C_play_sound_effect_packet(cmc_buff *buff);
S2C_play_change_game_state_packet
unpack_S2C_play_change_game_state_packet(cmc_buff *buff);
S2C_play_player_abilities_packet
unpack_S2C_play_player_abilities_packet(cmc_buff *buff);
S2C_play_plugin_message_packet
unpack_S2C_play_plugin_message_packet(cmc_buff *buff);
S2C_play_disconnect_packet unpack_S2C_play_disconnect_packet(cmc_buff *buff);
S2C_play_change_difficulty_packet
unpack_S2C_play_change_difficulty_packet(cmc_buff *buff);
C2S_play_keep_alive_packet unpack_C2S_play_keep_alive_packet(cmc_buff *buff);
S2C_config_plugin_message_packet
unpack_S2C_config_plugin_message_packet(cmc_buff *buff);
S2C_config_disconnect_packet
unpack_S2C_config_disconnect_packet(cmc_buff *buff);
S2C_config_keep_alive_packet
unpack_S2C_config_keep_alive_packet(cmc_buff *buff);
S2C_config_ping_packet unpack_S2C_config_ping_packet(cmc_buff *buff);
S2C_config_registry_data_packet
unpack_S2C_config_registry_data_packet(cmc_buff *buff);
// CGSE: unpack_methods_h

// CGSS: free_methods_h
void cmc_free_C2S_handshake_handshake_packet(
    C2S_handshake_handshake_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_status_response_packet(S2C_status_response_packet *packet,
                                         cmc_err_extra *err);
void cmc_free_S2C_status_pong_packet(S2C_status_pong_packet *packet,
                                     cmc_err_extra *err);
void cmc_free_C2S_status_ping_packet(C2S_status_ping_packet *packet,
                                     cmc_err_extra *err);
void cmc_free_S2C_login_disconnect_packet(S2C_login_disconnect_packet *packet,
                                          cmc_err_extra *err);
void cmc_free_S2C_login_encryption_request_packet(
    S2C_login_encryption_request_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_login_success_packet(S2C_login_success_packet *packet,
                                       cmc_err_extra *err);
void cmc_free_S2C_login_set_compression_packet(
    S2C_login_set_compression_packet *packet, cmc_err_extra *err);
void cmc_free_C2S_login_start_packet(C2S_login_start_packet *packet,
                                     cmc_err_extra *err);
void cmc_free_C2S_login_encryption_response_packet(
    C2S_login_encryption_response_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_keep_alive_packet(S2C_play_keep_alive_packet *packet,
                                         cmc_err_extra *err);
void cmc_free_S2C_play_join_game_packet(S2C_play_join_game_packet *packet,
                                        cmc_err_extra *err);
void cmc_free_S2C_play_chat_message_packet(S2C_play_chat_message_packet *packet,
                                           cmc_err_extra *err);
void cmc_free_S2C_play_time_update_packet(S2C_play_time_update_packet *packet,
                                          cmc_err_extra *err);
void cmc_free_S2C_play_entity_equipment_packet(
    S2C_play_entity_equipment_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_spawn_position_packet(
    S2C_play_spawn_position_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_update_health_packet(
    S2C_play_update_health_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_respawn_packet(S2C_play_respawn_packet *packet,
                                      cmc_err_extra *err);
void cmc_free_S2C_play_player_look_and_position_packet(
    S2C_play_player_look_and_position_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_held_item_change_packet(
    S2C_play_held_item_change_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_use_bed_packet(S2C_play_use_bed_packet *packet,
                                      cmc_err_extra *err);
void cmc_free_S2C_play_animation_packet(S2C_play_animation_packet *packet,
                                        cmc_err_extra *err);
void cmc_free_S2C_play_spawn_player_packet(S2C_play_spawn_player_packet *packet,
                                           cmc_err_extra *err);
void cmc_free_S2C_play_collect_item_packet(S2C_play_collect_item_packet *packet,
                                           cmc_err_extra *err);
void cmc_free_S2C_play_spawn_mob_packet(S2C_play_spawn_mob_packet *packet,
                                        cmc_err_extra *err);
void cmc_free_S2C_play_spawn_painting_packet(
    S2C_play_spawn_painting_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_spawn_experience_orb_packet(
    S2C_play_spawn_experience_orb_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_entity_velocity_packet(
    S2C_play_entity_velocity_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_entity_packet(S2C_play_entity_packet *packet,
                                     cmc_err_extra *err);
void cmc_free_S2C_play_entity_relative_move_packet(
    S2C_play_entity_relative_move_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_entity_look_packet(S2C_play_entity_look_packet *packet,
                                          cmc_err_extra *err);
void cmc_free_S2C_play_entity_look_and_relative_move_packet(
    S2C_play_entity_look_and_relative_move_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_entity_teleport_packet(
    S2C_play_entity_teleport_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_entity_head_look_packet(
    S2C_play_entity_head_look_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_entity_status_packet(
    S2C_play_entity_status_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_attach_entity_packet(
    S2C_play_attach_entity_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_entity_metadata_packet(
    S2C_play_entity_metadata_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_entity_effect_packet(
    S2C_play_entity_effect_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_remove_entity_effect_packet(
    S2C_play_remove_entity_effect_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_set_experience_packet(
    S2C_play_set_experience_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_entity_properties_packet(
    S2C_play_entity_properties_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_chunk_data_packet(S2C_play_chunk_data_packet *packet,
                                         cmc_err_extra *err);
void cmc_free_S2C_play_multi_block_change_packet(
    S2C_play_multi_block_change_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_block_change_packet(S2C_play_block_change_packet *packet,
                                           cmc_err_extra *err);
void cmc_free_S2C_play_block_action_packet(S2C_play_block_action_packet *packet,
                                           cmc_err_extra *err);
void cmc_free_S2C_play_block_break_animation_packet(
    S2C_play_block_break_animation_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_map_chunk_bulk_packet(
    S2C_play_map_chunk_bulk_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_explosion_packet(S2C_play_explosion_packet *packet,
                                        cmc_err_extra *err);
void cmc_free_S2C_play_effect_packet(S2C_play_effect_packet *packet,
                                     cmc_err_extra *err);
void cmc_free_S2C_play_sound_effect_packet(S2C_play_sound_effect_packet *packet,
                                           cmc_err_extra *err);
void cmc_free_S2C_play_change_game_state_packet(
    S2C_play_change_game_state_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_player_abilities_packet(
    S2C_play_player_abilities_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_plugin_message_packet(
    S2C_play_plugin_message_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_play_disconnect_packet(S2C_play_disconnect_packet *packet,
                                         cmc_err_extra *err);
void cmc_free_S2C_play_change_difficulty_packet(
    S2C_play_change_difficulty_packet *packet, cmc_err_extra *err);
void cmc_free_C2S_play_keep_alive_packet(C2S_play_keep_alive_packet *packet,
                                         cmc_err_extra *err);
void cmc_free_S2C_config_plugin_message_packet(
    S2C_config_plugin_message_packet *packet, cmc_err_extra *err);
void cmc_free_S2C_config_disconnect_packet(S2C_config_disconnect_packet *packet,
                                           cmc_err_extra *err);
void cmc_free_S2C_config_keep_alive_packet(S2C_config_keep_alive_packet *packet,
                                           cmc_err_extra *err);
void cmc_free_S2C_config_ping_packet(S2C_config_ping_packet *packet,
                                     cmc_err_extra *err);
void cmc_free_S2C_config_registry_data_packet(
    S2C_config_registry_data_packet *packet, cmc_err_extra *err);
// CGSE: free_methods_h
