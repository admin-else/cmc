#pragma once

#include <cmc/buff.h>
#include <cmc/nbt_types.h>

// there are some circular include shenanigans with cmc_conn and packets header
// that the reason for the existence of this file

// CGSS: packet_types
struct cmc_packet_C2S_handshake_handshake_765 {
  int32_t protocole_version;
  char *server_addr;
  uint16_t server_port;
  int32_t next_state;
};
typedef struct cmc_packet_C2S_handshake_handshake_765
    cmc_packet_C2S_handshake_handshake_765;
struct cmc_packet_S2C_status_response_765 {
  char *response;
};
typedef struct cmc_packet_S2C_status_response_765
    cmc_packet_S2C_status_response_765;
struct cmc_packet_S2C_status_pong_765 {
  int64_t payload;
};
typedef struct cmc_packet_S2C_status_pong_765 cmc_packet_S2C_status_pong_765;
struct cmc_packet_C2S_status_ping_765 {
  int64_t payload;
};
typedef struct cmc_packet_C2S_status_ping_765 cmc_packet_C2S_status_ping_765;
struct cmc_packet_S2C_login_disconnect_765 {
  char *reason;
};
typedef struct cmc_packet_S2C_login_disconnect_765
    cmc_packet_S2C_login_disconnect_765;
struct cmc_packet_S2C_login_encryption_request_765 {
  char *server_id;
  cmc_buff *public_key;
  cmc_buff *verify_token;
};
typedef struct cmc_packet_S2C_login_encryption_request_765
    cmc_packet_S2C_login_encryption_request_765;
struct cmc_packet_S2C_login_success_765 {
  cmc_uuid uuid;
  char *name;
  int32_t properties_count;
};
typedef struct cmc_packet_S2C_login_success_765
    cmc_packet_S2C_login_success_765;
struct cmc_packet_S2C_login_set_compression_765 {
  int32_t threshold;
};
typedef struct cmc_packet_S2C_login_set_compression_765
    cmc_packet_S2C_login_set_compression_765;
struct cmc_packet_C2S_login_start_765 {
  char *name;
  cmc_uuid uuid;
};
typedef struct cmc_packet_C2S_login_start_765 cmc_packet_C2S_login_start_765;
struct cmc_packet_C2S_login_encryption_response_765 {
  cmc_buff *shared_secret;
  cmc_buff *verify_token;
};
typedef struct cmc_packet_C2S_login_encryption_response_765
    cmc_packet_C2S_login_encryption_response_765;
struct cmc_packet_S2C_config_plugin_message_765 {
  char *channel;
  cmc_buff *data;
};
typedef struct cmc_packet_S2C_config_plugin_message_765
    cmc_packet_S2C_config_plugin_message_765;
struct cmc_packet_S2C_config_disconnect_765 {
  char *reason;
};
typedef struct cmc_packet_S2C_config_disconnect_765
    cmc_packet_S2C_config_disconnect_765;
struct cmc_packet_S2C_config_keep_alive_765 {
  int64_t keep_alive_id;
};
typedef struct cmc_packet_S2C_config_keep_alive_765
    cmc_packet_S2C_config_keep_alive_765;
struct cmc_packet_S2C_config_ping_765 {
  int32_t id;
};
typedef struct cmc_packet_S2C_config_ping_765 cmc_packet_S2C_config_ping_765;
struct cmc_packet_S2C_config_registry_data_765 {
  cmc_nbt *registry_codec;
};
typedef struct cmc_packet_S2C_config_registry_data_765
    cmc_packet_S2C_config_registry_data_765;
struct cmc_packet_C2S_play_keep_alive_765 {
  int64_t keep_alive_id_long;
};
typedef struct cmc_packet_C2S_play_keep_alive_765
    cmc_packet_C2S_play_keep_alive_765;
struct cmc_packet_S2C_play_keep_alive_765 {
  int64_t keep_alive_id_long;
};
typedef struct cmc_packet_S2C_play_keep_alive_765
    cmc_packet_S2C_play_keep_alive_765;
struct cmc_packet_S2C_play_disconnect_765 {
  cmc_nbt *reason_nbt;
};
typedef struct cmc_packet_S2C_play_disconnect_765
    cmc_packet_S2C_play_disconnect_765;
