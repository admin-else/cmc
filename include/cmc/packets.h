#pragma once

#include <cmc/buff.h>
#include <cmc/conn.h>
#include <cmc/packet_types.h>

#include <stdbool.h>

// CGSS: unpack_methods_h
cmc_packet_C2S_handshake_handshake_765
cmc_packet_C2S_handshake_handshake_765_unpack(cmc_buff *buff);
cmc_packet_S2C_status_response_765
cmc_packet_S2C_status_response_765_unpack(cmc_buff *buff);
cmc_packet_S2C_status_pong_765
cmc_packet_S2C_status_pong_765_unpack(cmc_buff *buff);
cmc_packet_C2S_status_ping_765
cmc_packet_C2S_status_ping_765_unpack(cmc_buff *buff);
cmc_packet_S2C_login_disconnect_765
cmc_packet_S2C_login_disconnect_765_unpack(cmc_buff *buff);
cmc_packet_S2C_login_encryption_request_765
cmc_packet_S2C_login_encryption_request_765_unpack(cmc_buff *buff);
cmc_packet_S2C_login_success_765
cmc_packet_S2C_login_success_765_unpack(cmc_buff *buff);
cmc_packet_S2C_login_set_compression_765
cmc_packet_S2C_login_set_compression_765_unpack(cmc_buff *buff);
cmc_packet_C2S_login_start_765
cmc_packet_C2S_login_start_765_unpack(cmc_buff *buff);
cmc_packet_C2S_login_encryption_response_765
cmc_packet_C2S_login_encryption_response_765_unpack(cmc_buff *buff);
cmc_packet_S2C_config_plugin_message_765
cmc_packet_S2C_config_plugin_message_765_unpack(cmc_buff *buff);
cmc_packet_S2C_config_disconnect_765
cmc_packet_S2C_config_disconnect_765_unpack(cmc_buff *buff);
cmc_packet_S2C_config_keep_alive_765
cmc_packet_S2C_config_keep_alive_765_unpack(cmc_buff *buff);
cmc_packet_S2C_config_ping_765
cmc_packet_S2C_config_ping_765_unpack(cmc_buff *buff);
cmc_packet_S2C_config_registry_data_765
cmc_packet_S2C_config_registry_data_765_unpack(cmc_buff *buff);
cmc_packet_C2S_play_keep_alive_765
cmc_packet_C2S_play_keep_alive_765_unpack(cmc_buff *buff);
cmc_packet_S2C_play_keep_alive_765
cmc_packet_S2C_play_keep_alive_765_unpack(cmc_buff *buff);
cmc_packet_S2C_play_disconnect_765
cmc_packet_S2C_play_disconnect_765_unpack(cmc_buff *buff);
cmc_packet_C2S_handshake_handshake_47
cmc_packet_C2S_handshake_handshake_47_unpack(cmc_buff *buff);
cmc_packet_S2C_status_response_47
cmc_packet_S2C_status_response_47_unpack(cmc_buff *buff);
cmc_packet_S2C_status_pong_47
cmc_packet_S2C_status_pong_47_unpack(cmc_buff *buff);
cmc_packet_C2S_status_ping_47
cmc_packet_C2S_status_ping_47_unpack(cmc_buff *buff);
cmc_packet_S2C_login_disconnect_47
cmc_packet_S2C_login_disconnect_47_unpack(cmc_buff *buff);
cmc_packet_S2C_login_encryption_request_47
cmc_packet_S2C_login_encryption_request_47_unpack(cmc_buff *buff);
cmc_packet_S2C_login_success_47
cmc_packet_S2C_login_success_47_unpack(cmc_buff *buff);
cmc_packet_S2C_login_set_compression_47
cmc_packet_S2C_login_set_compression_47_unpack(cmc_buff *buff);
cmc_packet_C2S_login_start_47
cmc_packet_C2S_login_start_47_unpack(cmc_buff *buff);
cmc_packet_C2S_login_encryption_response_47
cmc_packet_C2S_login_encryption_response_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_keep_alive_47
cmc_packet_S2C_play_keep_alive_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_join_game_47
cmc_packet_S2C_play_join_game_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_chat_message_47
cmc_packet_S2C_play_chat_message_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_time_update_47
cmc_packet_S2C_play_time_update_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_entity_equipment_47
cmc_packet_S2C_play_entity_equipment_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_spawn_position_47
cmc_packet_S2C_play_spawn_position_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_update_health_47
cmc_packet_S2C_play_update_health_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_respawn_47
cmc_packet_S2C_play_respawn_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_player_look_and_position_47
cmc_packet_S2C_play_player_look_and_position_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_held_item_change_47
cmc_packet_S2C_play_held_item_change_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_use_bed_47
cmc_packet_S2C_play_use_bed_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_animation_47
cmc_packet_S2C_play_animation_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_spawn_player_47
cmc_packet_S2C_play_spawn_player_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_collect_item_47
cmc_packet_S2C_play_collect_item_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_spawn_mob_47
cmc_packet_S2C_play_spawn_mob_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_spawn_painting_47
cmc_packet_S2C_play_spawn_painting_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_spawn_experience_orb_47
cmc_packet_S2C_play_spawn_experience_orb_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_entity_velocity_47
cmc_packet_S2C_play_entity_velocity_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_entity_47
cmc_packet_S2C_play_entity_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_entity_relative_move_47
cmc_packet_S2C_play_entity_relative_move_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_entity_look_47
cmc_packet_S2C_play_entity_look_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_entity_look_and_relative_move_47
cmc_packet_S2C_play_entity_look_and_relative_move_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_entity_teleport_47
cmc_packet_S2C_play_entity_teleport_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_entity_head_look_47
cmc_packet_S2C_play_entity_head_look_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_entity_status_47
cmc_packet_S2C_play_entity_status_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_attach_entity_47
cmc_packet_S2C_play_attach_entity_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_entity_metadata_47
cmc_packet_S2C_play_entity_metadata_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_entity_effect_47
cmc_packet_S2C_play_entity_effect_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_remove_entity_effect_47
cmc_packet_S2C_play_remove_entity_effect_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_set_experience_47
cmc_packet_S2C_play_set_experience_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_entity_properties_47
cmc_packet_S2C_play_entity_properties_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_chunk_data_47
cmc_packet_S2C_play_chunk_data_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_multi_block_change_47
cmc_packet_S2C_play_multi_block_change_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_block_change_47
cmc_packet_S2C_play_block_change_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_block_action_47
cmc_packet_S2C_play_block_action_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_block_break_animation_47
cmc_packet_S2C_play_block_break_animation_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_map_chunk_bulk_47
cmc_packet_S2C_play_map_chunk_bulk_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_explosion_47
cmc_packet_S2C_play_explosion_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_effect_47
cmc_packet_S2C_play_effect_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_sound_effect_47
cmc_packet_S2C_play_sound_effect_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_change_game_state_47
cmc_packet_S2C_play_change_game_state_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_player_abilities_47
cmc_packet_S2C_play_player_abilities_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_plugin_message_47
cmc_packet_S2C_play_plugin_message_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_disconnect_47
cmc_packet_S2C_play_disconnect_47_unpack(cmc_buff *buff);
cmc_packet_S2C_play_change_difficulty_47
cmc_packet_S2C_play_change_difficulty_47_unpack(cmc_buff *buff);
cmc_packet_C2S_play_keep_alive_47
cmc_packet_C2S_play_keep_alive_47_unpack(cmc_buff *buff);
// CGSE: unpack_methods_h

