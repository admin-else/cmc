#pragma once
#include <cmc/buff.h>
#include <cmc/nbt_types.h>

// there are some circular include shenanigans with cmc_conn and packets header
// that the reason for the existence of this file

// CGSS: packet_types
typedef struct {
  int32_t protocole_version;
  char *server_addr;
  uint16_t server_port;
  int32_t next_state;
} C2S_handshake_handshake_packet;
typedef struct {
  char *response;
} S2C_status_response_packet;
typedef struct {
  int64_t payload;
} S2C_status_pong_packet;
typedef struct {
  int64_t payload;
} C2S_status_ping_packet;
typedef struct {
  char *reason;
} S2C_login_disconnect_packet;
typedef struct {
  cmc_buff *verify_token;
  cmc_buff *public_key;
  char *server_id;
} S2C_login_encryption_request_packet;
typedef struct {
  char *uuid_str;
  int32_t properties_count;
  cmc_uuid uuid;
  char *name;
} S2C_login_success_packet;
typedef struct {
  int32_t threshold;
} S2C_login_set_compression_packet;
typedef struct {
  cmc_uuid uuid;
  char *name;
} C2S_login_start_packet;
typedef struct {
  cmc_buff *verify_token;
  cmc_buff *shared_secret;
} C2S_login_encryption_response_packet;
typedef struct {
  cmc_buff *data;
  char *channel;
} S2C_config_plugin_message_packet;
typedef struct {
  char *reason;
} S2C_config_disconnect_packet;
typedef struct {
  int64_t keep_alive_id;
} S2C_config_keep_alive_packet;
typedef struct {
  int32_t id;
} S2C_config_ping_packet;
typedef struct {
  cmc_nbt *registry_codec;
} S2C_config_registry_data_packet;
typedef struct {
  int32_t keep_alive_id;
  int64_t keep_alive_id_long;
} C2S_play_keep_alive_packet;
typedef struct {
  int32_t keep_alive_id;
  int64_t keep_alive_id_long;
} S2C_play_keep_alive_packet;
typedef struct {
  cmc_nbt *reason_nbt;
  char *reason;
} S2C_play_disconnect_packet;
typedef struct {
  uint8_t max_players;
  uint8_t difficulty;
  char *level_type;
  int8_t dimension;
  bool reduced_debug_info;
  uint8_t gamemode;
  int32_t entity_id;
} S2C_play_join_game_packet;
typedef struct {
  int8_t position;
  char *message;
} S2C_play_chat_message_packet;
typedef struct {
  int64_t world_age;
  int64_t time_of_day;
} S2C_play_time_update_packet;
typedef struct {
  cmc_slot *item;
  int16_t slot;
  int32_t entity_id;
} S2C_play_entity_equipment_packet;
typedef struct {
  cmc_block_pos location;
} S2C_play_spawn_position_packet;
typedef struct {
  int32_t food;
  float food_saturation;
  float health;
} S2C_play_update_health_packet;
typedef struct {
  int32_t dimesion;
  uint8_t difficulty;
  uint8_t gamemode;
  char *level_type;
} S2C_play_respawn_packet;
typedef struct {
  float yaw;
  float pitch;
  double y;
  uint8_t flags;
  double x;
  double z;
} S2C_play_player_look_and_position_packet;
typedef struct {
  int8_t slot;
} S2C_play_held_item_change_packet;
typedef struct {
  cmc_block_pos location;
  int32_t entity_id;
} S2C_play_use_bed_packet;
typedef struct {
  uint8_t animation;
  int32_t entity_id;
} S2C_play_animation_packet;
typedef struct {
  cmc_entity_metadata meta_data;
  uint8_t pitch;
  int32_t y;
  int32_t z;
  cmc_uuid uuid;
  int32_t entity_id;
  uint8_t yaw;
  int16_t current_item;
  int32_t x;
} S2C_play_spawn_player_packet;
typedef struct {
  int32_t collected_entity_id;
  int32_t collector_entity_id;
} S2C_play_collect_item_packet;
typedef struct {
  cmc_entity_metadata meta_data;
  uint8_t pitch;
  int16_t z_vel;
  int32_t y;
  int16_t y_vel;
  int32_t z;
  uint8_t type;
  int32_t entity_id;
  uint8_t yaw;
  int16_t x_vel;
  uint8_t head_pitch;
  int32_t x;
} S2C_play_spawn_mob_packet;
typedef struct {
  cmc_block_pos location;
  char *title;
  int32_t entity_id;
  uint8_t direction;
} S2C_play_spawn_painting_packet;
typedef struct {
  int32_t y;
  int32_t z;
  int32_t entity_id;
  int32_t x;
  int16_t count;
} S2C_play_spawn_experience_orb_packet;
typedef struct {
  int16_t z_vel;
  int32_t entity_id;
  int16_t y_vel;
  int16_t x_vel;
} S2C_play_entity_velocity_packet;
typedef struct {
  int32_t entity_id;
} S2C_play_entity_packet;
typedef struct {
  bool on_ground;
  int8_t delta_y;
  int8_t delta_z;
  int32_t entity_id;
  int8_t delta_x;
} S2C_play_entity_relative_move_packet;
typedef struct {
  uint8_t pitch;
  bool on_ground;
  int32_t entity_id;
  uint8_t yaw;
} S2C_play_entity_look_packet;
typedef struct {
  uint8_t pitch;
  bool on_ground;
  int8_t delta_y;
  int8_t delta_z;
  int32_t entity_id;
  uint8_t yaw;
  int8_t delta_x;
} S2C_play_entity_look_and_relative_move_packet;
typedef struct {
  uint8_t pitch;
  int32_t y;
  bool on_ground;
  int32_t z;
  int32_t entity_id;
  uint8_t yaw;
  int32_t x;
} S2C_play_entity_teleport_packet;
typedef struct {
  uint8_t head_yaw;
  int32_t entity_id;
} S2C_play_entity_head_look_packet;
typedef struct {
  int8_t entity_status;
  int32_t entity_id;
} S2C_play_entity_status_packet;
typedef struct {
  int32_t vehicle_id;
  bool leash;
  int32_t entity_id;
} S2C_play_attach_entity_packet;
typedef struct {
  cmc_entity_metadata meta_data;
  int32_t entity_id;
} S2C_play_entity_metadata_packet;
typedef struct {
  int32_t duration;
  int8_t effect_id;
  int32_t entity_id;
  bool hide_particles;
  int8_t amplifier;
} S2C_play_entity_effect_packet;
typedef struct {
  int8_t effect_id;
  int32_t entity_id;
} S2C_play_remove_entity_effect_packet;
typedef struct {
  int32_t level;
  float experience_bar;
  int32_t total_experience;
} S2C_play_set_experience_packet;
typedef struct {
  double amount;
  int8_t operation;
} S2C_play_entity_properties_modifiers;
typedef struct {
  char *key;
  double value;
  int32_t num_of_modifiers;
  cmc_array modifiers;
} S2C_play_entity_properties_properties;
typedef struct {
  cmc_array properties;
  int32_t properties_count;
  int32_t entity_id;
} S2C_play_entity_properties_packet;
typedef struct {
  bool ground_up_continuous;
  uint16_t primary_bitmask;
  int32_t chunk_z;
  int32_t chunk_x;
  cmc_buff *chunk;
} S2C_play_chunk_data_packet;
typedef struct {
  uint8_t horizontal_position;
  uint8_t vertical_position;
  int32_t block_id;
} S2C_play_multi_block_change_records;
typedef struct {
  int32_t record_count;
  int32_t chunk_z;
  cmc_array records;
  int32_t chunk_x;
} S2C_play_multi_block_change_packet;
typedef struct {
  cmc_block_pos location;
  int32_t block_id;
} S2C_play_block_change_packet;
typedef struct {
  int32_t block_type;
  uint8_t block_data_2;
  cmc_block_pos location;
  uint8_t block_data_1;
} S2C_play_block_action_packet;
typedef struct {
  cmc_block_pos location;
  int8_t destroy_stage;
  int32_t entity_id;
} S2C_play_block_break_animation_packet;
typedef struct {
  int32_t chunk_x;
  int32_t chunk_z;
  uint16_t bit_mask;
} S2C_play_map_chunk_bulk_chunk_columns;
typedef struct {
  int32_t chunk_column_count;
  bool sky_light_sent;
  cmc_array chunk_columns;
  cmc_buff *chunk;
} S2C_play_map_chunk_bulk_packet;
typedef struct {
  int8_t x_offset;
  int8_t y_offset;
  int8_t z_offset;
} S2C_play_explosion_records;
typedef struct {
  float x;
  float y_player_vel;
  int32_t record_count;
  cmc_array records;
  float radius;
  float x_player_vel;
  float z_player_vel;
  float y;
  float z;
} S2C_play_explosion_packet;
typedef struct {
  float y_offset;
  float x;
  bool long_distances;
  int32_t sable_relative_volume;
  cmc_block_pos location;
  float z_offset;
  int32_t effect_id;
  bool d;
  int32_t particle_count;
  float x_offset;
  int32_t particle_id;
  int32_t data;
  float y;
  float particle_data;
  float z;
} S2C_play_effect_packet;
typedef struct {
  uint8_t pitch;
  char *sound_name;
  int32_t y;
  float volume;
  int32_t z;
  int32_t x;
} S2C_play_sound_effect_packet;
typedef struct {
  uint8_t reason;
  float value;
} S2C_play_change_game_state_packet;
typedef struct {
  float fov_modifier;
  int8_t flags;
  float flying_speed;
} S2C_play_player_abilities_packet;
typedef struct {
  cmc_buff *data;
  char *channel;
} S2C_play_plugin_message_packet;
typedef struct {
  uint8_t difficulty;
} S2C_play_change_difficulty_packet;
// CGSE: packet_types