struct cmc_packet_C2S_handshake_handshake_47 {
  int32_t protocole_version;
  char *server_addr;
  uint16_t server_port;
  int32_t next_state;
};
typedef struct cmc_packet_C2S_handshake_handshake_47
    cmc_packet_C2S_handshake_handshake_47;
struct cmc_packet_S2C_status_response_47 {
  char *response;
};
typedef struct cmc_packet_S2C_status_response_47
    cmc_packet_S2C_status_response_47;
struct cmc_packet_S2C_status_pong_47 {
  int64_t payload;
};
typedef struct cmc_packet_S2C_status_pong_47 cmc_packet_S2C_status_pong_47;
struct cmc_packet_C2S_status_ping_47 {
  int64_t payload;
};
typedef struct cmc_packet_C2S_status_ping_47 cmc_packet_C2S_status_ping_47;
struct cmc_packet_S2C_login_disconnect_47 {
  char *reason;
};
typedef struct cmc_packet_S2C_login_disconnect_47
    cmc_packet_S2C_login_disconnect_47;
struct cmc_packet_S2C_login_encryption_request_47 {
  char *server_id;
  cmc_buff *public_key;
  cmc_buff *verify_token;
};
typedef struct cmc_packet_S2C_login_encryption_request_47
    cmc_packet_S2C_login_encryption_request_47;
struct cmc_packet_S2C_login_success_47 {
  char *uuid_str;
  char *name;
};
typedef struct cmc_packet_S2C_login_success_47 cmc_packet_S2C_login_success_47;
struct cmc_packet_S2C_login_set_compression_47 {
  int32_t threshold;
};
typedef struct cmc_packet_S2C_login_set_compression_47
    cmc_packet_S2C_login_set_compression_47;
struct cmc_packet_C2S_login_start_47 {
  char *name;
};
typedef struct cmc_packet_C2S_login_start_47 cmc_packet_C2S_login_start_47;
struct cmc_packet_C2S_login_encryption_response_47 {
  cmc_buff *shared_secret;
  cmc_buff *verify_token;
};
typedef struct cmc_packet_C2S_login_encryption_response_47
    cmc_packet_C2S_login_encryption_response_47;
struct cmc_packet_S2C_play_keep_alive_47 {
  int32_t keep_alive_id;
};
typedef struct cmc_packet_S2C_play_keep_alive_47
    cmc_packet_S2C_play_keep_alive_47;
struct cmc_packet_S2C_play_join_game_47 {
  int32_t entity_id;
  uint8_t gamemode;
  int8_t dimension;
  uint8_t difficulty;
  uint8_t max_players;
  char *level_type;
  bool reduced_debug_info;
};
typedef struct cmc_packet_S2C_play_join_game_47
    cmc_packet_S2C_play_join_game_47;
struct cmc_packet_S2C_play_chat_message_47 {
  char *message;
  int8_t position;
};
typedef struct cmc_packet_S2C_play_chat_message_47
    cmc_packet_S2C_play_chat_message_47;
struct cmc_packet_S2C_play_time_update_47 {
  int64_t world_age;
  int64_t time_of_day;
};
typedef struct cmc_packet_S2C_play_time_update_47
    cmc_packet_S2C_play_time_update_47;
struct cmc_packet_S2C_play_entity_equipment_47 {
  int32_t entity_id;
  int16_t slot;
  cmc_slot *item;
};
typedef struct cmc_packet_S2C_play_entity_equipment_47
    cmc_packet_S2C_play_entity_equipment_47;
struct cmc_packet_S2C_play_spawn_position_47 {
  cmc_block_pos location;
};
typedef struct cmc_packet_S2C_play_spawn_position_47
    cmc_packet_S2C_play_spawn_position_47;
struct cmc_packet_S2C_play_update_health_47 {
  float health;
  int32_t food;
  float food_saturation;
};
typedef struct cmc_packet_S2C_play_update_health_47
    cmc_packet_S2C_play_update_health_47;
struct cmc_packet_S2C_play_respawn_47 {
  int32_t dimesion;
  uint8_t difficulty;
  uint8_t gamemode;
  char *level_type;
};
typedef struct cmc_packet_S2C_play_respawn_47 cmc_packet_S2C_play_respawn_47;
struct cmc_packet_S2C_play_player_look_and_position_47 {
  double x;
  double y;
  double z;
  float yaw;
  float pitch;
  uint8_t flags;
};
typedef struct cmc_packet_S2C_play_player_look_and_position_47
    cmc_packet_S2C_play_player_look_and_position_47;
