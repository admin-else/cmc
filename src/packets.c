#include <cmc/packets.h>

#include <cmc/buff.h>
#include <cmc/conn.h>
#include <cmc/err.h>
#include <cmc/heap_utils.h>
#include <cmc/nbt.h>
#include <cmc/packet_types.h>

#include "err_macros.h"
#include <stdbool.h>
#include <stdlib.h>

// CGSS: unpack_methods_c
cmc_packet_C2S_handshake_handshake_765
cmc_packet_C2S_handshake_handshake_765_unpack(cmc_buff *buff) {
  cmc_packet_C2S_handshake_handshake_765 packet = {0};
  packet.protocole_version =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_protocole_version;);
  packet.server_addr =
      CMC_ERRB_ABLE(cmc_buff_unpack_string(buff);, goto err_server_addr;);
  packet.server_port =
      CMC_ERRB_ABLE(cmc_buff_unpack_ushort(buff);, goto err_server_port;);
  packet.next_state =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_next_state;);
  return packet;
err_next_state:
err_server_port:
  cmc_string_free(packet.server_addr);
err_server_addr:
err_protocole_version:
  return (cmc_packet_C2S_handshake_handshake_765){0};
}
cmc_packet_S2C_status_response_765
cmc_packet_S2C_status_response_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_status_response_765 packet = {0};
  packet.response =
      CMC_ERRB_ABLE(cmc_buff_unpack_string(buff);, goto err_response;);
  return packet;
  cmc_string_free(packet.response);
err_response:
  return (cmc_packet_S2C_status_response_765){0};
}
cmc_packet_S2C_status_pong_765
cmc_packet_S2C_status_pong_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_status_pong_765 packet = {0};
  packet.payload =
      CMC_ERRB_ABLE(cmc_buff_unpack_long(buff);, goto err_payload;);
  return packet;
err_payload:
  return (cmc_packet_S2C_status_pong_765){0};
}
cmc_packet_C2S_status_ping_765
cmc_packet_C2S_status_ping_765_unpack(cmc_buff *buff) {
  cmc_packet_C2S_status_ping_765 packet = {0};
  packet.payload =
      CMC_ERRB_ABLE(cmc_buff_unpack_long(buff);, goto err_payload;);
  return packet;
err_payload:
  return (cmc_packet_C2S_status_ping_765){0};
}
cmc_packet_S2C_login_disconnect_765
cmc_packet_S2C_login_disconnect_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_login_disconnect_765 packet = {0};
  packet.reason =
      CMC_ERRB_ABLE(cmc_buff_unpack_string(buff);, goto err_reason;);
  return packet;
  cmc_string_free(packet.reason);
err_reason:
  return (cmc_packet_S2C_login_disconnect_765){0};
}
cmc_packet_S2C_login_encryption_request_765
cmc_packet_S2C_login_encryption_request_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_login_encryption_request_765 packet = {0};
  packet.server_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_string(buff);, goto err_server_id;);
  packet.public_key =
      CMC_ERRB_ABLE(cmc_buff_unpack_buff(buff);, goto err_public_key;);
  packet.verify_token =
      CMC_ERRB_ABLE(cmc_buff_unpack_buff(buff);, goto err_verify_token;);
  return packet;
  cmc_buff_free(packet.verify_token);
err_verify_token:
  cmc_buff_free(packet.public_key);
err_public_key:
  cmc_string_free(packet.server_id);
err_server_id:
  return (cmc_packet_S2C_login_encryption_request_765){0};
}
cmc_packet_S2C_login_success_765
cmc_packet_S2C_login_success_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_login_success_765 packet = {0};
  packet.uuid = CMC_ERRB_ABLE(cmc_buff_unpack_uuid(buff);, goto err_uuid;);
  packet.name = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff);, goto err_name;);
  packet.properties_count =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_properties_count;);
  return packet;
err_properties_count:
  cmc_string_free(packet.name);
err_name:
err_uuid:
  return (cmc_packet_S2C_login_success_765){0};
}
cmc_packet_S2C_login_set_compression_765
cmc_packet_S2C_login_set_compression_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_login_set_compression_765 packet = {0};
  packet.threshold =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_threshold;);
  return packet;
err_threshold:
  return (cmc_packet_S2C_login_set_compression_765){0};
}
cmc_packet_C2S_login_start_765
cmc_packet_C2S_login_start_765_unpack(cmc_buff *buff) {
  cmc_packet_C2S_login_start_765 packet = {0};
  packet.name = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff);, goto err_name;);
  packet.uuid = CMC_ERRB_ABLE(cmc_buff_unpack_uuid(buff);, goto err_uuid;);
  return packet;
err_uuid:
  cmc_string_free(packet.name);
err_name:
  return (cmc_packet_C2S_login_start_765){0};
}
cmc_packet_C2S_login_encryption_response_765
cmc_packet_C2S_login_encryption_response_765_unpack(cmc_buff *buff) {
  cmc_packet_C2S_login_encryption_response_765 packet = {0};
  packet.shared_secret =
      CMC_ERRB_ABLE(cmc_buff_unpack_buff(buff);, goto err_shared_secret;);
  packet.verify_token =
      CMC_ERRB_ABLE(cmc_buff_unpack_buff(buff);, goto err_verify_token;);
  return packet;
  cmc_buff_free(packet.verify_token);
err_verify_token:
  cmc_buff_free(packet.shared_secret);
err_shared_secret:
  return (cmc_packet_C2S_login_encryption_response_765){0};
}
cmc_packet_S2C_config_plugin_message_765
cmc_packet_S2C_config_plugin_message_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_config_plugin_message_765 packet = {0};
  packet.channel =
      CMC_ERRB_ABLE(cmc_buff_unpack_string(buff);, goto err_channel;);
  packet.data = CMC_ERRB_ABLE(cmc_buff_unpack_buff(buff);, goto err_data;);
  return packet;
  cmc_buff_free(packet.data);
err_data:
  cmc_string_free(packet.channel);
err_channel:
  return (cmc_packet_S2C_config_plugin_message_765){0};
}
cmc_packet_S2C_config_disconnect_765
cmc_packet_S2C_config_disconnect_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_config_disconnect_765 packet = {0};
  packet.reason =
      CMC_ERRB_ABLE(cmc_buff_unpack_string(buff);, goto err_reason;);
  return packet;
  cmc_string_free(packet.reason);
err_reason:
  return (cmc_packet_S2C_config_disconnect_765){0};
}
cmc_packet_S2C_config_keep_alive_765
cmc_packet_S2C_config_keep_alive_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_config_keep_alive_765 packet = {0};
  packet.keep_alive_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_long(buff);, goto err_keep_alive_id;);
  return packet;
err_keep_alive_id:
  return (cmc_packet_S2C_config_keep_alive_765){0};
}
cmc_packet_S2C_config_ping_765
cmc_packet_S2C_config_ping_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_config_ping_765 packet = {0};
  packet.id = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_id;);
  return packet;
err_id:
  return (cmc_packet_S2C_config_ping_765){0};
}
cmc_packet_S2C_config_registry_data_765
cmc_packet_S2C_config_registry_data_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_config_registry_data_765 packet = {0};
  packet.registry_codec =
      CMC_ERRB_ABLE(cmc_buff_unpack_nbt(buff);, goto err_registry_codec;);
  return packet;
  cmc_nbt_free(packet.registry_codec);
err_registry_codec:
  return (cmc_packet_S2C_config_registry_data_765){0};
}
cmc_packet_C2S_play_keep_alive_765
cmc_packet_C2S_play_keep_alive_765_unpack(cmc_buff *buff) {
  cmc_packet_C2S_play_keep_alive_765 packet = {0};
  packet.keep_alive_id_long =
      CMC_ERRB_ABLE(cmc_buff_unpack_long(buff);, goto err_keep_alive_id_long;);
  return packet;
err_keep_alive_id_long:
  return (cmc_packet_C2S_play_keep_alive_765){0};
}
cmc_packet_S2C_play_keep_alive_765
cmc_packet_S2C_play_keep_alive_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_keep_alive_765 packet = {0};
  packet.keep_alive_id_long =
      CMC_ERRB_ABLE(cmc_buff_unpack_long(buff);, goto err_keep_alive_id_long;);
  return packet;
err_keep_alive_id_long:
  return (cmc_packet_S2C_play_keep_alive_765){0};
}
cmc_packet_S2C_play_disconnect_765
cmc_packet_S2C_play_disconnect_765_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_disconnect_765 packet = {0};
  packet.reason_nbt =
      CMC_ERRB_ABLE(cmc_buff_unpack_nbt(buff);, goto err_reason_nbt;);
  return packet;
  cmc_nbt_free(packet.reason_nbt);
err_reason_nbt:
  return (cmc_packet_S2C_play_disconnect_765){0};
}
cmc_packet_C2S_handshake_handshake_47
cmc_packet_C2S_handshake_handshake_47_unpack(cmc_buff *buff) {
  cmc_packet_C2S_handshake_handshake_47 packet = {0};
  packet.protocole_version =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_protocole_version;);
  packet.server_addr =
      CMC_ERRB_ABLE(cmc_buff_unpack_string(buff);, goto err_server_addr;);
  packet.server_port =
      CMC_ERRB_ABLE(cmc_buff_unpack_ushort(buff);, goto err_server_port;);
  packet.next_state =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_next_state;);
  return packet;
err_next_state:
err_server_port:
  cmc_string_free(packet.server_addr);
err_server_addr:
err_protocole_version:
  return (cmc_packet_C2S_handshake_handshake_47){0};
}
cmc_packet_S2C_status_response_47
cmc_packet_S2C_status_response_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_status_response_47 packet = {0};
  packet.response =
      CMC_ERRB_ABLE(cmc_buff_unpack_string(buff);, goto err_response;);
  return packet;
  cmc_string_free(packet.response);
