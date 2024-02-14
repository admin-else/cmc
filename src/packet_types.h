#pragma once
#include "buffer.h"

// there are some circular include shenanigans with cmc_conn and packets header
// that the reason for the existence of this file

// CGSS: packet_types
typedef struct {
  int protocole_version;
  char *server_addr;
  unsigned short server_port;
  int next_state;
} C2S_handshake_handshake_packet;

typedef struct {
  char *response;
} S2C_status_response_packet;

typedef struct {
  long payload;
} S2C_status_pong_packet;

typedef struct {
  long payload;
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
  char *uuid;
  char *name;
} S2C_login_success_packet;

typedef struct {
  int threshold;
} S2C_login_set_compression_packet;

typedef struct {
  char *name;
} C2S_login_start_packet;

typedef struct {
  cmc_buffer *shared_secret;
  cmc_buffer *verify_token;
} C2S_login_encryption_response_packet;

typedef struct {
  int keep_alive_id;
} S2C_play_keep_alive_packet;

typedef struct {
  int entity_id;
  unsigned char gamemode;
  char dimension;
  unsigned char difficulty;
  unsigned char max_players;
  char *level_type;
  bool reduced_debug_info;
} S2C_play_join_game_packet;

typedef struct {
  char *message;
  char position;
} S2C_play_chat_message_packet;

typedef struct {
  long world_age;
  long time_of_day;
} S2C_play_time_update_packet;

typedef struct {
  int entity_id;
  short slot;
  cmc_slot *item;
} S2C_play_entity_equipment_packet;

typedef struct {
  cmc_block_pos location;
} S2C_play_spawn_position_packet;

typedef struct {
  float health;
  int food;
  float food_saturation;
} S2C_play_update_health_packet;

typedef struct {
  int dimesion;
  unsigned char difficulty;
  unsigned char gamemode;
  char *level_type;
} S2C_play_respawn_packet;

typedef struct {
  double x;
  double y;
  double z;
  float yaw;
  float pitch;
  unsigned char flags;
} S2C_play_player_look_and_position_packet;

typedef struct {
  char slot;
} S2C_play_held_item_change_packet;

typedef struct {
  int entity_id;
  cmc_block_pos location;
} S2C_play_use_bed_packet;

typedef struct {
  int entity_id;
  unsigned char animation;
} S2C_play_animation_packet;

typedef struct {
  int entity_id;
  unsigned long long uuid;
  int x;
  int y;
  int z;
  unsigned char yaw;
  unsigned char pitch;
  short current_item;
  cmc_entity_metadata meta_data;
} S2C_play_spawn_player_packet;

typedef struct {
  int collected_entity_id;
  int collector_entity_id;
} S2C_play_collect_item_packet;

typedef struct {
  int entity_id;
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
  cmc_entity_metadata meta_data;
} S2C_play_spawn_mob_packet;

typedef struct {
  int entity_id;
  char *title;
  cmc_block_pos location;
  unsigned char direction;
} S2C_play_spawn_painting_packet;

typedef struct {
  int entity_id;
  int x;
  int y;
  int z;
  short count;
} S2C_play_spawn_experience_orb_packet;

typedef struct {
  int entity_id;
  short x_vel;
  short y_vel;
  short z_vel;
} S2C_play_entity_velocity_packet;

typedef struct {
  int entity_id;
} S2C_play_entity_packet;

typedef struct {
  int entity_id;
  char delta_x;
  char delta_y;
  char delta_z;
  bool on_ground;
} S2C_play_entity_relative_move_packet;

typedef struct {
  int entity_id;
  unsigned char yaw;
  unsigned char pitch;
  bool on_ground;
} S2C_play_entity_look_packet;

typedef struct {
  int entity_id;
  char delta_x;
  char delta_y;
  char delta_z;
  unsigned char yaw;
  unsigned char pitch;
  bool on_ground;
} S2C_play_entity_look_and_relative_move_packet;

typedef struct {
  int entity_id;
  int x;
  int y;
  int z;
  unsigned char yaw;
  unsigned char pitch;
  bool on_ground;
} S2C_play_entity_teleport_packet;

typedef struct {
  int entity_id;
  unsigned char head_yaw;
} S2C_play_entity_head_look_packet;

typedef struct {
  int entity_id;
  char entity_status;
} S2C_play_entity_status_packet;

typedef struct {
  int entity_id;
  int vehicle_id;
  bool leash;
} S2C_play_attach_entity_packet;

typedef struct {
  int entity_id;
  cmc_entity_metadata meta_data;
} S2C_play_entity_metadata_packet;

typedef struct {
  int entity_id;
  char effect_id;
  char amplifier;
  int duration;
  bool hide_particles;
} S2C_play_entity_effect_packet;

typedef struct {
  int entity_id;
  char effect_id;
} S2C_play_remove_entity_effect_packet;

typedef struct {
  float experience_bar;
  int level;
  int total_experience;
} S2C_play_set_experience_packet;

typedef struct {
  int chunk_x;
  int chunk_z;
  bool ground_up_continuous;
  unsigned short primary_bitmask;
  cmc_buffer *chunk;
} S2C_play_chunk_data_packet;

typedef struct {
  cmc_block_pos location;
  int block_id;
} S2C_play_block_change_packet;

typedef struct {
  cmc_block_pos location;
  unsigned char block_data_1;
  unsigned char block_data_2;
  int block_type;
} S2C_play_block_action_packet;

typedef struct {
  int entity_id;
  cmc_block_pos location;
  char destroy_stage;
} S2C_play_block_break_animation_packet;

typedef struct {
  int effect_id;
  cmc_block_pos location;
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
} S2C_play_effect_packet;

typedef struct {
  char *sound_name;
  int x;
  int y;
  int z;
  float volume;
  unsigned char pitch;
} S2C_play_sound_effect_packet;

typedef struct {
  unsigned char reason;
  float value;
} S2C_play_change_game_state_packet;

typedef struct {
  char flags;
  float flying_speed;
  float fov_modifier;
} S2C_play_player_abilities_packet;

typedef struct {
  char *channel;
  cmc_buffer *data;
} S2C_play_plugin_message_packet;

typedef struct {
  char *reason;
} S2C_play_disconnect_packet;

typedef struct {
  unsigned char difficulty;
} S2C_play_change_difficulty_packet;

typedef struct {
  int keep_alive_id;
} C2S_play_keep_alive_packet;

// CGSE: packet_types