struct cmc_packet_S2C_play_held_item_change_47 {
  int8_t slot;
};
typedef struct cmc_packet_S2C_play_held_item_change_47
    cmc_packet_S2C_play_held_item_change_47;
struct cmc_packet_S2C_play_use_bed_47 {
  int32_t entity_id;
  cmc_block_pos location;
};
typedef struct cmc_packet_S2C_play_use_bed_47 cmc_packet_S2C_play_use_bed_47;
struct cmc_packet_S2C_play_animation_47 {
  int32_t entity_id;
  uint8_t animation;
};
typedef struct cmc_packet_S2C_play_animation_47
    cmc_packet_S2C_play_animation_47;
struct cmc_packet_S2C_play_spawn_player_47 {
  int32_t entity_id;
  cmc_uuid uuid;
  int32_t x;
  int32_t y;
  int32_t z;
  uint8_t yaw;
  uint8_t pitch;
  int16_t current_item;
  cmc_entity_metadata meta_data;
};
typedef struct cmc_packet_S2C_play_spawn_player_47
    cmc_packet_S2C_play_spawn_player_47;
struct cmc_packet_S2C_play_collect_item_47 {
  int32_t collected_entity_id;
  int32_t collector_entity_id;
};
typedef struct cmc_packet_S2C_play_collect_item_47
    cmc_packet_S2C_play_collect_item_47;
struct cmc_packet_S2C_play_spawn_mob_47 {
  int32_t entity_id;
  uint8_t type;
  int32_t x;
  int32_t y;
  int32_t z;
  uint8_t yaw;
  uint8_t pitch;
  uint8_t head_pitch;
  int16_t x_vel;
  int16_t y_vel;
  int16_t z_vel;
  cmc_entity_metadata meta_data;
};
typedef struct cmc_packet_S2C_play_spawn_mob_47
    cmc_packet_S2C_play_spawn_mob_47;
struct cmc_packet_S2C_play_spawn_painting_47 {
  int32_t entity_id;
  char *title;
  cmc_block_pos location;
  uint8_t direction;
};
typedef struct cmc_packet_S2C_play_spawn_painting_47
    cmc_packet_S2C_play_spawn_painting_47;
struct cmc_packet_S2C_play_spawn_experience_orb_47 {
  int32_t entity_id;
  int32_t x;
  int32_t y;
  int32_t z;
  int16_t count;
};
typedef struct cmc_packet_S2C_play_spawn_experience_orb_47
    cmc_packet_S2C_play_spawn_experience_orb_47;
struct cmc_packet_S2C_play_entity_velocity_47 {
  int32_t entity_id;
  int16_t x_vel;
  int16_t y_vel;
  int16_t z_vel;
};
typedef struct cmc_packet_S2C_play_entity_velocity_47
    cmc_packet_S2C_play_entity_velocity_47;
struct cmc_packet_S2C_play_entity_47 {
  int32_t entity_id;
};
typedef struct cmc_packet_S2C_play_entity_47 cmc_packet_S2C_play_entity_47;
struct cmc_packet_S2C_play_entity_relative_move_47 {
  int32_t entity_id;
  int8_t delta_x;
  int8_t delta_y;
  int8_t delta_z;
  bool on_ground;
};
typedef struct cmc_packet_S2C_play_entity_relative_move_47
    cmc_packet_S2C_play_entity_relative_move_47;
struct cmc_packet_S2C_play_entity_look_47 {
  int32_t entity_id;
  uint8_t yaw;
  uint8_t pitch;
  bool on_ground;
};
typedef struct cmc_packet_S2C_play_entity_look_47
    cmc_packet_S2C_play_entity_look_47;
struct cmc_packet_S2C_play_entity_look_and_relative_move_47 {
  int32_t entity_id;
  int8_t delta_x;
  int8_t delta_y;
  int8_t delta_z;
  uint8_t yaw;
  uint8_t pitch;
  bool on_ground;
};
typedef struct cmc_packet_S2C_play_entity_look_and_relative_move_47
    cmc_packet_S2C_play_entity_look_and_relative_move_47;
