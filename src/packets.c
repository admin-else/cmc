#include <cmc/packets.h>

#include <cmc/buff.h>
#include <cmc/conn.h>
#include <cmc/err.h>
#include <cmc/heap_utils.h>
#include <cmc/nbt.h>
#include <cmc/packet_types.h>

#include <stdbool.h>
#include <stdlib.h>

#include "err_macros.h"

// CGSS: unpack_methods_c
cmc_packet_C2S_handshake_handshake_765
cmc_packet_C2S_handshake_handshake_765_unpack(cmc_buff *buff) {
  cmc_packet_C2S_handshake_handshake_765 packet = {};
  packet.protocole_version =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  packet.server_addr = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff), goto err;);
  packet.server_port =
      CMC_ERRB_ABLE(cmc_buff_unpack_ushort(buff), goto err_server_addr;);
  packet.next_state =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err_server_addr;);
  return packet;
err_server_addr:
  cmc_string_free(packet.server_addr);
err:
  return (cmc_packet_C2S_handshake_handshake_765){};
}
cmc_packet_S2C_status_response_765
cmc_packet_S2C_status_response_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_status_response_765 packet = {};
  packet.response = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_status_response_765){};
}
cmc_packet_S2C_status_pong_765
cmc_packet_S2C_status_pong_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_status_pong_765 packet = {};
  packet.payload = CMC_ERRB_ABLE(cmc_buff_unpack_long(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_status_pong_765){};
}
cmc_packet_C2S_status_ping_765
cmc_packet_C2S_status_ping_765_unpack(cmc_buff *buff) {
  cmc_packet_C2S_status_ping_765 packet = {};
  packet.payload = CMC_ERRB_ABLE(cmc_buff_unpack_long(buff), goto err;);
  return packet;
err:
  return (cmc_packet_C2S_status_ping_765){};
}
cmc_packet_S2C_login_disconnect_765
cmc_packet_S2C_login_disconnect_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_login_disconnect_765 packet = {};
  packet.reason = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_login_disconnect_765){};
}
cmc_packet_S2C_login_encryption_request_765
cmc_packet_S2C_login_encryption_request_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_login_encryption_request_765 packet = {};
  packet.server_id = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff), goto err;);
  packet.public_key =
      CMC_ERRB_ABLE(cmc_buff_unpack_buff(buff), goto err_server_id;);
  packet.verify_token =
      CMC_ERRB_ABLE(cmc_buff_unpack_buff(buff), goto err_public_key;);
  return packet;
err_public_key:
  cmc_buff_free(packet.public_key);
err_server_id:
  cmc_string_free(packet.server_id);
err:
  return (cmc_packet_S2C_login_encryption_request_765){};
}
cmc_packet_S2C_login_success_765
cmc_packet_S2C_login_success_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_login_success_765 packet = {};
  packet.uuid = CMC_ERRB_ABLE(cmc_buff_unpack_uuid(buff), goto err;);
  packet.name = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff), goto err;);
  packet.properties_count =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err_name;);
  return packet;
err_name:
  cmc_string_free(packet.name);
err:
  return (cmc_packet_S2C_login_success_765){};
}
cmc_packet_S2C_login_set_compression_765
cmc_packet_S2C_login_set_compression_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_login_set_compression_765 packet = {};
  packet.threshold = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_login_set_compression_765){};
}
cmc_packet_C2S_login_start_765
cmc_packet_C2S_login_start_765_unpack(cmc_buff *buff) {
  cmc_packet_C2S_login_start_765 packet = {};
  packet.name = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff), goto err;);
  packet.uuid = CMC_ERRB_ABLE(cmc_buff_unpack_uuid(buff), goto err_name;);
  return packet;
err_name:
  cmc_string_free(packet.name);
err:
  return (cmc_packet_C2S_login_start_765){};
}
cmc_packet_C2S_login_encryption_response_765
cmc_packet_C2S_login_encryption_response_765_unpack(cmc_buff *buff) {
  cmc_packet_C2S_login_encryption_response_765 packet = {};
  packet.shared_secret = CMC_ERRB_ABLE(cmc_buff_unpack_buff(buff), goto err;);
  packet.verify_token =
      CMC_ERRB_ABLE(cmc_buff_unpack_buff(buff), goto err_shared_secret;);
  return packet;
err_shared_secret:
  cmc_buff_free(packet.shared_secret);
err:
  return (cmc_packet_C2S_login_encryption_response_765){};
}
cmc_packet_S2C_config_plugin_message_765
cmc_packet_S2C_config_plugin_message_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_config_plugin_message_765 packet = {};
  packet.channel = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff), goto err;);
  packet.data = CMC_ERRB_ABLE(cmc_buff_unpack_buff(buff), goto err_channel;);
  return packet;
