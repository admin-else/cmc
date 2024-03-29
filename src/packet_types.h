#pragma once
#include "buffer.h"
#include "nbt_types.h"

// there are some circular include shenanigans with cmc_conn and packets header
// that the reason for the existence of this file

// CGSS: packet_types
typedef struct {
  char *server_addr;
  uint16_t server_port;
  int32_t protocole_version;
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
  char *server_id;
  cmc_buffer *public_key;
  cmc_buffer *verify_token;
} S2C_login_encryption_request_packet;
typedef struct {
  char *name;
  char *uuid_str;
  cmc_uuid uuid;
  int32_t properties_count;
} S2C_login_success_packet;
typedef struct {
  int32_t threshold;
} S2C_login_set_compression_packet;
typedef struct {
  char *name;
  cmc_uuid uuid;
} C2S_login_start_packet;
typedef struct {
  cmc_buffer *verify_token;
  cmc_buffer *shared_secret;
} C2S_login_encryption_response_packet;
typedef struct {
  cmc_buffer *data;
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
  int64_t keep_alive_id_long;
  int32_t keep_alive_id;
} C2S_play_keep_alive_packet;
typedef struct {
  int64_t keep_alive_id_long;
  int32_t keep_alive_id;
} S2C_play_keep_alive_packet;
typedef struct {
  int8_t dimension;
  char *level_type;
  uint8_t difficulty;
  uint8_t max_players;
  bool reduced_debug_info;
  int32_t entity_id;
  uint8_t gamemode;
} S2C_play_join_game_packet;
typedef struct {
  int8_t position;
  char *message;
} S2C_play_chat_message_packet;
typedef struct {
  int64_t time_of_day;
  int64_t world_age;
} S2C_play_time_update_packet;
typedef struct {
  int16_t slot;
  cmc_slot *item;
  int32_t entity_id;
} S2C_play_entity_equipment_packet;
typedef struct {
  cmc_block_pos location;
} S2C_play_spawn_position_packet;
typedef struct {
  float health;
  int32_t food;
  float food_saturation;
} S2C_play_update_health_packet;
typedef struct {
  int32_t dimesion;
  uint8_t difficulty;
  char *level_type;
  uint8_t gamemode;
} S2C_play_respawn_packet;
typedef struct {
  float yaw;
  double x;
  float pitch;
  uint8_t flags;
  double z;
  double y;
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
  int32_t y;
  cmc_uuid uuid;
  int16_t current_item;
  int32_t entity_id;
  int32_t x;
  cmc_entity_metadata meta_data;
  uint8_t pitch;
  uint8_t yaw;
  int32_t z;
} S2C_play_spawn_player_packet;
typedef struct {
  int32_t collected_entity_id;
  int32_t collector_entity_id;
} S2C_play_collect_item_packet;
typedef struct {
  int32_t y;
  uint8_t head_pitch;
  uint8_t type;
  int16_t x_vel;
  int32_t entity_id;
  int32_t x;
  cmc_entity_metadata meta_data;
  uint8_t pitch;
  int16_t y_vel;
  int16_t z_vel;
  uint8_t yaw;
  int32_t z;
} S2C_play_spawn_mob_packet;
typedef struct {
  uint8_t direction;
  cmc_block_pos location;
  char *title;
  int32_t entity_id;
} S2C_play_spawn_painting_packet;
typedef struct {
  int32_t y;
  int32_t entity_id;
  int32_t x;
  int16_t count;
  int32_t z;
} S2C_play_spawn_experience_orb_packet;
typedef struct {
  int16_t y_vel;
  int16_t z_vel;
  int16_t x_vel;
  int32_t entity_id;
} S2C_play_entity_velocity_packet;
typedef struct {
  int32_t entity_id;
} S2C_play_entity_packet;
typedef struct {
  int32_t entity_id;
  bool on_ground;
  int8_t delta_z;
  int8_t delta_y;
  int8_t delta_x;
} S2C_play_entity_relative_move_packet;
typedef struct {
  bool on_ground;
  uint8_t pitch;
  uint8_t yaw;
  int32_t entity_id;
} S2C_play_entity_look_packet;
typedef struct {
  int32_t entity_id;
  bool on_ground;
  int8_t delta_z;
  int8_t delta_y;
  uint8_t pitch;
  int8_t delta_x;
  uint8_t yaw;
} S2C_play_entity_look_and_relative_move_packet;
typedef struct {
  int32_t y;
  int32_t entity_id;
  bool on_ground;
  int32_t x;
  uint8_t pitch;
  uint8_t yaw;
  int32_t z;
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
  bool leash;
  int32_t entity_id;
  int32_t vehicle_id;
} S2C_play_attach_entity_packet;
typedef struct {
  cmc_entity_metadata meta_data;
  int32_t entity_id;
} S2C_play_entity_metadata_packet;
typedef struct {
  int32_t entity_id;
  int32_t duration;
  bool hide_particles;
  int8_t effect_id;
  int8_t amplifier;
} S2C_play_entity_effect_packet;
typedef struct {
  int8_t effect_id;
  int32_t entity_id;
} S2C_play_remove_entity_effect_packet;
typedef struct {
  int32_t total_experience;
  int32_t level;
  float experience_bar;
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
  cmc_buffer *chunk;
  int32_t chunk_x;
  uint16_t primary_bitmask;
  int32_t chunk_z;
} S2C_play_chunk_data_packet;
typedef struct {
  uint8_t horizontal_position;
  uint8_t vertical_position;
  int32_t block_id;
} S2C_play_multi_block_change_records;
typedef struct {
  cmc_array records;
  int32_t record_count;
  int32_t chunk_x;
  int32_t chunk_z;
} S2C_play_multi_block_change_packet;
typedef struct {
  cmc_block_pos location;
  int32_t block_id;
} S2C_play_block_change_packet;
typedef struct {
  cmc_block_pos location;
  uint8_t block_data_1;
  uint8_t block_data_2;
  int32_t block_type;
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
  bool sky_light_sent;
  cmc_buffer *chunk;
  int32_t chunk_column_count;
  cmc_array chunk_columns;
} S2C_play_map_chunk_bulk_packet;
typedef struct {
  int8_t x_offset;
  int8_t y_offset;
  int8_t z_offset;
} S2C_play_explosion_records;
typedef struct {
  float y;
  float x;
  float radius;
  cmc_array records;
  float x_player_vel;
  float z;
  float z_player_vel;
  int32_t record_count;
  float y_player_vel;
} S2C_play_explosion_packet;
typedef struct {
  float y;
  float y_offset;
  float x;
  int32_t sable_relative_volume;
  cmc_block_pos location;
  float x_offset;
  bool d;
  float z_offset;
  int32_t data;
  bool long_distances;
  int32_t particle_id;
  float particle_data;
  float z;
  int32_t effect_id;
  int32_t particle_count;
} S2C_play_effect_packet;
typedef struct {
  int32_t y;
  char *sound_name;
  int32_t x;
  uint8_t pitch;
  float volume;
  int32_t z;
} S2C_play_sound_effect_packet;
typedef struct {
  float value;
  uint8_t reason;
} S2C_play_change_game_state_packet;
typedef struct {
  int8_t flags;
  float fov_modifier;
  float flying_speed;
} S2C_play_player_abilities_packet;
typedef struct {
  cmc_buffer *data;
  char *channel;
} S2C_play_plugin_message_packet;
typedef struct {
  char *reason;
} S2C_play_disconnect_packet;
typedef struct {
  uint8_t difficulty;
} S2C_play_change_difficulty_packet;
// CGSE: packet_types