err_response:
  return (cmc_packet_S2C_status_response_47){0};
}
cmc_packet_S2C_status_pong_47
cmc_packet_S2C_status_pong_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_status_pong_47 packet = {0};
  packet.payload =
      CMC_ERRB_ABLE(cmc_buff_unpack_long(buff);, goto err_payload;);
  return packet;
err_payload:
  return (cmc_packet_S2C_status_pong_47){0};
}
cmc_packet_C2S_status_ping_47
cmc_packet_C2S_status_ping_47_unpack(cmc_buff *buff) {
  cmc_packet_C2S_status_ping_47 packet = {0};
  packet.payload =
      CMC_ERRB_ABLE(cmc_buff_unpack_long(buff);, goto err_payload;);
  return packet;
err_payload:
  return (cmc_packet_C2S_status_ping_47){0};
}
cmc_packet_S2C_login_disconnect_47
cmc_packet_S2C_login_disconnect_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_login_disconnect_47 packet = {0};
  packet.reason =
      CMC_ERRB_ABLE(cmc_buff_unpack_string(buff);, goto err_reason;);
  return packet;
  cmc_string_free(packet.reason);
err_reason:
  return (cmc_packet_S2C_login_disconnect_47){0};
}
cmc_packet_S2C_login_encryption_request_47
cmc_packet_S2C_login_encryption_request_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_login_encryption_request_47 packet = {0};
  packet.server_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_string(buff);, goto err_server_id;);
  packet.public_key =
      CMC_ERRB_ABLE(cmc_buff_unpack_buff(buff);, goto err_public_key;);
  packet.verify_token =
      CMC_ERRB_ABLE(cmc_buff_unpack_buff(buff);, goto err_verify_token;);
  return packet;
  cmc_buff_free(packet.verify_token);
err_verify_token:
  cmc_buff_free(packet.public_key);
err_public_key:
  cmc_string_free(packet.server_id);
err_server_id:
  return (cmc_packet_S2C_login_encryption_request_47){0};
}
cmc_packet_S2C_login_success_47
cmc_packet_S2C_login_success_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_login_success_47 packet = {0};
  packet.uuid_str =
      CMC_ERRB_ABLE(cmc_buff_unpack_string(buff);, goto err_uuid_str;);
  packet.name = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff);, goto err_name;);
  return packet;
  cmc_string_free(packet.name);
err_name:
  cmc_string_free(packet.uuid_str);
err_uuid_str:
  return (cmc_packet_S2C_login_success_47){0};
}
cmc_packet_S2C_login_set_compression_47
cmc_packet_S2C_login_set_compression_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_login_set_compression_47 packet = {0};
  packet.threshold =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_threshold;);
  return packet;
err_threshold:
  return (cmc_packet_S2C_login_set_compression_47){0};
}
cmc_packet_C2S_login_start_47
cmc_packet_C2S_login_start_47_unpack(cmc_buff *buff) {
  cmc_packet_C2S_login_start_47 packet = {0};
  packet.name = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff);, goto err_name;);
  return packet;
  cmc_string_free(packet.name);
err_name:
  return (cmc_packet_C2S_login_start_47){0};
}
cmc_packet_C2S_login_encryption_response_47
cmc_packet_C2S_login_encryption_response_47_unpack(cmc_buff *buff) {
  cmc_packet_C2S_login_encryption_response_47 packet = {0};
  packet.shared_secret =
      CMC_ERRB_ABLE(cmc_buff_unpack_buff(buff);, goto err_shared_secret;);
  packet.verify_token =
      CMC_ERRB_ABLE(cmc_buff_unpack_buff(buff);, goto err_verify_token;);
  return packet;
  cmc_buff_free(packet.verify_token);
err_verify_token:
  cmc_buff_free(packet.shared_secret);
err_shared_secret:
  return (cmc_packet_C2S_login_encryption_response_47){0};
}
cmc_packet_S2C_play_keep_alive_47
cmc_packet_S2C_play_keep_alive_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_keep_alive_47 packet = {0};
  packet.keep_alive_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_keep_alive_id;);
  return packet;
err_keep_alive_id:
  return (cmc_packet_S2C_play_keep_alive_47){0};
}
cmc_packet_S2C_play_join_game_47
cmc_packet_S2C_play_join_game_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_join_game_47 packet = {0};
  packet.entity_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_entity_id;);
  packet.gamemode =
      CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_gamemode;);
  packet.dimension =
      CMC_ERRB_ABLE(cmc_buff_unpack_char(buff);, goto err_dimension;);
  packet.difficulty =
      CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_difficulty;);
  packet.max_players =
      CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_max_players;);
  packet.level_type =
      CMC_ERRB_ABLE(cmc_buff_unpack_string(buff);, goto err_level_type;);
  packet.reduced_debug_info =
      CMC_ERRB_ABLE(cmc_buff_unpack_bool(buff);, goto err_reduced_debug_info;);
  return packet;
err_reduced_debug_info:
  cmc_string_free(packet.level_type);
err_level_type:
err_max_players:
err_difficulty:
err_dimension:
err_gamemode:
err_entity_id:
  return (cmc_packet_S2C_play_join_game_47){0};
}
cmc_packet_S2C_play_chat_message_47
cmc_packet_S2C_play_chat_message_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_chat_message_47 packet = {0};
  packet.message =
      CMC_ERRB_ABLE(cmc_buff_unpack_string(buff);, goto err_message;);
  packet.position =
      CMC_ERRB_ABLE(cmc_buff_unpack_char(buff);, goto err_position;);
  return packet;
err_position:
  cmc_string_free(packet.message);
err_message:
  return (cmc_packet_S2C_play_chat_message_47){0};
}
cmc_packet_S2C_play_time_update_47
cmc_packet_S2C_play_time_update_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_time_update_47 packet = {0};
  packet.world_age =
      CMC_ERRB_ABLE(cmc_buff_unpack_long(buff);, goto err_world_age;);
  packet.time_of_day =
      CMC_ERRB_ABLE(cmc_buff_unpack_long(buff);, goto err_time_of_day;);
  return packet;
err_time_of_day:
err_world_age:
  return (cmc_packet_S2C_play_time_update_47){0};
}
cmc_packet_S2C_play_entity_equipment_47
cmc_packet_S2C_play_entity_equipment_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_entity_equipment_47 packet = {0};
  packet.entity_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_entity_id;);
  packet.slot = CMC_ERRB_ABLE(cmc_buff_unpack_short(buff);, goto err_slot;);
  packet.item = CMC_ERRB_ABLE(cmc_buff_unpack_slot(buff);, goto err_item;);
  return packet;
  cmc_slot_free(packet.item);
err_item:
err_slot:
err_entity_id:
  return (cmc_packet_S2C_play_entity_equipment_47){0};
}
cmc_packet_S2C_play_spawn_position_47
cmc_packet_S2C_play_spawn_position_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_spawn_position_47 packet = {0};
  packet.location =
      CMC_ERRB_ABLE(cmc_buff_unpack_position(buff);, goto err_location;);
  return packet;
err_location:
  return (cmc_packet_S2C_play_spawn_position_47){0};
}
cmc_packet_S2C_play_update_health_47
cmc_packet_S2C_play_update_health_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_update_health_47 packet = {0};
  packet.health = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff);, goto err_health;);
  packet.food = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_food;);
  packet.food_saturation =
      CMC_ERRB_ABLE(cmc_buff_unpack_float(buff);, goto err_food_saturation;);
  return packet;
err_food_saturation:
err_food:
err_health:
  return (cmc_packet_S2C_play_update_health_47){0};
}
cmc_packet_S2C_play_respawn_47
cmc_packet_S2C_play_respawn_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_respawn_47 packet = {0};
  packet.dimesion =
      CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_dimesion;);
  packet.difficulty =
      CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_difficulty;);
  packet.gamemode =
      CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_gamemode;);
  packet.level_type =
      CMC_ERRB_ABLE(cmc_buff_unpack_string(buff);, goto err_level_type;);
  return packet;
  cmc_string_free(packet.level_type);
err_level_type:
err_gamemode:
err_difficulty:
err_dimesion:
  return (cmc_packet_S2C_play_respawn_47){0};
}
cmc_packet_S2C_play_player_look_and_position_47
cmc_packet_S2C_play_player_look_and_position_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_player_look_and_position_47 packet = {0};
  packet.x = CMC_ERRB_ABLE(cmc_buff_unpack_double(buff);, goto err_x;);
  packet.y = CMC_ERRB_ABLE(cmc_buff_unpack_double(buff);, goto err_y;);
  packet.z = CMC_ERRB_ABLE(cmc_buff_unpack_double(buff);, goto err_z;);
  packet.yaw = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff);, goto err_yaw;);
  packet.pitch = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff);, goto err_pitch;);
  packet.flags = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_flags;);
  return packet;
err_flags:
err_pitch:
err_yaw:
err_z:
err_y:
err_x:
  return (cmc_packet_S2C_play_player_look_and_position_47){0};
}
cmc_packet_S2C_play_held_item_change_47
cmc_packet_S2C_play_held_item_change_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_held_item_change_47 packet = {0};
  packet.slot = CMC_ERRB_ABLE(cmc_buff_unpack_char(buff);, goto err_slot;);
  return packet;
err_slot:
  return (cmc_packet_S2C_play_held_item_change_47){0};
}
cmc_packet_S2C_play_use_bed_47
cmc_packet_S2C_play_use_bed_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_use_bed_47 packet = {0};
  packet.entity_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_entity_id;);
  packet.location =
      CMC_ERRB_ABLE(cmc_buff_unpack_position(buff);, goto err_location;);
  return packet;