err_channel:
  cmc_string_free(packet.channel);
err:
  return (cmc_packet_S2C_config_plugin_message_765){};
}
cmc_packet_S2C_config_disconnect_765
cmc_packet_S2C_config_disconnect_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_config_disconnect_765 packet = {};
  packet.reason = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_config_disconnect_765){};
}
cmc_packet_S2C_config_keep_alive_765
cmc_packet_S2C_config_keep_alive_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_config_keep_alive_765 packet = {};
  packet.keep_alive_id = CMC_ERRB_ABLE(cmc_buff_unpack_long(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_config_keep_alive_765){};
}
cmc_packet_S2C_config_ping_765
cmc_packet_S2C_config_ping_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_config_ping_765 packet = {};
  packet.id = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_config_ping_765){};
}
cmc_packet_S2C_config_registry_data_765
cmc_packet_S2C_config_registry_data_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_config_registry_data_765 packet = {};
  packet.registry_codec = CMC_ERRB_ABLE(cmc_buff_unpack_nbt(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_config_registry_data_765){};
}
cmc_packet_C2S_play_keep_alive_765
cmc_packet_C2S_play_keep_alive_765_unpack(cmc_buff *buff) {
  cmc_packet_C2S_play_keep_alive_765 packet = {};
  packet.keep_alive_id_long =
      CMC_ERRB_ABLE(cmc_buff_unpack_long(buff), goto err;);
  return packet;
err:
  return (cmc_packet_C2S_play_keep_alive_765){};
}
cmc_packet_S2C_play_keep_alive_765
cmc_packet_S2C_play_keep_alive_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_keep_alive_765 packet = {};
  packet.keep_alive_id_long =
      CMC_ERRB_ABLE(cmc_buff_unpack_long(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_keep_alive_765){};
}
cmc_packet_S2C_play_disconnect_765
cmc_packet_S2C_play_disconnect_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_disconnect_765 packet = {};
  packet.reason_nbt = CMC_ERRB_ABLE(cmc_buff_unpack_nbt(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_disconnect_765){};
}
cmc_packet_S2C_login_success_47
cmc_packet_S2C_login_success_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_login_success_47 packet = {};
  packet.uuid_str = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff), goto err;);
  packet.name = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff), goto err_uuid_str;);
  return packet;
err_uuid_str:
  cmc_string_free(packet.uuid_str);
err:
  return (cmc_packet_S2C_login_success_47){};
}
cmc_packet_C2S_login_start_47
cmc_packet_C2S_login_start_47_unpack(cmc_buff *buff) {
  cmc_packet_C2S_login_start_47 packet = {};
  packet.name = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff), goto err;);
  return packet;
err:
  return (cmc_packet_C2S_login_start_47){};
}
cmc_packet_S2C_play_keep_alive_47
cmc_packet_S2C_play_keep_alive_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_keep_alive_47 packet = {};
  packet.keep_alive_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_keep_alive_47){};
}
cmc_packet_S2C_play_join_game_47
cmc_packet_S2C_play_join_game_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_join_game_47 packet = {};
  packet.entity_id = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.gamemode = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  packet.dimension = CMC_ERRB_ABLE(cmc_buff_unpack_char(buff), goto err;);
  packet.difficulty = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  packet.max_players = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  packet.level_type = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff), goto err;);
  packet.reduced_debug_info =
      CMC_ERRB_ABLE(cmc_buff_unpack_bool(buff), goto err_level_type;);
  return packet;
err_level_type:
  cmc_string_free(packet.level_type);
err:
  return (cmc_packet_S2C_play_join_game_47){};
}
cmc_packet_S2C_play_chat_message_47
cmc_packet_S2C_play_chat_message_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_chat_message_47 packet = {};
  packet.message = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff), goto err;);
  packet.position =
      CMC_ERRB_ABLE(cmc_buff_unpack_char(buff), goto err_message;);
  return packet;
err_message:
  cmc_string_free(packet.message);