struct cmc_packet_S2C_play_entity_teleport_47 {
  int32_t entity_id;
  int32_t x;
  int32_t y;
  int32_t z;
  uint8_t yaw;
  uint8_t pitch;
  bool on_ground;
};
typedef struct cmc_packet_S2C_play_entity_teleport_47
    cmc_packet_S2C_play_entity_teleport_47;
struct cmc_packet_S2C_play_entity_head_look_47 {
  int32_t entity_id;
  uint8_t head_yaw;
};
typedef struct cmc_packet_S2C_play_entity_head_look_47
    cmc_packet_S2C_play_entity_head_look_47;
struct cmc_packet_S2C_play_entity_status_47 {
  int32_t entity_id;
  int8_t entity_status;
};
typedef struct cmc_packet_S2C_play_entity_status_47
    cmc_packet_S2C_play_entity_status_47;
struct cmc_packet_S2C_play_attach_entity_47 {
  int32_t entity_id;
  int32_t vehicle_id;
  bool leash;
};
typedef struct cmc_packet_S2C_play_attach_entity_47
    cmc_packet_S2C_play_attach_entity_47;
struct cmc_packet_S2C_play_entity_metadata_47 {
  int32_t entity_id;
  cmc_entity_metadata meta_data;
};
typedef struct cmc_packet_S2C_play_entity_metadata_47
    cmc_packet_S2C_play_entity_metadata_47;
struct cmc_packet_S2C_play_entity_effect_47 {
  int32_t entity_id;
  int8_t effect_id;
  int8_t amplifier;
  int32_t duration;
  bool hide_particles;
};
typedef struct cmc_packet_S2C_play_entity_effect_47
    cmc_packet_S2C_play_entity_effect_47;
struct cmc_packet_S2C_play_remove_entity_effect_47 {
  int32_t entity_id;
  int8_t effect_id;
};
typedef struct cmc_packet_S2C_play_remove_entity_effect_47
    cmc_packet_S2C_play_remove_entity_effect_47;
struct cmc_packet_S2C_play_set_experience_47 {
  float experience_bar;
  int32_t level;
  int32_t total_experience;
};
typedef struct cmc_packet_S2C_play_set_experience_47
    cmc_packet_S2C_play_set_experience_47;
struct cmc_packet_S2C_play_entity_properties_47_properties_modifiers {
  double amount;
  int8_t operation;
};
typedef struct cmc_packet_S2C_play_entity_properties_47_properties_modifiers
    cmc_packet_S2C_play_entity_properties_47_properties_modifiers;
struct cmc_packet_S2C_play_entity_properties_47_properties {
  char *key;
  double value;
  int32_t num_of_modifiers;
  cmc_packet_S2C_play_entity_properties_47_properties_modifiers *modifiers;
};
typedef struct cmc_packet_S2C_play_entity_properties_47_properties
    cmc_packet_S2C_play_entity_properties_47_properties;
struct cmc_packet_S2C_play_entity_properties_47 {
  int32_t entity_id;
  int32_t properties_count;
  cmc_packet_S2C_play_entity_properties_47_properties *properties;
};
typedef struct cmc_packet_S2C_play_entity_properties_47
    cmc_packet_S2C_play_entity_properties_47;
struct cmc_packet_S2C_play_chunk_data_47 {
  int32_t chunk_x;
  int32_t chunk_z;
  bool ground_up_continuous;
  uint16_t primary_bitmask;
  cmc_buff *chunk;
};
typedef struct cmc_packet_S2C_play_chunk_data_47
    cmc_packet_S2C_play_chunk_data_47;
struct cmc_packet_S2C_play_multi_block_change_47_records {
  uint8_t horizontal_position;
  uint8_t vertical_position;
  int32_t block_id;
};
typedef struct cmc_packet_S2C_play_multi_block_change_47_records
    cmc_packet_S2C_play_multi_block_change_47_records;
struct cmc_packet_S2C_play_multi_block_change_47 {
  int32_t chunk_x;
  int32_t chunk_z;
  int32_t record_count;
  cmc_packet_S2C_play_multi_block_change_47_records *records;
};
typedef struct cmc_packet_S2C_play_multi_block_change_47
    cmc_packet_S2C_play_multi_block_change_47;