err_location:
err_entity_id:
  return (cmc_packet_S2C_play_use_bed_47){0};
}
cmc_packet_S2C_play_animation_47
cmc_packet_S2C_play_animation_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_animation_47 packet = {0};
  packet.entity_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_entity_id;);
  packet.animation =
      CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_animation;);
  return packet;
err_animation:
err_entity_id:
  return (cmc_packet_S2C_play_animation_47){0};
}
cmc_packet_S2C_play_spawn_player_47
cmc_packet_S2C_play_spawn_player_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_spawn_player_47 packet = {0};
  packet.entity_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_entity_id;);
  packet.uuid = CMC_ERRB_ABLE(cmc_buff_unpack_uuid(buff);, goto err_uuid;);
  packet.x = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_x;);
  packet.y = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_y;);
  packet.z = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_z;);
  packet.yaw = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_yaw;);
  packet.pitch = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_pitch;);
  packet.current_item =
      CMC_ERRB_ABLE(cmc_buff_unpack_short(buff);, goto err_current_item;);
  packet.meta_data = CMC_ERRB_ABLE(cmc_buff_unpack_entity_metadata(buff);
                                   , goto err_meta_data;);
  return packet;
  cmc_entity_metadata_free(packet.meta_data);
err_meta_data:
err_current_item:
err_pitch:
err_yaw:
err_z:
err_y:
err_x:
err_uuid:
err_entity_id:
  return (cmc_packet_S2C_play_spawn_player_47){0};
}
cmc_packet_S2C_play_collect_item_47
cmc_packet_S2C_play_collect_item_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_collect_item_47 packet = {0};
  packet.collected_entity_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);
                                             , goto err_collected_entity_id;);
  packet.collector_entity_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);
                                             , goto err_collector_entity_id;);
  return packet;
err_collector_entity_id:
err_collected_entity_id:
  return (cmc_packet_S2C_play_collect_item_47){0};
}
cmc_packet_S2C_play_spawn_mob_47
cmc_packet_S2C_play_spawn_mob_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_spawn_mob_47 packet = {0};
  packet.entity_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_entity_id;);
  packet.type = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_type;);
  packet.x = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_x;);
  packet.y = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_y;);
  packet.z = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_z;);
  packet.yaw = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_yaw;);
  packet.pitch = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_pitch;);
  packet.head_pitch =
      CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_head_pitch;);
  packet.x_vel = CMC_ERRB_ABLE(cmc_buff_unpack_short(buff);, goto err_x_vel;);
  packet.y_vel = CMC_ERRB_ABLE(cmc_buff_unpack_short(buff);, goto err_y_vel;);
  packet.z_vel = CMC_ERRB_ABLE(cmc_buff_unpack_short(buff);, goto err_z_vel;);
  packet.meta_data = CMC_ERRB_ABLE(cmc_buff_unpack_entity_metadata(buff);
                                   , goto err_meta_data;);
  return packet;
  cmc_entity_metadata_free(packet.meta_data);
err_meta_data:
err_z_vel:
err_y_vel:
err_x_vel:
err_head_pitch:
err_pitch:
err_yaw:
err_z:
err_y:
err_x:
err_type:
err_entity_id:
  return (cmc_packet_S2C_play_spawn_mob_47){0};
}
cmc_packet_S2C_play_spawn_painting_47
cmc_packet_S2C_play_spawn_painting_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_spawn_painting_47 packet = {0};
  packet.entity_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_entity_id;);
  packet.title = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff);, goto err_title;);
  packet.location =
      CMC_ERRB_ABLE(cmc_buff_unpack_position(buff);, goto err_location;);
  packet.direction =
      CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_direction;);
  return packet;
err_direction:
err_location:
  cmc_string_free(packet.title);
err_title:
err_entity_id:
  return (cmc_packet_S2C_play_spawn_painting_47){0};
}
cmc_packet_S2C_play_spawn_experience_orb_47
cmc_packet_S2C_play_spawn_experience_orb_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_spawn_experience_orb_47 packet = {0};
  packet.entity_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_entity_id;);
  packet.x = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_x;);
  packet.y = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_y;);
  packet.z = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_z;);
  packet.count = CMC_ERRB_ABLE(cmc_buff_unpack_short(buff);, goto err_count;);
  return packet;
err_count:
err_z:
err_y:
err_x:
err_entity_id:
  return (cmc_packet_S2C_play_spawn_experience_orb_47){0};
}
cmc_packet_S2C_play_entity_velocity_47
cmc_packet_S2C_play_entity_velocity_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_entity_velocity_47 packet = {0};
  packet.entity_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_entity_id;);
  packet.x_vel = CMC_ERRB_ABLE(cmc_buff_unpack_short(buff);, goto err_x_vel;);
  packet.y_vel = CMC_ERRB_ABLE(cmc_buff_unpack_short(buff);, goto err_y_vel;);
  packet.z_vel = CMC_ERRB_ABLE(cmc_buff_unpack_short(buff);, goto err_z_vel;);
  return packet;
err_z_vel:
err_y_vel:
err_x_vel:
err_entity_id:
  return (cmc_packet_S2C_play_entity_velocity_47){0};
}
cmc_packet_S2C_play_entity_47
cmc_packet_S2C_play_entity_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_entity_47 packet = {0};
  packet.entity_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_entity_id;);
  return packet;
err_entity_id:
  return (cmc_packet_S2C_play_entity_47){0};
}
cmc_packet_S2C_play_entity_relative_move_47
cmc_packet_S2C_play_entity_relative_move_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_entity_relative_move_47 packet = {0};
  packet.entity_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_entity_id;);
  packet.delta_x =
      CMC_ERRB_ABLE(cmc_buff_unpack_char(buff);, goto err_delta_x;);
  packet.delta_y =
      CMC_ERRB_ABLE(cmc_buff_unpack_char(buff);, goto err_delta_y;);
  packet.delta_z =
      CMC_ERRB_ABLE(cmc_buff_unpack_char(buff);, goto err_delta_z;);
  packet.on_ground =
      CMC_ERRB_ABLE(cmc_buff_unpack_bool(buff);, goto err_on_ground;);
  return packet;
err_on_ground:
err_delta_z:
err_delta_y:
err_delta_x:
err_entity_id:
  return (cmc_packet_S2C_play_entity_relative_move_47){0};
}
cmc_packet_S2C_play_entity_look_47
cmc_packet_S2C_play_entity_look_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_entity_look_47 packet = {0};
  packet.entity_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_entity_id;);
  packet.yaw = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_yaw;);
  packet.pitch = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_pitch;);
  packet.on_ground =
      CMC_ERRB_ABLE(cmc_buff_unpack_bool(buff);, goto err_on_ground;);
  return packet;
err_on_ground:
err_pitch:
err_yaw:
err_entity_id:
  return (cmc_packet_S2C_play_entity_look_47){0};
}
cmc_packet_S2C_play_entity_look_and_relative_move_47
cmc_packet_S2C_play_entity_look_and_relative_move_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_entity_look_and_relative_move_47 packet = {0};
  packet.entity_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_entity_id;);
  packet.delta_x =
      CMC_ERRB_ABLE(cmc_buff_unpack_char(buff);, goto err_delta_x;);
  packet.delta_y =
      CMC_ERRB_ABLE(cmc_buff_unpack_char(buff);, goto err_delta_y;);
  packet.delta_z =
      CMC_ERRB_ABLE(cmc_buff_unpack_char(buff);, goto err_delta_z;);
  packet.yaw = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_yaw;);
  packet.pitch = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_pitch;);
  packet.on_ground =
      CMC_ERRB_ABLE(cmc_buff_unpack_bool(buff);, goto err_on_ground;);
  return packet;
err_on_ground:
err_pitch:
err_yaw:
err_delta_z:
err_delta_y:
err_delta_x:
err_entity_id:
  return (cmc_packet_S2C_play_entity_look_and_relative_move_47){0};
}
cmc_packet_S2C_play_entity_teleport_47
cmc_packet_S2C_play_entity_teleport_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_entity_teleport_47 packet = {0};
  packet.entity_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_entity_id;);
  packet.x = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_x;);
  packet.y = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_y;);
  packet.z = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_z;);
  packet.yaw = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_yaw;);
  packet.pitch = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_pitch;);
  packet.on_ground =
      CMC_ERRB_ABLE(cmc_buff_unpack_bool(buff);, goto err_on_ground;);
  return packet;
err_on_ground:
err_pitch:
err_yaw:
err_z:
err_y:
err_x:
err_entity_id:
  return (cmc_packet_S2C_play_entity_teleport_47){0};
}
cmc_packet_S2C_play_entity_head_look_47
cmc_packet_S2C_play_entity_head_look_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_entity_head_look_47 packet = {0};
  packet.entity_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_entity_id;);
  packet.head_yaw =
      CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_head_yaw;);
  return packet;
err_head_yaw:
err_entity_id:
  return (cmc_packet_S2C_play_entity_head_look_47){0};
}
cmc_packet_S2C_play_entity_status_47
cmc_packet_S2C_play_entity_status_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_entity_status_47 packet = {0};
  packet.entity_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_entity_id;);
  packet.entity_status =
      CMC_ERRB_ABLE(cmc_buff_unpack_char(buff);, goto err_entity_status;);
  return packet;
err_entity_status:
err_entity_id:
  return (cmc_packet_S2C_play_entity_status_47){0};
}
cmc_packet_S2C_play_attach_entity_47
cmc_packet_S2C_play_attach_entity_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_attach_entity_47 packet = {0};
  packet.entity_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_entity_id;);
  packet.vehicle_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_vehicle_id;);
  packet.leash = CMC_ERRB_ABLE(cmc_buff_unpack_bool(buff);, goto err_leash;);
  return packet;