err:
  return (cmc_packet_S2C_play_chat_message_47){};
}
cmc_packet_S2C_play_time_update_47
cmc_packet_S2C_play_time_update_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_time_update_47 packet = {};
  packet.world_age = CMC_ERRB_ABLE(cmc_buff_unpack_long(buff), goto err;);
  packet.time_of_day = CMC_ERRB_ABLE(cmc_buff_unpack_long(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_time_update_47){};
}
cmc_packet_S2C_play_entity_equipment_47
cmc_packet_S2C_play_entity_equipment_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_entity_equipment_47 packet = {};
  packet.entity_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  packet.slot = CMC_ERRB_ABLE(cmc_buff_unpack_short(buff), goto err;);
  packet.item = CMC_ERRB_ABLE(cmc_buff_unpack_slot(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_entity_equipment_47){};
}
cmc_packet_S2C_play_spawn_position_47
cmc_packet_S2C_play_spawn_position_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_spawn_position_47 packet = {};
  packet.location = CMC_ERRB_ABLE(cmc_buff_unpack_position(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_spawn_position_47){};
}
cmc_packet_S2C_play_update_health_47
cmc_packet_S2C_play_update_health_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_update_health_47 packet = {};
  packet.health = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff), goto err;);
  packet.food = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  packet.food_saturation =
      CMC_ERRB_ABLE(cmc_buff_unpack_float(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_update_health_47){};
}
cmc_packet_S2C_play_respawn_47
cmc_packet_S2C_play_respawn_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_respawn_47 packet = {};
  packet.dimesion = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.difficulty = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  packet.gamemode = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  packet.level_type = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_respawn_47){};
}
cmc_packet_S2C_play_player_look_and_position_47
cmc_packet_S2C_play_player_look_and_position_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_player_look_and_position_47 packet = {};
  packet.x = CMC_ERRB_ABLE(cmc_buff_unpack_double(buff), goto err;);
  packet.y = CMC_ERRB_ABLE(cmc_buff_unpack_double(buff), goto err;);
  packet.z = CMC_ERRB_ABLE(cmc_buff_unpack_double(buff), goto err;);
  packet.yaw = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff), goto err;);
  packet.pitch = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff), goto err;);
  packet.flags = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_player_look_and_position_47){};
}
cmc_packet_S2C_play_held_item_change_47
cmc_packet_S2C_play_held_item_change_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_held_item_change_47 packet = {};
  packet.slot = CMC_ERRB_ABLE(cmc_buff_unpack_char(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_held_item_change_47){};
}
cmc_packet_S2C_play_use_bed_47
cmc_packet_S2C_play_use_bed_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_use_bed_47 packet = {};
  packet.entity_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  packet.location = CMC_ERRB_ABLE(cmc_buff_unpack_position(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_use_bed_47){};
}
cmc_packet_S2C_play_animation_47
cmc_packet_S2C_play_animation_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_animation_47 packet = {};
  packet.entity_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  packet.animation = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_animation_47){};
}
cmc_packet_S2C_play_spawn_player_47
cmc_packet_S2C_play_spawn_player_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_spawn_player_47 packet = {};
  packet.entity_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  packet.uuid = CMC_ERRB_ABLE(cmc_buff_unpack_uuid(buff), goto err;);
  packet.x = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.y = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.z = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.yaw = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  packet.pitch = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  packet.current_item = CMC_ERRB_ABLE(cmc_buff_unpack_short(buff), goto err;);
  packet.meta_data =
      CMC_ERRB_ABLE(cmc_buff_unpack_entity_metadata(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_spawn_player_47){};
}
cmc_packet_S2C_play_collect_item_47
cmc_packet_S2C_play_collect_item_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_collect_item_47 packet = {};
  packet.collected_entity_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  packet.collector_entity_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_collect_item_47){};
}
cmc_packet_S2C_play_spawn_mob_47
cmc_packet_S2C_play_spawn_mob_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_spawn_mob_47 packet = {};
  packet.entity_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  packet.type = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  packet.x = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.y = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.z = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.yaw = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  packet.pitch = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  packet.head_pitch = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  packet.x_vel = CMC_ERRB_ABLE(cmc_buff_unpack_short(buff), goto err;);
  packet.y_vel = CMC_ERRB_ABLE(cmc_buff_unpack_short(buff), goto err;);
  packet.z_vel = CMC_ERRB_ABLE(cmc_buff_unpack_short(buff), goto err;);
  packet.meta_data =
      CMC_ERRB_ABLE(cmc_buff_unpack_entity_metadata(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_spawn_mob_47){};
}
cmc_packet_S2C_play_spawn_painting_47
cmc_packet_S2C_play_spawn_painting_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_spawn_painting_47 packet = {};
  packet.entity_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  packet.title = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff), goto err;);
  packet.location =
      CMC_ERRB_ABLE(cmc_buff_unpack_position(buff), goto err_title;);
  packet.direction = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err_title;);
  return packet;
err_title:
  cmc_string_free(packet.title);
