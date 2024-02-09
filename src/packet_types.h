#pragma once
#include "mctypes.h"

// there are some circular include shenanigans with cmc_conn and packets header
// that the reason for the existence of this file

// CGSS: packet_types
typedef struct {
  varint_t protocole_version;
  char *server_addr;
  unsigned short server_port;
  varint_t next_state;
} C2S_handshake_handshake_packet_t;

typedef struct {
  char *response;
} S2C_status_response_packet_t;

typedef struct {
  long payload;
} S2C_status_pong_packet_t;

typedef struct {
  long payload;
} C2S_status_ping_packet_t;

typedef struct {
  char *reason;
} S2C_login_disconnect_packet_t;

typedef struct {
  char *server_id;
  cmc_buffer *public_key;
  cmc_buffer *verify_token;
} S2C_login_encryption_request_packet_t;

typedef struct {
  char *uuid;
  char *name;
} S2C_login_success_packet_t;

typedef struct {
  varint_t threshold;
} S2C_login_set_compression_packet_t;

typedef struct {
  char *name;
} C2S_login_start_packet_t;

typedef struct {
  cmc_buffer *shared_secret;
  cmc_buffer *verify_token;
} C2S_login_encryption_response_packet_t;

typedef struct {
  varint_t keep_alive_id;
} S2C_play_keep_alive_packet_t;

typedef struct {
  int entity_id;
  unsigned char gamemode;
  char dimension;
  unsigned char difficulty;
  unsigned char max_players;
  char *level_type;
  bool reduced_debug_info;
} S2C_play_join_game_packet_t;

typedef struct {
  char *message;
  char position;
} S2C_play_chat_message_packet_t;

typedef struct {
  long world_age;
  long time_of_day;
} S2C_play_time_update_packet_t;

typedef struct {
  varint_t entity_id;
  short slot;
  slot_t *item;
} S2C_play_entity_equipment_packet_t;

typedef struct {
  block_pos_t location;
} S2C_play_spawn_position_packet_t;

typedef struct {
  float health;
  varint_t food;
  float food_saturation;
} S2C_play_update_health_packet_t;

typedef struct {
  int dimesion;
  unsigned char difficulty;
  unsigned char gamemode;
  char *level_type;
} S2C_play_respawn_packet_t;

typedef struct {
  double x;
  double y;
  double z;
  float yaw;
  float pitch;
  unsigned char flags;
} S2C_play_player_look_and_position_packet_t;

typedef struct {
  char slot;
} S2C_play_held_item_change_packet_t;

typedef struct {
  varint_t entity_id;
  block_pos_t location;
} S2C_play_use_bed_packet_t;

typedef struct {
  varint_t entity_id;
  unsigned char animation;
} S2C_play_animation_packet_t;

typedef struct {
  varint_t entity_id;
  unsigned long long uuid;
  int x;
  int y;
  int z;
  unsigned char yaw;
  unsigned char pitch;
  short current_item;
  entity_metadata_t meta_data;
} S2C_play_spawn_player_packet_t;

typedef struct {
  varint_t collected_entity_id;
  varint_t collector_entity_id;
} S2C_play_collect_item_packet_t;

typedef struct {
  varint_t entity_id;
  unsigned char type;
  int x;
  int y;
  int z;
  unsigned char yaw;
  unsigned char pitch;
  unsigned char head_pitch;
  short x_vel;
  short y_vel;
  short z_vel;
  entity_metadata_t meta_data;
} S2C_play_spawn_mob_packet_t;

typedef struct {
  varint_t entity_id;
  char *title;
  block_pos_t location;
  unsigned char direction;
} S2C_play_spawn_painting_packet_t;

typedef struct {
  varint_t entity_id;
  int x;
  int y;
  int z;
  short count;
} S2C_play_spawn_experience_orb_packet_t;

typedef struct {
  varint_t entity_id;
  short x_vel;
  short y_vel;
  short z_vel;
} S2C_play_entity_velocity_packet_t;

typedef struct {
  varint_t entity_id;
} S2C_play_entity_packet_t;

typedef struct {
  varint_t entity_id;
  char delta_x;
  char delta_y;
  char delta_z;
  bool on_ground;
} S2C_play_entity_relative_move_packet_t;

typedef struct {
  varint_t entity_id;
  unsigned char yaw;
  unsigned char pitch;
  bool on_ground;
} S2C_play_entity_look_packet_t;

typedef struct {
  varint_t entity_id;
  char delta_x;
  char delta_y;
  char delta_z;
  unsigned char yaw;
  unsigned char pitch;
  bool on_ground;
} S2C_play_entity_look_and_relative_move_packet_t;

typedef struct {
  varint_t entity_id;
  int x;
  int y;
  int z;
  unsigned char yaw;
  unsigned char pitch;
  bool on_ground;
} S2C_play_entity_teleport_packet_t;

typedef struct {
  varint_t entity_id;
  unsigned char head_yaw;
} S2C_play_entity_head_look_packet_t;

typedef struct {
  int entity_id;
  char entity_status;
} S2C_play_entity_status_packet_t;

typedef struct {
  int entity_id;
  int vehicle_id;
  bool leash;
} S2C_play_attach_entity_packet_t;

typedef struct {
  varint_t entity_id;
  entity_metadata_t meta_data;
} S2C_play_entity_metadata_packet_t;

typedef struct {
  varint_t entity_id;
  char effect_id;
  char amplifier;
  varint_t duration;
  bool hide_particles;
} S2C_play_entity_effect_packet_t;

typedef struct {
  varint_t entity_id;
  char effect_id;
} S2C_play_remove_entity_effect_packet_t;

typedef struct {
  float experience_bar;
  varint_t level;
  varint_t total_experience;
} S2C_play_set_experience_packet_t;

typedef struct {
  int chunk_x;
  int chunk_z;
  bool ground_up_continuous;
  unsigned short primary_bitmask;
  cmc_buffer *chunk;
} S2C_play_chunk_data_packet_t;

typedef struct {
  block_pos_t location;
  varint_t block_id;
} S2C_play_block_change_packet_t;

typedef struct {
  block_pos_t location;
  unsigned char block_data_1;
  unsigned char block_data_2;
  varint_t block_type;
} S2C_play_block_action_packet_t;

typedef struct {
  varint_t entity_id;
  block_pos_t location;
  char destroy_stage;
} S2C_play_block_break_animation_packet_t;

typedef struct {
  int effect_id;
  block_pos_t location;
  int data;
  bool d;
  int particle_id;
  bool long_distances;
  float x;
  float y;
  float z;
  float x_offset;
  float y_offset;
  float z_offset;
  float particle_data;
  int particle_count;
  int sable_relative_volume;
} S2C_play_effect_packet_t;

typedef struct {
  char *sound_name;
  int x;
  int y;
  int z;
  float volume;
  unsigned char pitch;
} S2C_play_sound_effect_packet_t;

typedef struct {
  unsigned char reason;
  float value;
} S2C_play_change_game_state_packet_t;

typedef struct {
  char flags;
  float flying_speed;
  float fov_modifier;
} S2C_play_player_abilities_packet_t;

typedef struct {
  char *channel;
  cmc_buffer *data;
} S2C_play_plugin_message_packet_t;

typedef struct {
  char *reason;
} S2C_play_disconnect_packet_t;

typedef struct {
  unsigned char difficulty;
} S2C_play_change_difficulty_packet_t;

typedef struct {
  varint_t keep_alive_id;
} C2S_play_keep_alive_packet_t;

// CGSE: packet_types