err_leash:
err_vehicle_id:
err_entity_id:
  return (cmc_packet_S2C_play_attach_entity_47){0};
}
cmc_packet_S2C_play_entity_metadata_47
cmc_packet_S2C_play_entity_metadata_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_entity_metadata_47 packet = {0};
  packet.entity_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_entity_id;);
  packet.meta_data = CMC_ERRB_ABLE(cmc_buff_unpack_entity_metadata(buff);
                                   , goto err_meta_data;);
  return packet;
  cmc_entity_metadata_free(packet.meta_data);
err_meta_data:
err_entity_id:
  return (cmc_packet_S2C_play_entity_metadata_47){0};
}
cmc_packet_S2C_play_entity_effect_47
cmc_packet_S2C_play_entity_effect_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_entity_effect_47 packet = {0};
  packet.entity_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_entity_id;);
  packet.effect_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_char(buff);, goto err_effect_id;);
  packet.amplifier =
      CMC_ERRB_ABLE(cmc_buff_unpack_char(buff);, goto err_amplifier;);
  packet.duration =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_duration;);
  packet.hide_particles =
      CMC_ERRB_ABLE(cmc_buff_unpack_bool(buff);, goto err_hide_particles;);
  return packet;
err_hide_particles:
err_duration:
err_amplifier:
err_effect_id:
err_entity_id:
  return (cmc_packet_S2C_play_entity_effect_47){0};
}
cmc_packet_S2C_play_remove_entity_effect_47
cmc_packet_S2C_play_remove_entity_effect_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_remove_entity_effect_47 packet = {0};
  packet.entity_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_entity_id;);
  packet.effect_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_char(buff);, goto err_effect_id;);
  return packet;
err_effect_id:
err_entity_id:
  return (cmc_packet_S2C_play_remove_entity_effect_47){0};
}
cmc_packet_S2C_play_set_experience_47
cmc_packet_S2C_play_set_experience_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_set_experience_47 packet = {0};
  packet.experience_bar =
      CMC_ERRB_ABLE(cmc_buff_unpack_float(buff);, goto err_experience_bar;);
  packet.level = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_level;);
  packet.total_experience =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_total_experience;);
  return packet;
err_total_experience:
err_level:
err_experience_bar:
  return (cmc_packet_S2C_play_set_experience_47){0};
}
cmc_packet_S2C_play_entity_properties_47
cmc_packet_S2C_play_entity_properties_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_entity_properties_47 packet = {0};
  size_t i = 0;
  size_t j = 0;
  packet.entity_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_entity_id;);
  packet.properties_count =
      CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_properties_count;);
  if (packet.properties_count > 0) {
    packet.properties = CMC_ERRB_ABLE(
        cmc_malloc_packet_array(buff, packet.properties_count *
                                          sizeof(*packet.properties)),
        goto err_properties;);
    for (; i < packet.properties_count; ++i) {
      packet.properties[i].key = CMC_ERRB_ABLE(cmc_buff_unpack_string(buff);
                                               , goto err_properties_key;);
      packet.properties[i].value = CMC_ERRB_ABLE(cmc_buff_unpack_double(buff);
                                                 , goto err_properties_value;);
      packet.properties[i].num_of_modifiers = CMC_ERRB_ABLE(
          cmc_buff_unpack_varint(buff);, goto err_properties_num_of_modifiers;);
      if (packet.properties[i].num_of_modifiers > 0) {
        packet.properties[i].modifiers = CMC_ERRB_ABLE(
            cmc_malloc_packet_array(
                buff, packet.properties[i].num_of_modifiers *
                          sizeof(*packet.properties[i].modifiers)),
            goto err_properties_modifiers;);
        for (; j < packet.properties[i].num_of_modifiers; ++j) {
          packet.properties[i].modifiers[j].amount =
              CMC_ERRB_ABLE(cmc_buff_unpack_double(buff);
                            , goto err_properties_modifiers_amount;);
          packet.properties[i].modifiers[j].operation =
              CMC_ERRB_ABLE(cmc_buff_unpack_char(buff);
                            , goto err_properties_modifiers_operation;);
        }
      }
    }
  }
  return packet;
  if (packet.properties_count > 0) {
    if (err_properties_num_of_modifiers > 0) {
      cmc_free(err_properties_modifiers);
    }
    cmc_string_free(err_properties_key);
    cmc_free(packet.properties);
  err_properties:
  }
err_properties_count:
err_entity_id:
  return (cmc_packet_S2C_play_entity_properties_47){0};
}
cmc_packet_S2C_play_chunk_data_47
cmc_packet_S2C_play_chunk_data_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_chunk_data_47 packet = {0};
  packet.chunk_x = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_chunk_x;);
  packet.chunk_z = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_chunk_z;);
  packet.ground_up_continuous = CMC_ERRB_ABLE(cmc_buff_unpack_bool(buff);
                                              , goto err_ground_up_continuous;);
  packet.primary_bitmask =
      CMC_ERRB_ABLE(cmc_buff_unpack_ushort(buff);, goto err_primary_bitmask;);
  packet.chunk = CMC_ERRB_ABLE(cmc_buff_unpack_buff(buff);, goto err_chunk;);
  return packet;
  cmc_buff_free(packet.chunk);
err_chunk:
err_primary_bitmask:
err_ground_up_continuous:
err_chunk_z:
err_chunk_x:
  return (cmc_packet_S2C_play_chunk_data_47){0};
}
cmc_packet_S2C_play_multi_block_change_47
cmc_packet_S2C_play_multi_block_change_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_multi_block_change_47 packet = {0};
  size_t i = 0;
  packet.chunk_x = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_chunk_x;);
  packet.chunk_z = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_chunk_z;);
  packet.record_count =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_record_count;);
  if (packet.record_count > 0) {
    packet.records =
        CMC_ERRB_ABLE(cmc_malloc_packet_array(
                          buff, packet.record_count * sizeof(*packet.records)),
                      goto err_records;);
    for (; i < packet.record_count; ++i) {
      packet.records[i].horizontal_position = CMC_ERRB_ABLE(
          cmc_buff_unpack_byte(buff);, goto err_records_horizontal_position;);
      packet.records[i].vertical_position = CMC_ERRB_ABLE(
          cmc_buff_unpack_byte(buff);, goto err_records_vertical_position;);
      packet.records[i].block_id = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);
                                                 , goto err_records_block_id;);
    }
  }
  return packet;
  if (packet.record_count > 0) {
    cmc_free(packet.records);
  err_records:
  }
err_record_count:
err_chunk_z:
err_chunk_x:
  return (cmc_packet_S2C_play_multi_block_change_47){0};
}
cmc_packet_S2C_play_block_change_47
cmc_packet_S2C_play_block_change_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_block_change_47 packet = {0};
  packet.location =
      CMC_ERRB_ABLE(cmc_buff_unpack_position(buff);, goto err_location;);
  packet.block_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_block_id;);
  return packet;
err_block_id:
err_location:
  return (cmc_packet_S2C_play_block_change_47){0};
}
cmc_packet_S2C_play_block_action_47
cmc_packet_S2C_play_block_action_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_block_action_47 packet = {0};
  packet.location =
      CMC_ERRB_ABLE(cmc_buff_unpack_position(buff);, goto err_location;);
  packet.block_data_1 =
      CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_block_data_1;);
  packet.block_data_2 =
      CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_block_data_2;);
  packet.block_type =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_block_type;);
  return packet;
err_block_type:
err_block_data_2:
err_block_data_1:
err_location:
  return (cmc_packet_S2C_play_block_action_47){0};
}
cmc_packet_S2C_play_block_break_animation_47
cmc_packet_S2C_play_block_break_animation_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_block_break_animation_47 packet = {0};
  packet.entity_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_entity_id;);
  packet.location =
      CMC_ERRB_ABLE(cmc_buff_unpack_position(buff);, goto err_location;);
  packet.destroy_stage =
      CMC_ERRB_ABLE(cmc_buff_unpack_char(buff);, goto err_destroy_stage;);
  return packet;
err_destroy_stage:
err_location:
err_entity_id:
  return (cmc_packet_S2C_play_block_break_animation_47){0};
}
cmc_packet_S2C_play_map_chunk_bulk_47
cmc_packet_S2C_play_map_chunk_bulk_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_map_chunk_bulk_47 packet = {0};
  size_t i = 0;
  packet.sky_light_sent =
      CMC_ERRB_ABLE(cmc_buff_unpack_bool(buff);, goto err_sky_light_sent;);
  packet.chunk_column_count = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);
                                            , goto err_chunk_column_count;);
  if (packet.chunk_column_count > 0) {
    packet.chunk_columns = CMC_ERRB_ABLE(
        cmc_malloc_packet_array(buff, packet.chunk_column_count *
                                          sizeof(*packet.chunk_columns)),
        goto err_chunk_columns;);
    for (; i < packet.chunk_column_count; ++i) {
      packet.chunk_columns[i].chunk_x = CMC_ERRB_ABLE(
          cmc_buff_unpack_int(buff);, goto err_chunk_columns_chunk_x;);
      packet.chunk_columns[i].chunk_z = CMC_ERRB_ABLE(
          cmc_buff_unpack_int(buff);, goto err_chunk_columns_chunk_z;);
      packet.chunk_columns[i].bit_mask = CMC_ERRB_ABLE(
          cmc_buff_unpack_ushort(buff);, goto err_chunk_columns_bit_mask;);
    }
  }
  packet.chunk = CMC_ERRB_ABLE(cmc_buff_unpack_buff(buff);, goto err_chunk;);
  return packet;
  cmc_buff_free(packet.chunk);
err_chunk:
  if (packet.chunk_column_count > 0) {
    cmc_free(packet.chunk_columns);
  err_chunk_columns:
  }