err:
  return (cmc_packet_S2C_play_spawn_painting_47){};
}
cmc_packet_S2C_play_spawn_experience_orb_47
cmc_packet_S2C_play_spawn_experience_orb_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_spawn_experience_orb_47 packet = {};
  packet.entity_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  packet.x = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.y = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.z = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.count = CMC_ERRB_ABLE(cmc_buff_unpack_short(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_spawn_experience_orb_47){};
}
cmc_packet_S2C_play_entity_velocity_47
cmc_packet_S2C_play_entity_velocity_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_entity_velocity_47 packet = {};
  packet.entity_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  packet.x_vel = CMC_ERRB_ABLE(cmc_buff_unpack_short(buff), goto err;);
  packet.y_vel = CMC_ERRB_ABLE(cmc_buff_unpack_short(buff), goto err;);
  packet.z_vel = CMC_ERRB_ABLE(cmc_buff_unpack_short(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_entity_velocity_47){};
}
cmc_packet_S2C_play_entity_47
cmc_packet_S2C_play_entity_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_entity_47 packet = {};
  packet.entity_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_entity_47){};
}
cmc_packet_S2C_play_entity_relative_move_47
cmc_packet_S2C_play_entity_relative_move_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_entity_relative_move_47 packet = {};
  packet.entity_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  packet.delta_x = CMC_ERRB_ABLE(cmc_buff_unpack_char(buff), goto err;);
  packet.delta_y = CMC_ERRB_ABLE(cmc_buff_unpack_char(buff), goto err;);
  packet.delta_z = CMC_ERRB_ABLE(cmc_buff_unpack_char(buff), goto err;);
  packet.on_ground = CMC_ERRB_ABLE(cmc_buff_unpack_bool(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_entity_relative_move_47){};
}
cmc_packet_S2C_play_entity_look_47
cmc_packet_S2C_play_entity_look_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_entity_look_47 packet = {};
  packet.entity_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  packet.yaw = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  packet.pitch = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  packet.on_ground = CMC_ERRB_ABLE(cmc_buff_unpack_bool(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_entity_look_47){};
}
cmc_packet_S2C_play_entity_look_and_relative_move_47
cmc_packet_S2C_play_entity_look_and_relative_move_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_entity_look_and_relative_move_47 packet = {};
  packet.entity_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  packet.delta_x = CMC_ERRB_ABLE(cmc_buff_unpack_char(buff), goto err;);
  packet.delta_y = CMC_ERRB_ABLE(cmc_buff_unpack_char(buff), goto err;);
  packet.delta_z = CMC_ERRB_ABLE(cmc_buff_unpack_char(buff), goto err;);
  packet.yaw = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  packet.pitch = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  packet.on_ground = CMC_ERRB_ABLE(cmc_buff_unpack_bool(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_entity_look_and_relative_move_47){};
}
cmc_packet_S2C_play_entity_teleport_47
cmc_packet_S2C_play_entity_teleport_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_entity_teleport_47 packet = {};
  packet.entity_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  packet.x = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.y = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.z = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.yaw = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  packet.pitch = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  packet.on_ground = CMC_ERRB_ABLE(cmc_buff_unpack_bool(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_entity_teleport_47){};
}
cmc_packet_S2C_play_entity_head_look_47
cmc_packet_S2C_play_entity_head_look_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_entity_head_look_47 packet = {};
  packet.entity_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  packet.head_yaw = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_entity_head_look_47){};
}
cmc_packet_S2C_play_entity_status_47
cmc_packet_S2C_play_entity_status_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_entity_status_47 packet = {};
  packet.entity_id = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.entity_status = CMC_ERRB_ABLE(cmc_buff_unpack_char(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_entity_status_47){};
}
cmc_packet_S2C_play_attach_entity_47
cmc_packet_S2C_play_attach_entity_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_attach_entity_47 packet = {};
  packet.entity_id = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.vehicle_id = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.leash = CMC_ERRB_ABLE(cmc_buff_unpack_bool(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_attach_entity_47){};
}
cmc_packet_S2C_play_entity_metadata_47
cmc_packet_S2C_play_entity_metadata_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_entity_metadata_47 packet = {};
  packet.entity_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  packet.meta_data =
      CMC_ERRB_ABLE(cmc_buff_unpack_entity_metadata(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_entity_metadata_47){};
}
cmc_packet_S2C_play_entity_effect_47
cmc_packet_S2C_play_entity_effect_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_entity_effect_47 packet = {};
  packet.entity_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  packet.effect_id = CMC_ERRB_ABLE(cmc_buff_unpack_char(buff), goto err;);
  packet.amplifier = CMC_ERRB_ABLE(cmc_buff_unpack_char(buff), goto err;);
  packet.duration = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  packet.hide_particles = CMC_ERRB_ABLE(cmc_buff_unpack_bool(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_entity_effect_47){};
}
cmc_packet_S2C_play_remove_entity_effect_47
cmc_packet_S2C_play_remove_entity_effect_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_remove_entity_effect_47 packet = {};
  packet.entity_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  packet.effect_id = CMC_ERRB_ABLE(cmc_buff_unpack_char(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_remove_entity_effect_47){};
}
cmc_packet_S2C_play_set_experience_47
cmc_packet_S2C_play_set_experience_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_set_experience_47 packet = {};
  packet.experience_bar = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff), goto err;);
  packet.level = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  packet.total_experience =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_set_experience_47){};
}
cmc_packet_S2C_play_entity_properties_47
cmc_packet_S2C_play_entity_properties_47_unpack(cmc_buff *buff) {
  int i = 0;
  int j = 0;
  cmc_packet_S2C_play_entity_properties_47 packet = {};
  packet.entity_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  packet.properties_count = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  if (packet.properties_count > 0) {
    packet.properties = CMC_ERRB_ABLE(
        cmc_malloc(packet.properties_count * sizeof(*packet.properties),
                   &buff->err),
        goto err;);
    for (i = 0; i < packet.properties_count; ++i) {
      packet.properties[i].key = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff),
                                               goto err_properties_forloop);
      packet.properties[i].value =
          CMC_ERRB_ABLE(cmc_buff_unpack_double(buff), goto err_properties_key;);
      packet.properties[i].num_of_modifiers =
          CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err_properties_key;);
      if (packet.properties[i].num_of_modifiers > 0) {
        packet.properties[i].modifiers = CMC_ERRB_ABLE(
            cmc_malloc(packet.properties[i].num_of_modifiers *
                           sizeof(*packet.properties[i].modifiers),
                       &buff->err),
            goto err_properties_key;);
        for (j = 0; j < packet.properties[i].num_of_modifiers; ++j) {
          packet.properties[i].modifiers[j].amount =
              CMC_ERRB_ABLE(cmc_buff_unpack_double(buff),
                            goto err_properties_modifiers_forloop);
          packet.properties[i].modifiers[j].operation =
              CMC_ERRB_ABLE(cmc_buff_unpack_char(buff),
                            goto err_properties_modifiers_forloop);
        }
      }
    }
  }
  return packet;
  if (packet.properties_count > 0) {
  err_properties_forloop:
    for (; i > 0; --i) {
      if (packet.properties[i].num_of_modifiers > 0) {
      err_properties_modifiers_forloop:
        for (; j > 0; --j) {
        err_properties_modifiers:
          free(packet.properties[i].modifiers);
        }
      }
    err_properties_key:
      cmc_string_free(packet.properties[i].key);
    err_properties:
      free(packet.properties);
    }
  }