struct cmc_packet_S2C_play_block_change_47 {
  cmc_block_pos location;
  int32_t block_id;
};
typedef struct cmc_packet_S2C_play_block_change_47
    cmc_packet_S2C_play_block_change_47;
struct cmc_packet_S2C_play_block_action_47 {
  cmc_block_pos location;
  uint8_t block_data_1;
  uint8_t block_data_2;
  int32_t block_type;
};
typedef struct cmc_packet_S2C_play_block_action_47
    cmc_packet_S2C_play_block_action_47;
struct cmc_packet_S2C_play_block_break_animation_47 {
  int32_t entity_id;
  cmc_block_pos location;
  int8_t destroy_stage;
};
typedef struct cmc_packet_S2C_play_block_break_animation_47
    cmc_packet_S2C_play_block_break_animation_47;
struct cmc_packet_S2C_play_map_chunk_bulk_47_chunk_columns {
  int32_t chunk_x;
  int32_t chunk_z;
  uint16_t bit_mask;
};
typedef struct cmc_packet_S2C_play_map_chunk_bulk_47_chunk_columns
    cmc_packet_S2C_play_map_chunk_bulk_47_chunk_columns;
struct cmc_packet_S2C_play_map_chunk_bulk_47 {
  bool sky_light_sent;
  int32_t chunk_column_count;
  cmc_packet_S2C_play_map_chunk_bulk_47_chunk_columns *chunk_columns;
  cmc_buff *chunk;
};
typedef struct cmc_packet_S2C_play_map_chunk_bulk_47
    cmc_packet_S2C_play_map_chunk_bulk_47;
struct cmc_packet_S2C_play_explosion_47_records {
  int8_t x_offset;
  int8_t y_offset;
  int8_t z_offset;
};
typedef struct cmc_packet_S2C_play_explosion_47_records
    cmc_packet_S2C_play_explosion_47_records;
struct cmc_packet_S2C_play_explosion_47 {
  float x;
  float y;
  float z;
  float radius;
  int32_t record_count;
  cmc_packet_S2C_play_explosion_47_records *records;
  float x_player_vel;
  float y_player_vel;
  float z_player_vel;
};
typedef struct cmc_packet_S2C_play_explosion_47
    cmc_packet_S2C_play_explosion_47;
struct cmc_packet_S2C_play_effect_47 {
  int32_t effect_id;
  cmc_block_pos location;
  int32_t data;
  bool d;
  int32_t particle_id;
  bool long_distances;
  float x;
  float y;
  float z;
  float x_offset;
  float y_offset;
  float z_offset;
  float particle_data;
  int32_t particle_count;
  int32_t sable_relative_volume;
};
typedef struct cmc_packet_S2C_play_effect_47 cmc_packet_S2C_play_effect_47;
struct cmc_packet_S2C_play_sound_effect_47 {
  char *sound_name;
  int32_t x;
  int32_t y;
  int32_t z;
  float volume;
  uint8_t pitch;
};
typedef struct cmc_packet_S2C_play_sound_effect_47
    cmc_packet_S2C_play_sound_effect_47;
struct cmc_packet_S2C_play_change_game_state_47 {
  uint8_t reason;
  float value;
};
typedef struct cmc_packet_S2C_play_change_game_state_47
    cmc_packet_S2C_play_change_game_state_47;
struct cmc_packet_S2C_play_player_abilities_47 {
  int8_t flags;
  float flying_speed;
  float fov_modifier;
};
typedef struct cmc_packet_S2C_play_player_abilities_47
    cmc_packet_S2C_play_player_abilities_47;
struct cmc_packet_S2C_play_plugin_message_47 {
  char *channel;
  cmc_buff *data;
};
typedef struct cmc_packet_S2C_play_plugin_message_47
    cmc_packet_S2C_play_plugin_message_47;
struct cmc_packet_S2C_play_disconnect_47 {
  char *reason;
};
typedef struct cmc_packet_S2C_play_disconnect_47
    cmc_packet_S2C_play_disconnect_47;
struct cmc_packet_S2C_play_change_difficulty_47 {
  uint8_t difficulty;
};
typedef struct cmc_packet_S2C_play_change_difficulty_47
    cmc_packet_S2C_play_change_difficulty_47;