err_chunk_column_count:
err_sky_light_sent:
  return (cmc_packet_S2C_play_map_chunk_bulk_47){0};
}
cmc_packet_S2C_play_explosion_47
cmc_packet_S2C_play_explosion_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_explosion_47 packet = {0};
  size_t i = 0;
  packet.x = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff);, goto err_x;);
  packet.y = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff);, goto err_y;);
  packet.z = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff);, goto err_z;);
  packet.radius = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff);, goto err_radius;);
  packet.record_count =
      CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_record_count;);
  if (packet.record_count > 0) {
    packet.records =
        CMC_ERRB_ABLE(cmc_malloc_packet_array(
                          buff, packet.record_count * sizeof(*packet.records)),
                      goto err_records;);
    for (; i < packet.record_count; ++i) {
      packet.records[i].x_offset = CMC_ERRB_ABLE(cmc_buff_unpack_char(buff);
                                                 , goto err_records_x_offset;);
      packet.records[i].y_offset = CMC_ERRB_ABLE(cmc_buff_unpack_char(buff);
                                                 , goto err_records_y_offset;);
      packet.records[i].z_offset = CMC_ERRB_ABLE(cmc_buff_unpack_char(buff);
                                                 , goto err_records_z_offset;);
    }
  }
  packet.x_player_vel =
      CMC_ERRB_ABLE(cmc_buff_unpack_float(buff);, goto err_x_player_vel;);
  packet.y_player_vel =
      CMC_ERRB_ABLE(cmc_buff_unpack_float(buff);, goto err_y_player_vel;);
  packet.z_player_vel =
      CMC_ERRB_ABLE(cmc_buff_unpack_float(buff);, goto err_z_player_vel;);
  return packet;
err_z_player_vel:
err_y_player_vel:
err_x_player_vel:
  if (packet.record_count > 0) {
    cmc_free(packet.records);
  err_records:
  }
err_record_count:
err_radius:
err_z:
err_y:
err_x:
  return (cmc_packet_S2C_play_explosion_47){0};
}
cmc_packet_S2C_play_effect_47
cmc_packet_S2C_play_effect_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_effect_47 packet = {0};
  packet.effect_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_effect_id;);
  packet.location =
      CMC_ERRB_ABLE(cmc_buff_unpack_position(buff);, goto err_location;);
  packet.data = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_data;);
  packet.d = CMC_ERRB_ABLE(cmc_buff_unpack_bool(buff);, goto err_d;);
  packet.particle_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_particle_id;);
  packet.long_distances =
      CMC_ERRB_ABLE(cmc_buff_unpack_bool(buff);, goto err_long_distances;);
  packet.x = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff);, goto err_x;);
  packet.y = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff);, goto err_y;);
  packet.z = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff);, goto err_z;);
  packet.x_offset =
      CMC_ERRB_ABLE(cmc_buff_unpack_float(buff);, goto err_x_offset;);
  packet.y_offset =
      CMC_ERRB_ABLE(cmc_buff_unpack_float(buff);, goto err_y_offset;);
  packet.z_offset =
      CMC_ERRB_ABLE(cmc_buff_unpack_float(buff);, goto err_z_offset;);
  packet.particle_data =
      CMC_ERRB_ABLE(cmc_buff_unpack_float(buff);, goto err_particle_data;);
  packet.particle_count =
      CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_particle_count;);
  packet.sable_relative_volume = CMC_ERRB_ABLE(
      cmc_buff_unpack_int(buff);, goto err_sable_relative_volume;);
  return packet;
err_sable_relative_volume:
err_particle_count:
err_particle_data:
err_z_offset:
err_y_offset:
err_x_offset:
err_z:
err_y:
err_x:
err_long_distances:
err_particle_id:
err_d:
err_data:
err_location:
err_effect_id:
  return (cmc_packet_S2C_play_effect_47){0};
}
cmc_packet_S2C_play_sound_effect_47
cmc_packet_S2C_play_sound_effect_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_sound_effect_47 packet = {0};
  packet.sound_name =
      CMC_ERRB_ABLE(cmc_buff_unpack_string(buff);, goto err_sound_name;);
  packet.x = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_x;);
  packet.y = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_y;);
  packet.z = CMC_ERRB_ABLE(cmc_buff_unpack_int(buff);, goto err_z;);
  packet.volume = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff);, goto err_volume;);
  packet.pitch = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_pitch;);
  return packet;
err_pitch:
err_volume:
err_z:
err_y:
err_x:
  cmc_string_free(packet.sound_name);
err_sound_name:
  return (cmc_packet_S2C_play_sound_effect_47){0};
}
cmc_packet_S2C_play_change_game_state_47
cmc_packet_S2C_play_change_game_state_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_change_game_state_47 packet = {0};
  packet.reason = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_reason;);
  packet.value = CMC_ERRB_ABLE(cmc_buff_unpack_float(buff);, goto err_value;);
  return packet;
err_value:
err_reason:
  return (cmc_packet_S2C_play_change_game_state_47){0};
}
cmc_packet_S2C_play_player_abilities_47
cmc_packet_S2C_play_player_abilities_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_player_abilities_47 packet = {0};
  packet.flags = CMC_ERRB_ABLE(cmc_buff_unpack_char(buff);, goto err_flags;);
  packet.flying_speed =
      CMC_ERRB_ABLE(cmc_buff_unpack_float(buff);, goto err_flying_speed;);
  packet.fov_modifier =
      CMC_ERRB_ABLE(cmc_buff_unpack_float(buff);, goto err_fov_modifier;);
  return packet;
err_fov_modifier:
err_flying_speed:
err_flags:
  return (cmc_packet_S2C_play_player_abilities_47){0};
}
cmc_packet_S2C_play_plugin_message_47
cmc_packet_S2C_play_plugin_message_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_plugin_message_47 packet = {0};
  packet.channel =
      CMC_ERRB_ABLE(cmc_buff_unpack_string(buff);, goto err_channel;);
  packet.data = CMC_ERRB_ABLE(cmc_buff_unpack_buff(buff);, goto err_data;);
  return packet;
  cmc_buff_free(packet.data);
err_data:
  cmc_string_free(packet.channel);
err_channel:
  return (cmc_packet_S2C_play_plugin_message_47){0};
}
cmc_packet_S2C_play_disconnect_47
cmc_packet_S2C_play_disconnect_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_disconnect_47 packet = {0};
  packet.reason =
      CMC_ERRB_ABLE(cmc_buff_unpack_string(buff);, goto err_reason;);
  return packet;
  cmc_string_free(packet.reason);
err_reason:
  return (cmc_packet_S2C_play_disconnect_47){0};
}
cmc_packet_S2C_play_change_difficulty_47
cmc_packet_S2C_play_change_difficulty_47_unpack(cmc_buff *buff) {
  cmc_packet_S2C_play_change_difficulty_47 packet = {0};
  packet.difficulty =
      CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff);, goto err_difficulty;);
  return packet;
err_difficulty:
  return (cmc_packet_S2C_play_change_difficulty_47){0};
}
cmc_packet_C2S_play_keep_alive_47
cmc_packet_C2S_play_keep_alive_47_unpack(cmc_buff *buff) {
  cmc_packet_C2S_play_keep_alive_47 packet = {0};
  packet.keep_alive_id =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff);, goto err_keep_alive_id;);
  return packet;
err_keep_alive_id:
  return (cmc_packet_C2S_play_keep_alive_47){0};
}
// CGSE: unpack_methods_c

