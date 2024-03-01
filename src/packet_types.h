#pragma once
#include "buffer.h"

// there are some circular include shenanigans with cmc_conn and packets header
// that the reason for the existence of this file

// CGSS: packet_types
typedef struct {
  char *server_addr;
  int next_state;
  int protocole_version;
  unsigned short server_port;
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
  cmc_buffer *shared_secret;
  cmc_buffer *verify_token;
} C2S_login_encryption_response_packet;

typedef struct {
  int keep_alive_id;
} S2C_play_keep_alive_packet;

typedef struct {
  char dimension;
  bool reduced_debug_info;
  unsigned char max_players;
  unsigned char gamemode;
  int entity_id;
  char *level_type;
  unsigned char difficulty;
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
  short slot;
  cmc_slot *item;
  int entity_id;
} S2C_play_entity_equipment_packet;

typedef struct {
  cmc_block_pos location;
} S2C_play_spawn_position_packet;

typedef struct {
  float health;
  float food_saturation;
  int food;
} S2C_play_update_health_packet;

typedef struct {
  int dimesion;
  char *level_type;
  unsigned char gamemode;
  unsigned char difficulty;
} S2C_play_respawn_packet;

typedef struct {
  double y;
  float pitch;
  double z;
  float yaw;
  unsigned char flags;
  double x;
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
  unsigned long long uuid;
  cmc_entity_metadata meta_data;
  unsigned char yaw;
  int y;
  int z;
  short current_item;
  int x;
  unsigned char pitch;
  int entity_id;
} S2C_play_spawn_player_packet;

typedef struct {
  int collector_entity_id;
  int collected_entity_id;
} S2C_play_collect_item_packet;

typedef struct {
  cmc_entity_metadata meta_data;
  unsigned char yaw;
  int y;
  unsigned char type;
  int z;
  short z_vel;
  int x;
  unsigned char pitch;
  short x_vel;
  unsigned char head_pitch;
  int entity_id;
  short y_vel;
} S2C_play_spawn_mob_packet;

typedef struct {
  char *title;
  unsigned char direction;
  int entity_id;
  cmc_block_pos location;
} S2C_play_spawn_painting_packet;

typedef struct {
  int y;
  int z;
  short count;
  int x;
  int entity_id;
} S2C_play_spawn_experience_orb_packet;

typedef struct {
  short z_vel;
  short x_vel;
  int entity_id;
  short y_vel;
} S2C_play_entity_velocity_packet;

typedef struct {
  int entity_id;
} S2C_play_entity_packet;

typedef struct {
  char delta_z;
  char delta_y;
  bool on_ground;
  char delta_x;
  int entity_id;
} S2C_play_entity_relative_move_packet;

typedef struct {
  unsigned char yaw;
  bool on_ground;
  int entity_id;
  unsigned char pitch;
} S2C_play_entity_look_packet;

typedef struct {
  unsigned char yaw;
  char delta_z;
  char delta_y;
  bool on_ground;
  unsigned char pitch;
  char delta_x;
  int entity_id;
} S2C_play_entity_look_and_relative_move_packet;

typedef struct {
  unsigned char yaw;
  int y;
  int z;
  bool on_ground;
  int x;
  unsigned char pitch;
  int entity_id;
} S2C_play_entity_teleport_packet;

typedef struct {
  unsigned char head_yaw;
  int entity_id;
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
  cmc_entity_metadata meta_data;
  int entity_id;
} S2C_play_entity_metadata_packet;

typedef struct {
  char effect_id;
  char amplifier;
  int duration;
  bool hide_particles;
  int entity_id;
} S2C_play_entity_effect_packet;

typedef struct {
  int entity_id;
  char effect_id;
} S2C_play_remove_entity_effect_packet;

typedef struct {
  int total_experience;
  int level;
  float experience_bar;
} S2C_play_set_experience_packet;

typedef struct {
  cmc_buffer *chunk;
  bool ground_up_continuous;
  int chunk_z;
  int chunk_x;
  unsigned short primary_bitmask;
} S2C_play_chunk_data_packet;

typedef struct {
  int block_id;
  cmc_block_pos location;
} S2C_play_block_change_packet;

typedef struct {
  unsigned char block_data_1;
  unsigned char block_data_2;
  int block_type;
  cmc_block_pos location;
} S2C_play_block_action_packet;

typedef struct {
  char destroy_stage;
  int entity_id;
  cmc_block_pos location;
} S2C_play_block_break_animation_packet;

typedef struct {
  float z;
  int effect_id;
  float y_offset;
  float y;
  float particle_data;
  float x_offset;
  float x;
  int sable_relative_volume;
  bool long_distances;
  float z_offset;
  bool d;
  cmc_block_pos location;
  int particle_count;
  int data;
  int particle_id;
} S2C_play_effect_packet;

typedef struct {
  int y;
  float volume;
  int z;
  int x;
  unsigned char pitch;
  char *sound_name;
} S2C_play_sound_effect_packet;

typedef struct {
  float value;
  unsigned char reason;
} S2C_play_change_game_state_packet;

typedef struct {
  char flags;
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
  unsigned char difficulty;
} S2C_play_change_difficulty_packet;

typedef struct {
  int keep_alive_id;
} C2S_play_keep_alive_packet;

// CGSE: packet_types