struct cmc_packet_C2S_play_keep_alive_47 {
  int32_t keep_alive_id;
};
typedef struct cmc_packet_C2S_play_keep_alive_47
    cmc_packet_C2S_play_keep_alive_47;
// CGSE: packet_types

// CGSS: packet_ids
enum cmc_packet_ids_state_C2S_handshake_765 {
  cmc_packet_C2S_handshake_handshake_765_id = 0,
};
typedef enum cmc_packet_ids_state_C2S_handshake_765
    cmc_packet_ids_state_C2S_handshake_765;
enum cmc_packet_ids_state_S2C_status_765 {
  cmc_packet_S2C_status_response_765_id = 0,
  cmc_packet_S2C_status_pong_765_id = 1,
};
typedef enum cmc_packet_ids_state_S2C_status_765
    cmc_packet_ids_state_S2C_status_765;
enum cmc_packet_ids_state_C2S_status_765 {
  cmc_packet_C2S_status_request_765_id = 0,
  cmc_packet_C2S_status_ping_765_id = 1,
};
typedef enum cmc_packet_ids_state_C2S_status_765
    cmc_packet_ids_state_C2S_status_765;
enum cmc_packet_ids_state_S2C_login_765 {
  cmc_packet_S2C_login_disconnect_765_id = 0,
  cmc_packet_S2C_login_encryption_request_765_id = 1,
  cmc_packet_S2C_login_success_765_id = 2,
  cmc_packet_S2C_login_set_compression_765_id = 3,
};
typedef enum cmc_packet_ids_state_S2C_login_765
    cmc_packet_ids_state_S2C_login_765;
enum cmc_packet_ids_state_C2S_login_765 {
  cmc_packet_C2S_login_start_765_id = 0,
  cmc_packet_C2S_login_encryption_response_765_id = 1,
  cmc_packet_C2S_login_acknowledged_765_id = 3,
};
typedef enum cmc_packet_ids_state_C2S_login_765
    cmc_packet_ids_state_C2S_login_765;
enum cmc_packet_ids_state_S2C_config_765 {
  cmc_packet_S2C_config_plugin_message_765_id = 0,
  cmc_packet_S2C_config_disconnect_765_id = 1,
  cmc_packet_S2C_config_finish_765_id = 2,
  cmc_packet_S2C_config_keep_alive_765_id = 3,
  cmc_packet_S2C_config_ping_765_id = 4,
  cmc_packet_S2C_config_registry_data_765_id = 5,
  cmc_packet_S2C_config_remove_resource_pack_765_id = 6,
  cmc_packet_S2C_config_add_resource_pack_765_id = 7,
};
typedef enum cmc_packet_ids_state_S2C_config_765
    cmc_packet_ids_state_S2C_config_765;
enum cmc_packet_ids_state_C2S_play_765 {
  cmc_packet_C2S_play_keep_alive_765_id = 21,
};
typedef enum cmc_packet_ids_state_C2S_play_765
    cmc_packet_ids_state_C2S_play_765;
enum cmc_packet_ids_state_S2C_play_765 {
  cmc_packet_S2C_play_keep_alive_765_id = 36,
  cmc_packet_S2C_play_disconnect_765_id = 27,
};
typedef enum cmc_packet_ids_state_S2C_play_765
    cmc_packet_ids_state_S2C_play_765;
enum cmc_packet_ids_state_C2S_handshake_47 {
  cmc_packet_C2S_handshake_handshake_47_id = 0,
};
typedef enum cmc_packet_ids_state_C2S_handshake_47
    cmc_packet_ids_state_C2S_handshake_47;
enum cmc_packet_ids_state_S2C_status_47 {
  cmc_packet_S2C_status_response_47_id = 0,
  cmc_packet_S2C_status_pong_47_id = 1,
};
typedef enum cmc_packet_ids_state_S2C_status_47
    cmc_packet_ids_state_S2C_status_47;
enum cmc_packet_ids_state_C2S_status_47 {
  cmc_packet_C2S_status_request_47_id = 0,
  cmc_packet_C2S_status_ping_47_id = 1,
};
typedef enum cmc_packet_ids_state_C2S_status_47
    cmc_packet_ids_state_C2S_status_47;