err:
  return (cmc_packet_S2C_play_entity_properties_47){};
}
cmc_packet_S2C_play_chunk_data_47
cmc_packet_S2C_play_chunk_data_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_chunk_data_47 packet = {};
  packet.chunk_x = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.chunk_z = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.ground_up_continuous =
      CMC_ERRB_ABLE(cmc_buff_unpack_bool(buff), goto err;);
  packet.primary_bitmask =
      CMC_ERRB_ABLE(cmc_buff_unpack_ushort(buff), goto err;);
  packet.chunk = CMC_ERRB_ABLE(cmc_buff_unpack_buff(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_chunk_data_47){};
}
cmc_packet_S2C_play_multi_block_change_47
cmc_packet_S2C_play_multi_block_change_47_unpack(cmc_buff *buff) {
  int i = 0;
  cmc_packet_S2C_play_multi_block_change_47 packet = {};
  packet.chunk_x = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.chunk_z = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.record_count = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  if (packet.record_count > 0) {
    packet.records = CMC_ERRB_ABLE(
        cmc_malloc(packet.record_count * sizeof(*packet.records), &buff->err),
        goto err;);
    for (i = 0; i < packet.record_count; ++i) {
      packet.records[i].horizontal_position =
          CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err_records_forloop);
      packet.records[i].vertical_position =
          CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err_records_forloop);
      packet.records[i].block_id =
          CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err_records_forloop);
    }
  }
  return packet;
  if (packet.record_count > 0) {
  err_records_forloop:
    for (; i > 0; --i) {
    err_records:
      free(packet.records);
    }
  }