// CGSS: free_methods_h
void cmc_packet_C2S_handshake_handshake_765_free(
    cmc_packet_C2S_handshake_handshake_765 *packet);
void cmc_packet_S2C_status_response_765_free(
    cmc_packet_S2C_status_response_765 *packet);
void cmc_packet_S2C_login_disconnect_765_free(
    cmc_packet_S2C_login_disconnect_765 *packet);
void cmc_packet_S2C_login_encryption_request_765_free(
    cmc_packet_S2C_login_encryption_request_765 *packet);
void cmc_packet_S2C_login_success_765_free(
    cmc_packet_S2C_login_success_765 *packet);
void cmc_packet_C2S_login_start_765_free(
    cmc_packet_C2S_login_start_765 *packet);
void cmc_packet_C2S_login_encryption_response_765_free(
    cmc_packet_C2S_login_encryption_response_765 *packet);
void cmc_packet_S2C_config_plugin_message_765_free(
    cmc_packet_S2C_config_plugin_message_765 *packet);
void cmc_packet_S2C_config_disconnect_765_free(
    cmc_packet_S2C_config_disconnect_765 *packet);
void cmc_packet_S2C_config_registry_data_765_free(
    cmc_packet_S2C_config_registry_data_765 *packet);
void cmc_packet_S2C_play_disconnect_765_free(
    cmc_packet_S2C_play_disconnect_765 *packet);