// CGSS: free_methods_c
void cmc_packet_C2S_handshake_handshake_765_free(
    cmc_packet_C2S_handshake_handshake_765 *packet) {
  cmc_string_free(packet->server_addr);
}
void cmc_packet_S2C_status_response_765_free(
    cmc_packet_S2C_status_response_765 *packet) {
  cmc_string_free(packet->response);
}
void cmc_packet_S2C_login_disconnect_765_free(
    cmc_packet_S2C_login_disconnect_765 *packet) {
  cmc_string_free(packet->reason);
}
void cmc_packet_S2C_login_encryption_request_765_free(
    cmc_packet_S2C_login_encryption_request_765 *packet) {
  cmc_string_free(packet->server_id);
  cmc_buff_free(packet->public_key);
  cmc_buff_free(packet->verify_token);
}
void cmc_packet_S2C_login_success_765_free(
    cmc_packet_S2C_login_success_765 *packet) {
  cmc_string_free(packet->name);
}
void cmc_packet_C2S_login_start_765_free(
    cmc_packet_C2S_login_start_765 *packet) {
  cmc_string_free(packet->name);
}
void cmc_packet_C2S_login_encryption_response_765_free(
    cmc_packet_C2S_login_encryption_response_765 *packet) {
  cmc_buff_free(packet->shared_secret);
  cmc_buff_free(packet->verify_token);
}
void cmc_packet_S2C_config_plugin_message_765_free(
    cmc_packet_S2C_config_plugin_message_765 *packet) {
  cmc_string_free(packet->channel);
  cmc_buff_free(packet->data);
}
void cmc_packet_S2C_config_disconnect_765_free(
    cmc_packet_S2C_config_disconnect_765 *packet) {
  cmc_string_free(packet->reason);
}
void cmc_packet_S2C_config_registry_data_765_free(
    cmc_packet_S2C_config_registry_data_765 *packet) {
  cmc_nbt_free(packet->registry_codec);
}
void cmc_packet_S2C_play_disconnect_765_free(
    cmc_packet_S2C_play_disconnect_765 *packet) {
  cmc_nbt_free(packet->reason_nbt);
}
void cmc_packet_C2S_handshake_handshake_47_free(
    cmc_packet_C2S_handshake_handshake_47 *packet) {
  cmc_string_free(packet->server_addr);
}
void cmc_packet_S2C_status_response_47_free(
    cmc_packet_S2C_status_response_47 *packet) {
  cmc_string_free(packet->response);
}
void cmc_packet_S2C_login_disconnect_47_free(
    cmc_packet_S2C_login_disconnect_47 *packet) {
  cmc_string_free(packet->reason);
}
void cmc_packet_S2C_login_encryption_request_47_free(
    cmc_packet_S2C_login_encryption_request_47 *packet) {
  cmc_string_free(packet->server_id);
  cmc_buff_free(packet->public_key);
  cmc_buff_free(packet->verify_token);
}
void cmc_packet_S2C_login_success_47_free(
    cmc_packet_S2C_login_success_47 *packet) {
  cmc_string_free(packet->uuid_str);
  cmc_string_free(packet->name);
}
void cmc_packet_C2S_login_start_47_free(cmc_packet_C2S_login_start_47 *packet) {
  cmc_string_free(packet->name);
}
void cmc_packet_C2S_login_encryption_response_47_free(
    cmc_packet_C2S_login_encryption_response_47 *packet) {
  cmc_buff_free(packet->shared_secret);
  cmc_buff_free(packet->verify_token);
}
void cmc_packet_S2C_play_join_game_47_free(
    cmc_packet_S2C_play_join_game_47 *packet) {
  cmc_string_free(packet->level_type);
}
void cmc_packet_S2C_play_chat_message_47_free(
    cmc_packet_S2C_play_chat_message_47 *packet) {
  cmc_string_free(packet->message);
}
void cmc_packet_S2C_play_entity_equipment_47_free(
    cmc_packet_S2C_play_entity_equipment_47 *packet) {
  cmc_slot_free(packet->item);
}
void cmc_packet_S2C_play_respawn_47_free(
    cmc_packet_S2C_play_respawn_47 *packet) {
  cmc_string_free(packet->level_type);
}
void cmc_packet_S2C_play_spawn_player_47_free(
    cmc_packet_S2C_play_spawn_player_47 *packet) {
  cmc_entity_metadata_free(packet->meta_data);
}
void cmc_packet_S2C_play_spawn_mob_47_free(
    cmc_packet_S2C_play_spawn_mob_47 *packet) {
  cmc_entity_metadata_free(packet->meta_data);
}
void cmc_packet_S2C_play_spawn_painting_47_free(
    cmc_packet_S2C_play_spawn_painting_47 *packet) {
  cmc_string_free(packet->title);
}
void cmc_packet_S2C_play_entity_metadata_47_free(
    cmc_packet_S2C_play_entity_metadata_47 *packet) {
  cmc_entity_metadata_free(packet->meta_data);
}
void cmc_packet_S2C_play_entity_properties_47_free(
    cmc_packet_S2C_play_entity_properties_47 *packet) {
  if (packet->properties_count > 0) {
    for (int32_t i = 0; i < packet->properties_count; ++i) {
      cmc_string_free(packet->properties[i].key);
      if (packet->properties[i].num_of_modifiers > 0) {
        free(packet->properties[i].modifiers);
      }
    }
    free(packet->properties);
  }
}
void cmc_packet_S2C_play_chunk_data_47_free(
    cmc_packet_S2C_play_chunk_data_47 *packet) {
  cmc_buff_free(packet->chunk);
}
void cmc_packet_S2C_play_multi_block_change_47_free(
    cmc_packet_S2C_play_multi_block_change_47 *packet) {
  if (packet->record_count > 0) {
    free(packet->records);
  }
}
void cmc_packet_S2C_play_map_chunk_bulk_47_free(
    cmc_packet_S2C_play_map_chunk_bulk_47 *packet) {
  if (packet->chunk_column_count > 0) {
    free(packet->chunk_columns);
  }
  cmc_buff_free(packet->chunk);
}
void cmc_packet_S2C_play_explosion_47_free(
    cmc_packet_S2C_play_explosion_47 *packet) {
  if (packet->record_count > 0) {
    free(packet->records);
  }
}
void cmc_packet_S2C_play_sound_effect_47_free(
    cmc_packet_S2C_play_sound_effect_47 *packet) {
  cmc_string_free(packet->sound_name);
}
void cmc_packet_S2C_play_plugin_message_47_free(
    cmc_packet_S2C_play_plugin_message_47 *packet) {
  cmc_string_free(packet->channel);
  cmc_buff_free(packet->data);
}
void cmc_packet_S2C_play_disconnect_47_free(
    cmc_packet_S2C_play_disconnect_47 *packet) {
  cmc_string_free(packet->reason);
}
// CGSE: free_methods_c

