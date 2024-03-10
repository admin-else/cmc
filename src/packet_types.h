#pragma once
#include "array.h"
#include "buffer.h"

// there are some circular include shenanigans with cmc_conn and packets header
// that the reason for the existence of this file

// CGSS: packet_types
typedef struct {
  uint16_t server_port;
  int32_t protocole_version;
  int32_t next_state;
  char *server_addr;
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
  cmc_buffer *public_key;
  char *server_id;
  cmc_buffer *verify_token;
} S2C_login_encryption_request_packet;

typedef struct {
  cmc_uuid uuid;
  char *name;
  char *uuid_str;
} S2C_login_success_packet;

typedef struct {
  int32_t threshold;
} S2C_login_set_compression_packet;

typedef struct {
  cmc_uuid uuid;
  char *name;
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
  int32_t keep_alive_id;
} S2C_play_keep_alive_packet;

typedef struct {
  int8_t dimension;
  int32_t entity_id;
  char *level_type;
  uint8_t difficulty;
  uint8_t gamemode;
  uint8_t max_players;
  bool reduced_debug_info;
} S2C_play_join_game_packet;

typedef struct {
  char *message;
  int8_t position;
} S2C_play_chat_message_packet;

typedef struct {
  int64_t time_of_day;
  int64_t world_age;
} S2C_play_time_update_packet;

typedef struct {
  cmc_slot *item;
  int32_t entity_id;
  int16_t slot;
} S2C_play_entity_equipment_packet;

typedef struct {
  cmc_block_pos location;
} S2C_play_spawn_position_packet;

typedef struct {
  float health;
  float food_saturation;
  int32_t food;
} S2C_play_update_health_packet;

typedef struct {
  uint8_t difficulty;
  char *level_type;
  uint8_t gamemode;
  int32_t dimesion;
} S2C_play_respawn_packet;

typedef struct {
  double x;
  float yaw;
  float pitch;
  double z;
  uint8_t flags;
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
  int32_t entity_id;
  int32_t x;
  uint8_t yaw;
  int32_t z;
  int32_t y;
  int16_t current_item;
  cmc_uuid uuid;
  uint8_t pitch;
  cmc_entity_metadata meta_data;
} S2C_play_spawn_player_packet;

typedef struct {
  int32_t collector_entity_id;
  int32_t collected_entity_id;
} S2C_play_collect_item_packet;

typedef struct {
  int16_t z_vel;
  uint8_t head_pitch;
  int32_t entity_id;
  int32_t x;
  uint8_t type;
  uint8_t yaw;
  cmc_entity_metadata meta_data;
  int16_t x_vel;
  int32_t z;
  int32_t y;
  uint8_t pitch;
  int16_t y_vel;
} S2C_play_spawn_mob_packet;

typedef struct {
  uint8_t direction;
  cmc_block_pos location;
  int32_t entity_id;
  char *title;
} S2C_play_spawn_painting_packet;

typedef struct {
  int32_t entity_id;
  int32_t x;
  int16_t count;
  int32_t z;
  int32_t y;
} S2C_play_spawn_experience_orb_packet;

typedef struct {
  int16_t x_vel;
  int32_t entity_id;
  int16_t y_vel;
  int16_t z_vel;
} S2C_play_entity_velocity_packet;

typedef struct {
  int32_t entity_id;
} S2C_play_entity_packet;

typedef struct {
  int32_t entity_id;
  bool on_ground;
  int8_t delta_y;
  int8_t delta_x;
  int8_t delta_z;
} S2C_play_entity_relative_move_packet;

typedef struct {
  uint8_t pitch;
  uint8_t yaw;
  int32_t entity_id;
  bool on_ground;
} S2C_play_entity_look_packet;

typedef struct {
  int32_t entity_id;
  bool on_ground;
  uint8_t yaw;
  int8_t delta_y;
  int8_t delta_x;
  uint8_t pitch;
  int8_t delta_z;
} S2C_play_entity_look_and_relative_move_packet;

typedef struct {
  int32_t entity_id;
  int32_t x;
  bool on_ground;
  uint8_t yaw;
  int32_t z;
  int32_t y;
  uint8_t pitch;
} S2C_play_entity_teleport_packet;

typedef struct {
  int32_t entity_id;
  uint8_t head_yaw;
} S2C_play_entity_head_look_packet;

typedef struct {
  int8_t entity_status;
  int32_t entity_id;
} S2C_play_entity_status_packet;

typedef struct {
  bool leash;
  int32_t vehicle_id;
  int32_t entity_id;
} S2C_play_attach_entity_packet;

typedef struct {
  int32_t entity_id;
  cmc_entity_metadata meta_data;
} S2C_play_entity_metadata_packet;

typedef struct {
  int8_t effect_id;
  int32_t entity_id;
  int8_t amplifier;
  bool hide_particles;
  int32_t duration;
} S2C_play_entity_effect_packet;

typedef struct {
  int8_t effect_id;
  int32_t entity_id;
} S2C_play_remove_entity_effect_packet;

typedef struct {
  float experience_bar;
  int32_t level;
  int32_t total_experience;
} S2C_play_set_experience_packet;

typedef struct {
  int32_t entity_id;
  cmc_array niga[skey; dvalue; vnum_of_modifiers; Anum_of_modifiers
                                 [damount; boperation] modifiers] properties;
  int32_t properties_count;
} S2C_play_entity_properties_packet;

typedef struct {
  cmc_buffer *chunk;
  int32_t chunk_z;
  uint16_t primary_bitmask;
  bool ground_up_continuous;
  int32_t chunk_x;
} S2C_play_chunk_data_packet;

typedef struct {
  cmc_block_pos location;
  int32_t block_id;
} S2C_play_block_change_packet;

typedef struct {
  uint8_t block_data_2;
  int32_t block_type;
  cmc_block_pos location;
  uint8_t block_data_1;
} S2C_play_block_action_packet;

typedef struct {
  cmc_block_pos location;
  int8_t destroy_stage;
  int32_t entity_id;
} S2C_play_block_break_animation_packet;

typedef struct {
  float y_offset;
  bool d;
  float x;
  int32_t particle_count;
  int32_t sable_relative_volume;
  cmc_block_pos location;
  int32_t effect_id;
  float y;
  float x_offset;
  float particle_data;
  int32_t data;
  float z_offset;
  float z;
  bool long_distances;
  int32_t particle_id;
} S2C_play_effect_packet;

typedef struct {
  float volume;
  int32_t x;
  int32_t z;
  int32_t y;
  uint8_t pitch;
  char *sound_name;
} S2C_play_sound_effect_packet;

typedef struct {
  float value;
  uint8_t reason;
} S2C_play_change_game_state_packet;

typedef struct {
  float fov_modifier;
  int8_t flags;
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

typedef struct {
  int32_t keep_alive_id;
} C2S_play_keep_alive_packet;

// CGSE: packet_types