void cmc_packet_C2S_handshake_handshake_47_free(
    cmc_packet_C2S_handshake_handshake_47 *packet);
void cmc_packet_S2C_status_response_47_free(
    cmc_packet_S2C_status_response_47 *packet);
void cmc_packet_S2C_login_disconnect_47_free(
    cmc_packet_S2C_login_disconnect_47 *packet);
void cmc_packet_S2C_login_encryption_request_47_free(
    cmc_packet_S2C_login_encryption_request_47 *packet);
void cmc_packet_S2C_login_success_47_free(
    cmc_packet_S2C_login_success_47 *packet);
void cmc_packet_C2S_login_start_47_free(cmc_packet_C2S_login_start_47 *packet);
void cmc_packet_C2S_login_encryption_response_47_free(
    cmc_packet_C2S_login_encryption_response_47 *packet);
void cmc_packet_S2C_play_join_game_47_free(
    cmc_packet_S2C_play_join_game_47 *packet);
void cmc_packet_S2C_play_chat_message_47_free(
    cmc_packet_S2C_play_chat_message_47 *packet);
void cmc_packet_S2C_play_entity_equipment_47_free(
    cmc_packet_S2C_play_entity_equipment_47 *packet);
void cmc_packet_S2C_play_respawn_47_free(
    cmc_packet_S2C_play_respawn_47 *packet);
void cmc_packet_S2C_play_spawn_player_47_free(
    cmc_packet_S2C_play_spawn_player_47 *packet);
void cmc_packet_S2C_play_spawn_mob_47_free(
    cmc_packet_S2C_play_spawn_mob_47 *packet);
void cmc_packet_S2C_play_spawn_painting_47_free(
    cmc_packet_S2C_play_spawn_painting_47 *packet);
void cmc_packet_S2C_play_entity_metadata_47_free(
    cmc_packet_S2C_play_entity_metadata_47 *packet);
void cmc_packet_S2C_play_entity_properties_47_free(
    cmc_packet_S2C_play_entity_properties_47 *packet);
void cmc_packet_S2C_play_chunk_data_47_free(
    cmc_packet_S2C_play_chunk_data_47 *packet);
void cmc_packet_S2C_play_multi_block_change_47_free(
    cmc_packet_S2C_play_multi_block_change_47 *packet);
void cmc_packet_S2C_play_map_chunk_bulk_47_free(
    cmc_packet_S2C_play_map_chunk_bulk_47 *packet);
void cmc_packet_S2C_play_explosion_47_free(
    cmc_packet_S2C_play_explosion_47 *packet);
void cmc_packet_S2C_play_sound_effect_47_free(
    cmc_packet_S2C_play_sound_effect_47 *packet);
void cmc_packet_S2C_play_plugin_message_47_free(
    cmc_packet_S2C_play_plugin_message_47 *packet);
void cmc_packet_S2C_play_disconnect_47_free(
    cmc_packet_S2C_play_disconnect_47 *packet);
// CGSE: free_methods_h
// CGSS: pack_methods_h
void cmc_packet_C2S_handshake_handshake_765_pack(
    cmc_buff *buff, cmc_packet_C2S_handshake_handshake_765 *packet);
void cmc_packet_S2C_status_response_765_pack(
    cmc_buff *buff, cmc_packet_S2C_status_response_765 *packet);
void cmc_packet_S2C_status_pong_765_pack(
    cmc_buff *buff, cmc_packet_S2C_status_pong_765 *packet);
void cmc_packet_C2S_status_request_765_pack(cmc_buff *buff);
void cmc_packet_C2S_status_ping_765_pack(
    cmc_buff *buff, cmc_packet_C2S_status_ping_765 *packet);