// CGSS: pack_methods_c
void cmc_packet_C2S_handshake_handshake_765_pack(
    cmc_buff *buff, cmc_packet_C2S_handshake_handshake_765 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_C2S_handshake_handshake_765_id);
  cmc_buff_pack_varint(buff, packet->protocole_version);
  cmc_buff_pack_string(buff, packet->server_addr);
  cmc_buff_pack_ushort(buff, packet->server_port);
  cmc_buff_pack_varint(buff, packet->next_state);
}
void cmc_packet_S2C_status_response_765_pack(
    cmc_buff *buff, cmc_packet_S2C_status_response_765 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_status_response_765_id);
  cmc_buff_pack_string(buff, packet->response);
}
void cmc_packet_S2C_status_pong_765_pack(
    cmc_buff *buff, cmc_packet_S2C_status_pong_765 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_status_pong_765_id);
  cmc_buff_pack_long(buff, packet->payload);
}
void cmc_packet_C2S_status_request_765_pack(cmc_buff *buff) {
  cmc_buff_pack_varint(buff, cmc_packet_C2S_status_request_765_id);
}
void cmc_packet_C2S_status_ping_765_pack(
    cmc_buff *buff, cmc_packet_C2S_status_ping_765 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_C2S_status_ping_765_id);
  cmc_buff_pack_long(buff, packet->payload);
}
void cmc_packet_S2C_login_disconnect_765_pack(
    cmc_buff *buff, cmc_packet_S2C_login_disconnect_765 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_login_disconnect_765_id);
  cmc_buff_pack_string(buff, packet->reason);
}
void cmc_packet_S2C_login_encryption_request_765_pack(
    cmc_buff *buff, cmc_packet_S2C_login_encryption_request_765 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_login_encryption_request_765_id);
  cmc_buff_pack_string(buff, packet->server_id);
  cmc_buff_pack_buff(buff, packet->public_key);
  cmc_buff_pack_buff(buff, packet->verify_token);
}
void cmc_packet_S2C_login_success_765_pack(
    cmc_buff *buff, cmc_packet_S2C_login_success_765 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_login_success_765_id);
  cmc_buff_pack_uuid(buff, packet->uuid);
  cmc_buff_pack_string(buff, packet->name);
  cmc_buff_pack_varint(buff, packet->properties_count);
}
void cmc_packet_S2C_login_set_compression_765_pack(
    cmc_buff *buff, cmc_packet_S2C_login_set_compression_765 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_login_set_compression_765_id);
  cmc_buff_pack_varint(buff, packet->threshold);
}
void cmc_packet_C2S_login_start_765_pack(
    cmc_buff *buff, cmc_packet_C2S_login_start_765 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_C2S_login_start_765_id);
  cmc_buff_pack_string(buff, packet->name);
  cmc_buff_pack_uuid(buff, packet->uuid);
}
void cmc_packet_C2S_login_encryption_response_765_pack(
    cmc_buff *buff, cmc_packet_C2S_login_encryption_response_765 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_C2S_login_encryption_response_765_id);
  cmc_buff_pack_buff(buff, packet->shared_secret);
  cmc_buff_pack_buff(buff, packet->verify_token);
}
void cmc_packet_C2S_login_acknowledged_765_pack(cmc_buff *buff) {
  cmc_buff_pack_varint(buff, cmc_packet_C2S_login_acknowledged_765_id);
}
void cmc_packet_S2C_config_plugin_message_765_pack(
    cmc_buff *buff, cmc_packet_S2C_config_plugin_message_765 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_config_plugin_message_765_id);
  cmc_buff_pack_string(buff, packet->channel);
  cmc_buff_pack_buff(buff, packet->data);
}
void cmc_packet_S2C_config_disconnect_765_pack(
    cmc_buff *buff, cmc_packet_S2C_config_disconnect_765 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_config_disconnect_765_id);
  cmc_buff_pack_string(buff, packet->reason);
}
void cmc_packet_S2C_config_finish_765_pack(cmc_buff *buff) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_config_finish_765_id);
}
void cmc_packet_S2C_config_keep_alive_765_pack(
    cmc_buff *buff, cmc_packet_S2C_config_keep_alive_765 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_config_keep_alive_765_id);
  cmc_buff_pack_long(buff, packet->keep_alive_id);
}
void cmc_packet_S2C_config_ping_765_pack(
    cmc_buff *buff, cmc_packet_S2C_config_ping_765 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_config_ping_765_id);
  cmc_buff_pack_int(buff, packet->id);
}
void cmc_packet_S2C_config_registry_data_765_pack(
    cmc_buff *buff, cmc_packet_S2C_config_registry_data_765 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_config_registry_data_765_id);
  cmc_buff_pack_nbt(buff, packet->registry_codec);
}
void cmc_packet_S2C_config_remove_resource_pack_765_pack(cmc_buff *buff) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_config_remove_resource_pack_765_id);
}
void cmc_packet_S2C_config_add_resource_pack_765_pack(cmc_buff *buff) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_config_add_resource_pack_765_id);
}
void cmc_packet_C2S_play_keep_alive_765_pack(
    cmc_buff *buff, cmc_packet_C2S_play_keep_alive_765 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_C2S_play_keep_alive_765_id);
  cmc_buff_pack_long(buff, packet->keep_alive_id_long);
}
void cmc_packet_S2C_play_keep_alive_765_pack(
    cmc_buff *buff, cmc_packet_S2C_play_keep_alive_765 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_keep_alive_765_id);
  cmc_buff_pack_long(buff, packet->keep_alive_id_long);
}
void cmc_packet_S2C_play_disconnect_765_pack(
    cmc_buff *buff, cmc_packet_S2C_play_disconnect_765 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_disconnect_765_id);
  cmc_buff_pack_nbt(buff, packet->reason_nbt);
}
void cmc_packet_C2S_handshake_handshake_47_pack(
    cmc_buff *buff, cmc_packet_C2S_handshake_handshake_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_C2S_handshake_handshake_47_id);
  cmc_buff_pack_varint(buff, packet->protocole_version);
  cmc_buff_pack_string(buff, packet->server_addr);
  cmc_buff_pack_ushort(buff, packet->server_port);
  cmc_buff_pack_varint(buff, packet->next_state);
}
void cmc_packet_S2C_status_response_47_pack(
    cmc_buff *buff, cmc_packet_S2C_status_response_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_status_response_47_id);
  cmc_buff_pack_string(buff, packet->response);
}
void cmc_packet_S2C_status_pong_47_pack(cmc_buff *buff,
                                        cmc_packet_S2C_status_pong_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_status_pong_47_id);
  cmc_buff_pack_long(buff, packet->payload);
}
void cmc_packet_C2S_status_request_47_pack(cmc_buff *buff) {
  cmc_buff_pack_varint(buff, cmc_packet_C2S_status_request_47_id);
}
void cmc_packet_C2S_status_ping_47_pack(cmc_buff *buff,
                                        cmc_packet_C2S_status_ping_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_C2S_status_ping_47_id);
  cmc_buff_pack_long(buff, packet->payload);
}
void cmc_packet_S2C_login_disconnect_47_pack(
    cmc_buff *buff, cmc_packet_S2C_login_disconnect_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_login_disconnect_47_id);
  cmc_buff_pack_string(buff, packet->reason);
}
void cmc_packet_S2C_login_encryption_request_47_pack(
    cmc_buff *buff, cmc_packet_S2C_login_encryption_request_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_login_encryption_request_47_id);
  cmc_buff_pack_string(buff, packet->server_id);
  cmc_buff_pack_buff(buff, packet->public_key);
  cmc_buff_pack_buff(buff, packet->verify_token);
}
void cmc_packet_S2C_login_success_47_pack(
    cmc_buff *buff, cmc_packet_S2C_login_success_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_login_success_47_id);
  cmc_buff_pack_string(buff, packet->uuid_str);
  cmc_buff_pack_string(buff, packet->name);
}
void cmc_packet_S2C_login_set_compression_47_pack(
    cmc_buff *buff, cmc_packet_S2C_login_set_compression_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_login_set_compression_47_id);
  cmc_buff_pack_varint(buff, packet->threshold);
}
void cmc_packet_C2S_login_start_47_pack(cmc_buff *buff,
                                        cmc_packet_C2S_login_start_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_C2S_login_start_47_id);
  cmc_buff_pack_string(buff, packet->name);
}
void cmc_packet_C2S_login_encryption_response_47_pack(
    cmc_buff *buff, cmc_packet_C2S_login_encryption_response_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_C2S_login_encryption_response_47_id);
  cmc_buff_pack_buff(buff, packet->shared_secret);
  cmc_buff_pack_buff(buff, packet->verify_token);
}
void cmc_packet_S2C_play_keep_alive_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_keep_alive_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_keep_alive_47_id);
  cmc_buff_pack_varint(buff, packet->keep_alive_id);
}
void cmc_packet_S2C_play_join_game_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_join_game_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_join_game_47_id);
  cmc_buff_pack_int(buff, packet->entity_id);
  cmc_buff_pack_byte(buff, packet->gamemode);
  cmc_buff_pack_char(buff, packet->dimension);
  cmc_buff_pack_byte(buff, packet->difficulty);
  cmc_buff_pack_byte(buff, packet->max_players);
  cmc_buff_pack_string(buff, packet->level_type);
  cmc_buff_pack_bool(buff, packet->reduced_debug_info);
}
void cmc_packet_S2C_play_chat_message_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_chat_message_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_chat_message_47_id);
  cmc_buff_pack_string(buff, packet->message);
  cmc_buff_pack_char(buff, packet->position);
}
void cmc_packet_S2C_play_time_update_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_time_update_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_time_update_47_id);
  cmc_buff_pack_long(buff, packet->world_age);
  cmc_buff_pack_long(buff, packet->time_of_day);
}
void cmc_packet_S2C_play_entity_equipment_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_entity_equipment_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_entity_equipment_47_id);
  cmc_buff_pack_varint(buff, packet->entity_id);
  cmc_buff_pack_short(buff, packet->slot);
  cmc_buff_pack_slot(buff, packet->item);
}
void cmc_packet_S2C_play_spawn_position_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_spawn_position_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_spawn_position_47_id);
  cmc_buff_pack_position(buff, packet->location);
}
void cmc_packet_S2C_play_update_health_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_update_health_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_update_health_47_id);
  cmc_buff_pack_float(buff, packet->health);
  cmc_buff_pack_varint(buff, packet->food);
  cmc_buff_pack_float(buff, packet->food_saturation);
}
void cmc_packet_S2C_play_respawn_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_respawn_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_respawn_47_id);
  cmc_buff_pack_int(buff, packet->dimesion);
  cmc_buff_pack_byte(buff, packet->difficulty);
  cmc_buff_pack_byte(buff, packet->gamemode);
  cmc_buff_pack_string(buff, packet->level_type);
}
void cmc_packet_S2C_play_player_look_and_position_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_player_look_and_position_47 *packet) {
  cmc_buff_pack_varint(buff,
                       cmc_packet_S2C_play_player_look_and_position_47_id);
  cmc_buff_pack_double(buff, packet->x);
  cmc_buff_pack_double(buff, packet->y);
  cmc_buff_pack_double(buff, packet->z);
  cmc_buff_pack_float(buff, packet->yaw);
  cmc_buff_pack_float(buff, packet->pitch);
  cmc_buff_pack_byte(buff, packet->flags);
}
void cmc_packet_S2C_play_held_item_change_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_held_item_change_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_held_item_change_47_id);
  cmc_buff_pack_char(buff, packet->slot);
}
void cmc_packet_S2C_play_use_bed_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_use_bed_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_use_bed_47_id);
  cmc_buff_pack_varint(buff, packet->entity_id);
  cmc_buff_pack_position(buff, packet->location);
}
void cmc_packet_S2C_play_animation_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_animation_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_animation_47_id);
  cmc_buff_pack_varint(buff, packet->entity_id);
  cmc_buff_pack_byte(buff, packet->animation);
}
void cmc_packet_S2C_play_spawn_player_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_spawn_player_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_spawn_player_47_id);
  cmc_buff_pack_varint(buff, packet->entity_id);
  cmc_buff_pack_uuid(buff, packet->uuid);
  cmc_buff_pack_int(buff, packet->x);
  cmc_buff_pack_int(buff, packet->y);
  cmc_buff_pack_int(buff, packet->z);
  cmc_buff_pack_byte(buff, packet->yaw);
  cmc_buff_pack_byte(buff, packet->pitch);
  cmc_buff_pack_short(buff, packet->current_item);
  cmc_buff_pack_entity_metadata(buff, packet->meta_data);
}
void cmc_packet_S2C_play_collect_item_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_collect_item_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_collect_item_47_id);
  cmc_buff_pack_varint(buff, packet->collected_entity_id);
  cmc_buff_pack_varint(buff, packet->collector_entity_id);
}
void cmc_packet_S2C_play_spawn_mob_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_spawn_mob_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_spawn_mob_47_id);
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
}
void cmc_packet_S2C_play_spawn_painting_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_spawn_painting_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_spawn_painting_47_id);
  cmc_buff_pack_varint(buff, packet->entity_id);
  cmc_buff_pack_string(buff, packet->title);
  cmc_buff_pack_position(buff, packet->location);
  cmc_buff_pack_byte(buff, packet->direction);
}
void cmc_packet_S2C_play_spawn_experience_orb_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_spawn_experience_orb_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_spawn_experience_orb_47_id);
  cmc_buff_pack_varint(buff, packet->entity_id);
  cmc_buff_pack_int(buff, packet->x);
  cmc_buff_pack_int(buff, packet->y);
  cmc_buff_pack_int(buff, packet->z);
  cmc_buff_pack_short(buff, packet->count);
}
void cmc_packet_S2C_play_entity_velocity_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_entity_velocity_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_entity_velocity_47_id);
  cmc_buff_pack_varint(buff, packet->entity_id);
  cmc_buff_pack_short(buff, packet->x_vel);
  cmc_buff_pack_short(buff, packet->y_vel);
  cmc_buff_pack_short(buff, packet->z_vel);
}
void cmc_packet_S2C_play_entity_47_pack(cmc_buff *buff,
                                        cmc_packet_S2C_play_entity_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_entity_47_id);
  cmc_buff_pack_varint(buff, packet->entity_id);
}
void cmc_packet_S2C_play_entity_relative_move_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_entity_relative_move_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_entity_relative_move_47_id);
  cmc_buff_pack_varint(buff, packet->entity_id);
  cmc_buff_pack_char(buff, packet->delta_x);
  cmc_buff_pack_char(buff, packet->delta_y);
  cmc_buff_pack_char(buff, packet->delta_z);
  cmc_buff_pack_bool(buff, packet->on_ground);
}
void cmc_packet_S2C_play_entity_look_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_entity_look_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_entity_look_47_id);
  cmc_buff_pack_varint(buff, packet->entity_id);
  cmc_buff_pack_byte(buff, packet->yaw);
  cmc_buff_pack_byte(buff, packet->pitch);
  cmc_buff_pack_bool(buff, packet->on_ground);
}
void cmc_packet_S2C_play_entity_look_and_relative_move_47_pack(
    cmc_buff *buff,
    cmc_packet_S2C_play_entity_look_and_relative_move_47 *packet) {
  cmc_buff_pack_varint(buff,
                       cmc_packet_S2C_play_entity_look_and_relative_move_47_id);
  cmc_buff_pack_varint(buff, packet->entity_id);
  cmc_buff_pack_char(buff, packet->delta_x);
  cmc_buff_pack_char(buff, packet->delta_y);
  cmc_buff_pack_char(buff, packet->delta_z);
  cmc_buff_pack_byte(buff, packet->yaw);
  cmc_buff_pack_byte(buff, packet->pitch);
  cmc_buff_pack_bool(buff, packet->on_ground);
}
void cmc_packet_S2C_play_entity_teleport_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_entity_teleport_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_entity_teleport_47_id);
  cmc_buff_pack_varint(buff, packet->entity_id);
  cmc_buff_pack_int(buff, packet->x);
  cmc_buff_pack_int(buff, packet->y);
  cmc_buff_pack_int(buff, packet->z);
  cmc_buff_pack_byte(buff, packet->yaw);
  cmc_buff_pack_byte(buff, packet->pitch);
  cmc_buff_pack_bool(buff, packet->on_ground);
}
void cmc_packet_S2C_play_entity_head_look_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_entity_head_look_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_entity_head_look_47_id);
  cmc_buff_pack_varint(buff, packet->entity_id);
  cmc_buff_pack_byte(buff, packet->head_yaw);
}
void cmc_packet_S2C_play_entity_status_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_entity_status_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_entity_status_47_id);
  cmc_buff_pack_int(buff, packet->entity_id);
  cmc_buff_pack_char(buff, packet->entity_status);
}
void cmc_packet_S2C_play_attach_entity_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_attach_entity_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_attach_entity_47_id);
  cmc_buff_pack_int(buff, packet->entity_id);
  cmc_buff_pack_int(buff, packet->vehicle_id);
  cmc_buff_pack_bool(buff, packet->leash);
}
void cmc_packet_S2C_play_entity_metadata_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_entity_metadata_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_entity_metadata_47_id);
  cmc_buff_pack_varint(buff, packet->entity_id);
  cmc_buff_pack_entity_metadata(buff, packet->meta_data);
}
void cmc_packet_S2C_play_entity_effect_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_entity_effect_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_entity_effect_47_id);
  cmc_buff_pack_varint(buff, packet->entity_id);
  cmc_buff_pack_char(buff, packet->effect_id);
  cmc_buff_pack_char(buff, packet->amplifier);
  cmc_buff_pack_varint(buff, packet->duration);
  cmc_buff_pack_bool(buff, packet->hide_particles);
}
void cmc_packet_S2C_play_remove_entity_effect_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_remove_entity_effect_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_remove_entity_effect_47_id);
  cmc_buff_pack_varint(buff, packet->entity_id);
  cmc_buff_pack_char(buff, packet->effect_id);
}
void cmc_packet_S2C_play_set_experience_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_set_experience_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_set_experience_47_id);
  cmc_buff_pack_float(buff, packet->experience_bar);
  cmc_buff_pack_varint(buff, packet->level);
  cmc_buff_pack_varint(buff, packet->total_experience);
}
void cmc_packet_S2C_play_entity_properties_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_entity_properties_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_entity_properties_47_id);
  cmc_buff_pack_varint(buff, packet->entity_id);
  cmc_buff_pack_int(buff, packet->properties_count);
  for (int32_t i = 0; i < packet->properties_count; ++i) {
    cmc_buff_pack_string(buff, packet->properties[i].key);
    cmc_buff_pack_double(buff, packet->properties[i].value);
    cmc_buff_pack_varint(buff, packet->properties[i].num_of_modifiers);
    for (int32_t j = 0; j < packet->properties[i].num_of_modifiers; ++j) {
      cmc_buff_pack_double(buff, packet->properties[i].modifiers[j].amount);
      cmc_buff_pack_char(buff, packet->properties[i].modifiers[j].operation);
    }
  }
}
void cmc_packet_S2C_play_chunk_data_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_chunk_data_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_chunk_data_47_id);
  cmc_buff_pack_int(buff, packet->chunk_x);
  cmc_buff_pack_int(buff, packet->chunk_z);
  cmc_buff_pack_bool(buff, packet->ground_up_continuous);
  cmc_buff_pack_ushort(buff, packet->primary_bitmask);
  cmc_buff_pack_buff(buff, packet->chunk);
}
void cmc_packet_S2C_play_multi_block_change_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_multi_block_change_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_multi_block_change_47_id);
  cmc_buff_pack_int(buff, packet->chunk_x);
  cmc_buff_pack_int(buff, packet->chunk_z);
  cmc_buff_pack_varint(buff, packet->record_count);
  for (int32_t i = 0; i < packet->record_count; ++i) {
    cmc_buff_pack_byte(buff, packet->records[i].horizontal_position);
    cmc_buff_pack_byte(buff, packet->records[i].vertical_position);
    cmc_buff_pack_varint(buff, packet->records[i].block_id);
  }
}
void cmc_packet_S2C_play_block_change_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_block_change_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_block_change_47_id);
  cmc_buff_pack_position(buff, packet->location);
  cmc_buff_pack_varint(buff, packet->block_id);
}
void cmc_packet_S2C_play_block_action_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_block_action_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_block_action_47_id);
  cmc_buff_pack_position(buff, packet->location);
  cmc_buff_pack_byte(buff, packet->block_data_1);
  cmc_buff_pack_byte(buff, packet->block_data_2);
  cmc_buff_pack_varint(buff, packet->block_type);
}
void cmc_packet_S2C_play_block_break_animation_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_block_break_animation_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_block_break_animation_47_id);
  cmc_buff_pack_varint(buff, packet->entity_id);
  cmc_buff_pack_position(buff, packet->location);
  cmc_buff_pack_char(buff, packet->destroy_stage);
}
void cmc_packet_S2C_play_map_chunk_bulk_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_map_chunk_bulk_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_map_chunk_bulk_47_id);
  cmc_buff_pack_bool(buff, packet->sky_light_sent);
  cmc_buff_pack_varint(buff, packet->chunk_column_count);
  for (int32_t i = 0; i < packet->chunk_column_count; ++i) {
    cmc_buff_pack_int(buff, packet->chunk_columns[i].chunk_x);
    cmc_buff_pack_int(buff, packet->chunk_columns[i].chunk_z);
    cmc_buff_pack_ushort(buff, packet->chunk_columns[i].bit_mask);
  }
  cmc_buff_pack_buff(buff, packet->chunk);
}
void cmc_packet_S2C_play_explosion_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_explosion_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_explosion_47_id);
  cmc_buff_pack_float(buff, packet->x);
  cmc_buff_pack_float(buff, packet->y);
  cmc_buff_pack_float(buff, packet->z);
  cmc_buff_pack_float(buff, packet->radius);
  cmc_buff_pack_int(buff, packet->record_count);
  for (int32_t i = 0; i < packet->record_count; ++i) {
    cmc_buff_pack_char(buff, packet->records[i].x_offset);
    cmc_buff_pack_char(buff, packet->records[i].y_offset);
    cmc_buff_pack_char(buff, packet->records[i].z_offset);
  }
  cmc_buff_pack_float(buff, packet->x_player_vel);
  cmc_buff_pack_float(buff, packet->y_player_vel);
  cmc_buff_pack_float(buff, packet->z_player_vel);
}
void cmc_packet_S2C_play_effect_47_pack(cmc_buff *buff,
                                        cmc_packet_S2C_play_effect_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_effect_47_id);
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
}
void cmc_packet_S2C_play_sound_effect_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_sound_effect_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_sound_effect_47_id);
  cmc_buff_pack_string(buff, packet->sound_name);
  cmc_buff_pack_int(buff, packet->x);
  cmc_buff_pack_int(buff, packet->y);
  cmc_buff_pack_int(buff, packet->z);
  cmc_buff_pack_float(buff, packet->volume);
  cmc_buff_pack_byte(buff, packet->pitch);
}
void cmc_packet_S2C_play_change_game_state_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_change_game_state_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_change_game_state_47_id);
  cmc_buff_pack_byte(buff, packet->reason);
  cmc_buff_pack_float(buff, packet->value);
}
void cmc_packet_S2C_play_player_abilities_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_player_abilities_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_player_abilities_47_id);
  cmc_buff_pack_char(buff, packet->flags);
  cmc_buff_pack_float(buff, packet->flying_speed);
  cmc_buff_pack_float(buff, packet->fov_modifier);
}
void cmc_packet_S2C_play_plugin_message_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_plugin_message_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_plugin_message_47_id);
  cmc_buff_pack_string(buff, packet->channel);
  cmc_buff_pack_buff(buff, packet->data);
}
void cmc_packet_S2C_play_disconnect_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_disconnect_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_disconnect_47_id);
  cmc_buff_pack_string(buff, packet->reason);
}
void cmc_packet_S2C_play_change_difficulty_47_pack(
    cmc_buff *buff, cmc_packet_S2C_play_change_difficulty_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_S2C_play_change_difficulty_47_id);
  cmc_buff_pack_byte(buff, packet->difficulty);
}
void cmc_packet_C2S_play_keep_alive_47_pack(
    cmc_buff *buff, cmc_packet_C2S_play_keep_alive_47 *packet) {
  cmc_buff_pack_varint(buff, cmc_packet_C2S_play_keep_alive_47_id);
  cmc_buff_pack_varint(buff, packet->keep_alive_id);
}
// CGSE: pack_methods_c