err:
  return (cmc_packet_S2C_play_multi_block_change_47){};
}
cmc_packet_S2C_play_block_change_47
cmc_packet_S2C_play_block_change_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_block_change_47 packet = {};
  packet.location = CMC_ERRB_ABLE(cmc_buff_unpack_position(buff), goto err;);
  packet.block_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_block_change_47){};
}
cmc_packet_S2C_play_block_action_47
cmc_packet_S2C_play_block_action_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_block_action_47 packet = {};
  packet.location = CMC_ERRB_ABLE(cmc_buff_unpack_position(buff), goto err;);
  packet.block_data_1 = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  packet.block_data_2 = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  packet.block_type = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_block_action_47){};
}
cmc_packet_S2C_play_block_break_animation_47
cmc_packet_S2C_play_block_break_animation_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_block_break_animation_47 packet = {};
  packet.entity_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  packet.location = CMC_ERRB_ABLE(cmc_buff_unpack_position(buff), goto err;);
  packet.destroy_stage = CMC_ERRB_ABLE(cmc_buff_unpack_char(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_block_break_animation_47){};
}
cmc_packet_S2C_play_map_chunk_bulk_47
cmc_packet_S2C_play_map_chunk_bulk_47_unpack(cmc_buff *buff) {
  int i = 0;
  cmc_packet_S2C_play_map_chunk_bulk_47 packet = {};
  packet.sky_light_sent = CMC_ERRB_ABLE(cmc_buff_unpack_bool(buff), goto err;);
  packet.chunk_column_count =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  if (packet.chunk_column_count > 0) {
    packet.chunk_columns = CMC_ERRB_ABLE(
        cmc_malloc(packet.chunk_column_count * sizeof(*packet.chunk_columns),
                   &buff->err),
        goto err;);
    for (i = 0; i < packet.chunk_column_count; ++i) {
      packet.chunk_columns[i].chunk_x = CMC_ERRB_ABLE(
          cmc_buff_unpack_int(buff), goto err_chunk_columns_forloop);
      packet.chunk_columns[i].chunk_z = CMC_ERRB_ABLE(
          cmc_buff_unpack_int(buff), goto err_chunk_columns_forloop);
      packet.chunk_columns[i].bit_mask = CMC_ERRB_ABLE(
          cmc_buff_unpack_ushort(buff), goto err_chunk_columns_forloop);
    }
  }
  packet.chunk =
      CMC_ERRB_ABLE(cmc_buff_unpack_buff(buff), goto err_chunk_columns_forloop);
  return packet;
  if (packet.chunk_column_count > 0) {
  err_chunk_columns_forloop:
    for (; i > 0; --i) {
    err_chunk_columns:
      free(packet.chunk_columns);
    }
  }
err:
  return (cmc_packet_S2C_play_map_chunk_bulk_47){};
}
cmc_packet_S2C_play_explosion_47
cmc_packet_S2C_play_explosion_47_unpack(cmc_buff *buff) {
  int i = 0;
  cmc_packet_S2C_play_explosion_47 packet = {};
  packet.x = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff), goto err;);
  packet.y = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff), goto err;);
  packet.z = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff), goto err;);
  packet.radius = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff), goto err;);
  packet.record_count = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  if (packet.record_count > 0) {
    packet.records = CMC_ERRB_ABLE(
        cmc_malloc(packet.record_count * sizeof(*packet.records), &buff->err),
        goto err;);
    for (i = 0; i < packet.record_count; ++i) {
      packet.records[i].x_offset =
          CMC_ERRB_ABLE(cmc_buff_unpack_char(buff), goto err_records_forloop);
      packet.records[i].y_offset =
          CMC_ERRB_ABLE(cmc_buff_unpack_char(buff), goto err_records_forloop);
      packet.records[i].z_offset =
          CMC_ERRB_ABLE(cmc_buff_unpack_char(buff), goto err_records_forloop);
    }
  }
  packet.x_player_vel =
      CMC_ERRB_ABLE(cmc_buff_unpack_float(buff), goto err_records_forloop);
  packet.y_player_vel =
      CMC_ERRB_ABLE(cmc_buff_unpack_float(buff), goto err_records_forloop);
  packet.z_player_vel =
      CMC_ERRB_ABLE(cmc_buff_unpack_float(buff), goto err_records_forloop);
  return packet;
  if (packet.record_count > 0) {
  err_records_forloop:
    for (; i > 0; --i) {
    err_records:
      free(packet.records);
    }
  }