void cmc_packet_S2C_login_disconnect_765_pack(
    cmc_buff *buff, cmc_packet_S2C_login_disconnect_765 *packet);
void cmc_packet_S2C_login_encryption_request_765_pack(
    cmc_buff *buff, cmc_packet_S2C_login_encryption_request_765 *packet);
void cmc_packet_S2C_login_success_765_pack(
    cmc_buff *buff, cmc_packet_S2C_login_success_765 *packet);
void cmc_packet_S2C_login_set_compression_765_pack(
    cmc_buff *buff, cmc_packet_S2C_login_set_compression_765 *packet);
void cmc_packet_C2S_login_start_765_pack(
    cmc_buff *buff, cmc_packet_C2S_login_start_765 *packet);
void cmc_packet_C2S_login_encryption_response_765_pack(
    cmc_buff *buff, cmc_packet_C2S_login_encryption_response_765 *packet);
void cmc_packet_C2S_login_acknowledged_765_pack(cmc_buff *buff);
void cmc_packet_S2C_config_plugin_message_765_pack(
    cmc_buff *buff, cmc_packet_S2C_config_plugin_message_765 *packet);
void cmc_packet_S2C_config_disconnect_765_pack(
    cmc_buff *buff, cmc_packet_S2C_config_disconnect_765 *packet);
void cmc_packet_S2C_config_finish_765_pack(cmc_buff *buff);
void cmc_packet_S2C_config_keep_alive_765_pack(
    cmc_buff *buff, cmc_packet_S2C_config_keep_alive_765 *packet);
void cmc_packet_S2C_config_ping_765_pack(
    cmc_buff *buff, cmc_packet_S2C_config_ping_765 *packet);
void cmc_packet_S2C_config_registry_data_765_pack(
    cmc_buff *buff, cmc_packet_S2C_config_registry_data_765 *packet);
void cmc_packet_S2C_config_remove_resource_pack_765_pack(cmc_buff *buff);
void cmc_packet_S2C_config_add_resource_pack_765_pack(cmc_buff *buff);
void cmc_packet_C2S_play_keep_alive_765_pack(
    cmc_buff *buff, cmc_packet_C2S_play_keep_alive_765 *packet);
void cmc_packet_S2C_play_keep_alive_765_pack(
    cmc_buff *buff, cmc_packet_S2C_play_keep_alive_765 *packet);
void cmc_packet_S2C_play_disconnect_765_pack(
    cmc_buff *buff, cmc_packet_S2C_play_disconnect_765 *packet);
void cmc_packet_C2S_handshake_handshake_47_pack(
    cmc_buff *buff, cmc_packet_C2S_handshake_handshake_47 *packet);
void cmc_packet_S2C_status_response_47_pack(
    cmc_buff *buff, cmc_packet_S2C_status_response_47 *packet);
void cmc_packet_S2C_status_pong_47_pack(cmc_buff *buff,
                                        cmc_packet_S2C_status_pong_47 *packet);
void cmc_packet_C2S_status_request_47_pack(cmc_buff *buff);
void cmc_packet_C2S_status_ping_47_pack(cmc_buff *buff,
                                        cmc_packet_C2S_status_ping_47 *packet);
void cmc_packet_S2C_login_disconnect_47_pack(
    cmc_buff *buff, cmc_packet_S2C_login_disconnect_47 *packet);
void cmc_packet_S2C_login_encryption_request_47_pack(
    cmc_buff *buff, cmc_packet_S2C_login_encryption_request_47 *packet);
void cmc_packet_S2C_login_success_47_pack(
    cmc_buff *buff, cmc_packet_S2C_login_success_47 *packet);
void cmc_packet_S2C_login_set_compression_47_pack(
    cmc_buff *buff, cmc_packet_S2C_login_set_compression_47 *packet);
void cmc_packet_C2S_login_start_47_pack(cmc_buff *buff,
                                        cmc_packet_C2S_login_start_47 *packet);
void cmc_packet_C2S_login_encryption_response_47_pack(
    cmc_buff *buff, cmc_packet_C2S_login_encryption_response_47 *packet);