enum cmc_packet_ids_state_S2C_login_47 {
  cmc_packet_S2C_login_disconnect_47_id = 0,
  cmc_packet_S2C_login_encryption_request_47_id = 1,
  cmc_packet_S2C_login_success_47_id = 2,
  cmc_packet_S2C_login_set_compression_47_id = 3,
};
typedef enum cmc_packet_ids_state_S2C_login_47
    cmc_packet_ids_state_S2C_login_47;
enum cmc_packet_ids_state_C2S_login_47 {
  cmc_packet_C2S_login_start_47_id = 0,
  cmc_packet_C2S_login_encryption_response_47_id = 1,
};
typedef enum cmc_packet_ids_state_C2S_login_47
    cmc_packet_ids_state_C2S_login_47;
enum cmc_packet_ids_state_S2C_play_47 {
  cmc_packet_S2C_play_keep_alive_47_id = 0,
  cmc_packet_S2C_play_join_game_47_id = 1,
  cmc_packet_S2C_play_chat_message_47_id = 2,
  cmc_packet_S2C_play_time_update_47_id = 3,
  cmc_packet_S2C_play_entity_equipment_47_id = 4,
  cmc_packet_S2C_play_spawn_position_47_id = 5,
  cmc_packet_S2C_play_update_health_47_id = 6,
  cmc_packet_S2C_play_respawn_47_id = 7,
  cmc_packet_S2C_play_player_look_and_position_47_id = 8,
  cmc_packet_S2C_play_held_item_change_47_id = 9,
  cmc_packet_S2C_play_use_bed_47_id = 10,
  cmc_packet_S2C_play_animation_47_id = 11,
  cmc_packet_S2C_play_spawn_player_47_id = 12,
  cmc_packet_S2C_play_collect_item_47_id = 13,
  cmc_packet_S2C_play_spawn_mob_47_id = 15,
  cmc_packet_S2C_play_spawn_painting_47_id = 16,
  cmc_packet_S2C_play_spawn_experience_orb_47_id = 17,
  cmc_packet_S2C_play_entity_velocity_47_id = 18,
  cmc_packet_S2C_play_entity_47_id = 20,
  cmc_packet_S2C_play_entity_relative_move_47_id = 21,
  cmc_packet_S2C_play_entity_look_47_id = 22,
  cmc_packet_S2C_play_entity_look_and_relative_move_47_id = 23,
  cmc_packet_S2C_play_entity_teleport_47_id = 24,
  cmc_packet_S2C_play_entity_head_look_47_id = 25,
  cmc_packet_S2C_play_entity_status_47_id = 26,
  cmc_packet_S2C_play_attach_entity_47_id = 27,
  cmc_packet_S2C_play_entity_metadata_47_id = 28,
  cmc_packet_S2C_play_entity_effect_47_id = 29,
  cmc_packet_S2C_play_remove_entity_effect_47_id = 30,
  cmc_packet_S2C_play_set_experience_47_id = 31,
  cmc_packet_S2C_play_entity_properties_47_id = 32,
  cmc_packet_S2C_play_chunk_data_47_id = 33,
  cmc_packet_S2C_play_multi_block_change_47_id = 34,
  cmc_packet_S2C_play_block_change_47_id = 35,
  cmc_packet_S2C_play_block_action_47_id = 36,
  cmc_packet_S2C_play_block_break_animation_47_id = 37,
  cmc_packet_S2C_play_map_chunk_bulk_47_id = 38,
  cmc_packet_S2C_play_explosion_47_id = 39,
  cmc_packet_S2C_play_effect_47_id = 40,
  cmc_packet_S2C_play_sound_effect_47_id = 41,
  cmc_packet_S2C_play_change_game_state_47_id = 43,
  cmc_packet_S2C_play_player_abilities_47_id = 57,
  cmc_packet_S2C_play_plugin_message_47_id = 63,
  cmc_packet_S2C_play_disconnect_47_id = 64,
  cmc_packet_S2C_play_change_difficulty_47_id = 65,
};
typedef enum cmc_packet_ids_state_S2C_play_47 cmc_packet_ids_state_S2C_play_47;
enum cmc_packet_ids_state_C2S_play_47 {
  cmc_packet_C2S_play_keep_alive_47_id = 0,
};
typedef enum cmc_packet_ids_state_C2S_play_47 cmc_packet_ids_state_C2S_play_47;
// CGSE: packet_ids