err:
  return (cmc_packet_S2C_play_explosion_47){};
}
cmc_packet_S2C_play_effect_47
cmc_packet_S2C_play_effect_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_effect_47 packet = {};
  packet.effect_id = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.location = CMC_ERRB_ABLE(cmc_buff_unpack_position(buff), goto err;);
  packet.data = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.d = CMC_ERRB_ABLE(cmc_buff_unpack_bool(buff), goto err;);
  packet.particle_id = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.long_distances = CMC_ERRB_ABLE(cmc_buff_unpack_bool(buff), goto err;);
  packet.x = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff), goto err;);
  packet.y = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff), goto err;);
  packet.z = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff), goto err;);
  packet.x_offset = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff), goto err;);
  packet.y_offset = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff), goto err;);
  packet.z_offset = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff), goto err;);
  packet.particle_data = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff), goto err;);
  packet.particle_count = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  packet.sable_relative_volume =
      CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_effect_47){};
}
cmc_packet_S2C_play_sound_effect_47
cmc_packet_S2C_play_sound_effect_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_sound_effect_47 packet = {};
  packet.sound_name = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff), goto err;);
  packet.x = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err_sound_name;);
  packet.y = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err_sound_name;);
  packet.z = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff), goto err_sound_name;);
  packet.volume =
      CMC_ERRB_ABLE(cmc_buff_unpack_float(buff), goto err_sound_name;);
  packet.pitch =
      CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err_sound_name;);
  return packet;
err_sound_name:
  cmc_string_free(packet.sound_name);
err:
  return (cmc_packet_S2C_play_sound_effect_47){};
}
cmc_packet_S2C_play_change_game_state_47
cmc_packet_S2C_play_change_game_state_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_change_game_state_47 packet = {};
  packet.reason = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  packet.value = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_change_game_state_47){};
}
cmc_packet_S2C_play_player_abilities_47
cmc_packet_S2C_play_player_abilities_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_player_abilities_47 packet = {};
  packet.flags = CMC_ERRB_ABLE(cmc_buff_unpack_char(buff), goto err;);
  packet.flying_speed = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff), goto err;);
  packet.fov_modifier = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_player_abilities_47){};
}
cmc_packet_S2C_play_plugin_message_47
cmc_packet_S2C_play_plugin_message_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_plugin_message_47 packet = {};
  packet.channel = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff), goto err;);
  packet.data = CMC_ERRB_ABLE(cmc_buff_unpack_buff(buff), goto err_channel;);
  return packet;
err_channel:
  cmc_string_free(packet.channel);
err:
  return (cmc_packet_S2C_play_plugin_message_47){};
}
cmc_packet_S2C_play_disconnect_47
cmc_packet_S2C_play_disconnect_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_disconnect_47 packet = {};
  packet.reason = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_disconnect_47){};
}
cmc_packet_S2C_play_change_difficulty_47
cmc_packet_S2C_play_change_difficulty_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_change_difficulty_47 packet = {};
  packet.difficulty = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  return packet;
err:
  return (cmc_packet_S2C_play_change_difficulty_47){};
}
cmc_packet_C2S_play_keep_alive_47
cmc_packet_C2S_play_keep_alive_47_unpack(cmc_buff *buff) {
  cmc_packet_C2S_play_keep_alive_47 packet = {};
  packet.keep_alive_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err;);
  return packet;
err:
  return (cmc_packet_C2S_play_keep_alive_47){};
}
// CGSE: unpack_methods_c