void cmc_packet_S2C_play_keep_alive_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_keep_alive_47 *packet);
void cmc_packet_S2C_play_join_game_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_join_game_47 *packet);
void cmc_packet_S2C_play_chat_message_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_chat_message_47 *packet);
void cmc_packet_S2C_play_time_update_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_time_update_47 *packet);
void cmc_packet_S2C_play_entity_equipment_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_entity_equipment_47 *packet);
void cmc_packet_S2C_play_spawn_position_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_spawn_position_47 *packet);
void cmc_packet_S2C_play_update_health_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_update_health_47 *packet);
void cmc_packet_S2C_play_respawn_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_respawn_47 *packet);
void cmc_packet_S2C_play_player_look_and_position_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_player_look_and_position_47 *packet);
void cmc_packet_S2C_play_held_item_change_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_held_item_change_47 *packet);
void cmc_packet_S2C_play_use_bed_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_use_bed_47 *packet);
void cmc_packet_S2C_play_animation_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_animation_47 *packet);
void cmc_packet_S2C_play_spawn_player_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_spawn_player_47 *packet);
void cmc_packet_S2C_play_collect_item_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_collect_item_47 *packet);
void cmc_packet_S2C_play_spawn_mob_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_spawn_mob_47 *packet);
void cmc_packet_S2C_play_spawn_painting_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_spawn_painting_47 *packet);
void cmc_packet_S2C_play_spawn_experience_orb_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_spawn_experience_orb_47 *packet);
void cmc_packet_S2C_play_entity_velocity_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_entity_velocity_47 *packet);
void cmc_packet_S2C_play_entity_47_pack(cmc_buff *buff,
                                        cmc_packet_S2C_play_entity_47 *packet);
void cmc_packet_S2C_play_entity_relative_move_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_entity_relative_move_47 *packet);
void cmc_packet_S2C_play_entity_look_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_entity_look_47 *packet);
void cmc_packet_S2C_play_entity_look_and_relative_move_47_pack(
    cmc_buff *buff,
    cmc_packet_S2C_play_entity_look_and_relative_move_47 *packet);
void cmc_packet_S2C_play_entity_teleport_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_entity_teleport_47 *packet);
void cmc_packet_S2C_play_entity_head_look_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_entity_head_look_47 *packet);
void cmc_packet_S2C_play_entity_status_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_entity_status_47 *packet);
void cmc_packet_S2C_play_attach_entity_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_attach_entity_47 *packet);
void cmc_packet_S2C_play_entity_metadata_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_entity_metadata_47 *packet);
void cmc_packet_S2C_play_entity_effect_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_entity_effect_47 *packet);
void cmc_packet_S2C_play_remove_entity_effect_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_remove_entity_effect_47 *packet);
void cmc_packet_S2C_play_set_experience_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_set_experience_47 *packet);
void cmc_packet_S2C_play_entity_properties_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_entity_properties_47 *packet);
void cmc_packet_S2C_play_chunk_data_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_chunk_data_47 *packet);
void cmc_packet_S2C_play_multi_block_change_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_multi_block_change_47 *packet);
void cmc_packet_S2C_play_block_change_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_block_change_47 *packet);
void cmc_packet_S2C_play_block_action_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_block_action_47 *packet);
void cmc_packet_S2C_play_block_break_animation_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_block_break_animation_47 *packet);
void cmc_packet_S2C_play_map_chunk_bulk_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_map_chunk_bulk_47 *packet);
void cmc_packet_S2C_play_explosion_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_explosion_47 *packet);
void cmc_packet_S2C_play_effect_47_pack(cmc_buff *buff,
                                        cmc_packet_S2C_play_effect_47 *packet);
void cmc_packet_S2C_play_sound_effect_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_sound_effect_47 *packet);
void cmc_packet_S2C_play_change_game_state_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_change_game_state_47 *packet);
void cmc_packet_S2C_play_player_abilities_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_player_abilities_47 *packet);
void cmc_packet_S2C_play_plugin_message_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_plugin_message_47 *packet);
void cmc_packet_S2C_play_disconnect_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_disconnect_47 *packet);
void cmc_packet_S2C_play_change_difficulty_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_change_difficulty_47 *packet);
void cmc_packet_C2S_play_keep_alive_47_pack(
    cmc_buff *buff, cmc_packet_C2S_play_keep_alive_47 *packet);
// CGSE: pack_methods_h
