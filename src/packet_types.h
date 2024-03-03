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
  cmc_buffer *public_key;
  cmc_buffer *verify_token;
  char *server_id;
} S2C_login_encryption_request_packet;

typedef struct {
  char *name;
  char *uuid;
} S2C_login_success_packet;

typedef struct {
  int threshold;
} S2C_login_set_compression_packet;

typedef struct {
  char *name;
} C2S_login_start_packet;

typedef struct {
  cmc_buffer *verify_token;
  cmc_buffer *shared_secret;
} C2S_login_encryption_response_packet;

typedef struct {
  int keep_alive_id;
} S2C_play_keep_alive_packet;

typedef struct {
  bool reduced_debug_info;
  unsigned char difficulty;
  char dimension;
  unsigned char max_players;
  unsigned char gamemode;
  int entity_id;
  char *level_type;
} S2C_play_join_game_packet;

typedef struct {
  char position;
  char *message;
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
  unsigned char gamemode;
  int dimesion;
  char *level_type;
  unsigned char difficulty;
} S2C_play_respawn_packet;

typedef struct {
  float yaw;
  unsigned char flags;
  float pitch;
  double x;
  double z;
  double y;
} S2C_play_player_look_and_position_packet;

typedef struct {
  char slot;
} S2C_play_held_item_change_packet;

typedef struct {
  int entity_id;
  cmc_block_pos location;
} S2C_play_use_bed_packet;

typedef struct {
  unsigned char animation;
  int entity_id;
} S2C_play_animation_packet;

typedef struct {
  int entity_id;
  unsigned char pitch;
  int y;
  int z;
  short current_item;
  int x;
  cmc_entity_metadata meta_data;
  unsigned char yaw;
  unsigned long long uuid;
} S2C_play_spawn_player_packet;

typedef struct {
  int collected_entity_id;
  int collector_entity_id;
} S2C_play_collect_item_packet;

typedef struct {
  int entity_id;
  unsigned char pitch;
  int y;
  int z;
  short y_vel;
  short z_vel;
  int x;
  cmc_entity_metadata meta_data;
  unsigned char head_pitch;
  short x_vel;
  unsigned char type;
  unsigned char yaw;
} S2C_play_spawn_mob_packet;

typedef struct {
  int entity_id;
  cmc_block_pos location;
  unsigned char direction;
  char *title;
} S2C_play_spawn_painting_packet;

typedef struct {
  int entity_id;
  short count;
  int y;
  int z;
  int x;
} S2C_play_spawn_experience_orb_packet;

typedef struct {
  short z_vel;
  int entity_id;
  short y_vel;
  short x_vel;
} S2C_play_entity_velocity_packet;

typedef struct {
  int entity_id;
} S2C_play_entity_packet;

typedef struct {
  int entity_id;
  char delta_y;
  char delta_z;
  bool on_ground;
  char delta_x;
} S2C_play_entity_relative_move_packet;

typedef struct {
  int entity_id;
  unsigned char yaw;
  bool on_ground;
  unsigned char pitch;
} S2C_play_entity_look_packet;

typedef struct {
  int entity_id;
  unsigned char pitch;
  char delta_y;
  char delta_z;
  bool on_ground;
  char delta_x;
  unsigned char yaw;
} S2C_play_entity_look_and_relative_move_packet;

typedef struct {
  int entity_id;
  unsigned char pitch;
  int y;
  int z;
  bool on_ground;
  int x;
  unsigned char yaw;
} S2C_play_entity_teleport_packet;

typedef struct {
  int entity_id;
  unsigned char head_yaw;
} S2C_play_entity_head_look_packet;

typedef struct {
  char entity_status;
  int entity_id;
} S2C_play_entity_status_packet;

typedef struct {
  int vehicle_id;
  int entity_id;
  bool leash;
} S2C_play_attach_entity_packet;

typedef struct {
  int entity_id;
  cmc_entity_metadata meta_data;
} S2C_play_entity_metadata_packet;

typedef struct {
  int entity_id;
  int duration;
  char effect_id;
  char amplifier;
  bool hide_particles;
} S2C_play_entity_effect_packet;

typedef struct {
  char effect_id;
  int entity_id;
} S2C_play_remove_entity_effect_packet;

typedef struct {
  float experience_bar;
  int total_experience;
  int level;
} S2C_play_set_experience_packet;

typedef struct {
  bool ground_up_continuous;
  unsigned short primary_bitmask;
  cmc_buffer *chunk;
  int chunk_x;
  int chunk_z;
} S2C_play_chunk_data_packet;

typedef struct {
  cmc_block_pos location;
  int block_id;
} S2C_play_block_change_packet;

typedef struct {
  cmc_block_pos location;
  int block_type;
  unsigned char block_data_1;
  unsigned char block_data_2;
} S2C_play_block_action_packet;

typedef struct {
  char destroy_stage;
  int entity_id;
  cmc_block_pos location;
} S2C_play_block_break_animation_packet;

typedef struct {
  int particle_id;
  cmc_block_pos location;
  float z;
  float y_offset;
  float y;
  float x;
  float particle_data;
  bool d;
  int data;
  float x_offset;
  int effect_id;
  float z_offset;
  int sable_relative_volume;
  int particle_count;
  bool long_distances;
} S2C_play_effect_packet;

typedef struct {
  char *sound_name;
  unsigned char pitch;
  int y;
  int z;
  int x;
  float volume;
} S2C_play_sound_effect_packet;

typedef struct {
  float value;
  unsigned char reason;
} S2C_play_change_game_state_packet;

typedef struct {
  char flags;
  float flying_speed;
  float fov_modifier;
} S2C_play_player_abilities_packet;

typedef struct {
  cmc_buffer *data;
  char *channel;
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