// CGSS: free_methods_c
void cmc_packet_free_cmc_packet_C2S_handshake_handshake_765(
    cmc_packet_C2S_handshake_handshake_765 *packet) {
  cmc_string_free(packet->server_addr);
}
void cmc_packet_free_cmc_packet_S2C_status_response_765(
    cmc_packet_S2C_status_response_765 *packet) {
  cmc_string_free(packet->response);
}
void cmc_packet_free_cmc_packet_S2C_login_disconnect_765(
    cmc_packet_S2C_login_disconnect_765 *packet) {
  cmc_string_free(packet->reason);
}
void cmc_packet_free_cmc_packet_S2C_login_encryption_request_765(
    cmc_packet_S2C_login_encryption_request_765 *packet) {
  cmc_string_free(packet->server_id);
  cmc_buff_free(packet->public_key);
  cmc_buff_free(packet->verify_token);
}
void cmc_packet_free_cmc_packet_S2C_login_success_765(
    cmc_packet_S2C_login_success_765 *packet) {
  cmc_string_free(packet->name);
}
void cmc_packet_free_cmc_packet_C2S_login_start_765(
    cmc_packet_C2S_login_start_765 *packet) {
  cmc_string_free(packet->name);
}
void cmc_packet_free_cmc_packet_C2S_login_encryption_response_765(
    cmc_packet_C2S_login_encryption_response_765 *packet) {
  cmc_buff_free(packet->shared_secret);
  cmc_buff_free(packet->verify_token);
}
void cmc_packet_free_cmc_packet_S2C_config_plugin_message_765(
    cmc_packet_S2C_config_plugin_message_765 *packet) {
  cmc_string_free(packet->channel);
  cmc_buff_free(packet->data);
}
void cmc_packet_free_cmc_packet_S2C_config_disconnect_765(
    cmc_packet_S2C_config_disconnect_765 *packet) {
  cmc_string_free(packet->reason);
}
void cmc_packet_free_cmc_packet_S2C_config_registry_data_765(
    cmc_packet_S2C_config_registry_data_765 *packet) {
  cmc_nbt_free(packet->registry_codec);
}
void cmc_packet_free_cmc_packet_S2C_play_disconnect_765(
    cmc_packet_S2C_play_disconnect_765 *packet) {
  cmc_nbt_free(packet->reason_nbt);
}
void cmc_packet_free_cmc_packet_S2C_login_success_47(
    cmc_packet_S2C_login_success_47 *packet) {
  cmc_string_free(packet->uuid_str);
  cmc_string_free(packet->name);
}
void cmc_packet_free_cmc_packet_C2S_login_start_47(
    cmc_packet_C2S_login_start_47 *packet) {
  cmc_string_free(packet->name);
}
void cmc_packet_free_cmc_packet_S2C_play_join_game_47(
    cmc_packet_S2C_play_join_game_47 *packet) {
  cmc_string_free(packet->level_type);
}
void cmc_packet_free_cmc_packet_S2C_play_chat_message_47(
    cmc_packet_S2C_play_chat_message_47 *packet) {
  cmc_string_free(packet->message);
}
void cmc_packet_free_cmc_packet_S2C_play_entity_equipment_47(
    cmc_packet_S2C_play_entity_equipment_47 *packet) {
  cmc_slot_free(packet->item);
}
void cmc_packet_free_cmc_packet_S2C_play_respawn_47(
    cmc_packet_S2C_play_respawn_47 *packet) {
  cmc_string_free(packet->level_type);
}
void cmc_packet_free_cmc_packet_S2C_play_spawn_player_47(
    cmc_packet_S2C_play_spawn_player_47 *packet) {
  cmc_entity_metadata_free(packet->meta_data);
}
void cmc_packet_free_cmc_packet_S2C_play_spawn_mob_47(
    cmc_packet_S2C_play_spawn_mob_47 *packet) {
  cmc_entity_metadata_free(packet->meta_data);
}
void cmc_packet_free_cmc_packet_S2C_play_spawn_painting_47(
    cmc_packet_S2C_play_spawn_painting_47 *packet) {
  cmc_string_free(packet->title);
}
void cmc_packet_free_cmc_packet_S2C_play_entity_metadata_47(
    cmc_packet_S2C_play_entity_metadata_47 *packet) {
  cmc_entity_metadata_free(packet->meta_data);
}
void cmc_packet_free_cmc_packet_S2C_play_entity_properties_47(
    cmc_packet_S2C_play_entity_properties_47 *packet) {
  if (packet->properties_count > 0) {
    for (; packet->properties_count > 0; --packet->properties_count) {
      cmc_string_free(packet->properties[packet->properties_count].key);
      if (packet->properties[packet->properties_count].num_of_modifiers > 0) {
        free(packet->properties[packet->properties_count].modifiers);
      }
    }
    free(packet->properties);
  }
}
void cmc_packet_free_cmc_packet_S2C_play_chunk_data_47(
    cmc_packet_S2C_play_chunk_data_47 *packet) {
  cmc_buff_free(packet->chunk);
}
void cmc_packet_free_cmc_packet_S2C_play_multi_block_change_47(
    cmc_packet_S2C_play_multi_block_change_47 *packet) {
  if (packet->record_count > 0) {
    free(packet->records);
  }
}
void cmc_packet_free_cmc_packet_S2C_play_map_chunk_bulk_47(
    cmc_packet_S2C_play_map_chunk_bulk_47 *packet) {
  if (packet->chunk_column_count > 0) {
    free(packet->chunk_columns);
  }
  cmc_buff_free(packet->chunk);
}
void cmc_packet_free_cmc_packet_S2C_play_explosion_47(
    cmc_packet_S2C_play_explosion_47 *packet) {
  if (packet->record_count > 0) {
    free(packet->records);
  }
}
void cmc_packet_free_cmc_packet_S2C_play_sound_effect_47(
    cmc_packet_S2C_play_sound_effect_47 *packet) {
  cmc_string_free(packet->sound_name);
}
void cmc_packet_free_cmc_packet_S2C_play_plugin_message_47(
    cmc_packet_S2C_play_plugin_message_47 *packet) {
  cmc_string_free(packet->channel);
  cmc_buff_free(packet->data);
}
void cmc_packet_free_cmc_packet_S2C_play_disconnect_47(
    cmc_packet_S2C_play_disconnect_47 *packet) {
  cmc_string_free(packet->reason);
}
// CGSE: free_methods_c
