#pragma once
#include "buffer.h"

// there are some circular include shenanigans with cmc_conn and packets header
// that the reason for the existence of this file

// CGSS: packet_types
typedef struct {int protocole_version;int next_state;unsigned short server_port;char *server_addr;} C2S_handshake_handshake_packet;

typedef struct {char *response;} S2C_status_response_packet;

typedef struct {long payload;} S2C_status_pong_packet;

typedef struct {long payload;} C2S_status_ping_packet;

typedef struct {char *reason;} S2C_login_disconnect_packet;

typedef struct {cmc_buffer *verify_token;char *server_id;cmc_buffer *public_key;} S2C_login_encryption_request_packet;

typedef struct {char *name;char *uuid;} S2C_login_success_packet;

typedef struct {int threshold;} S2C_login_set_compression_packet;

typedef struct {char *name;} C2S_login_start_packet;

typedef struct {cmc_buffer *verify_token;cmc_buffer *shared_secret;} C2S_login_encryption_response_packet;

typedef struct {int keep_alive_id;} S2C_play_keep_alive_packet;

typedef struct {unsigned char max_players;char dimension;unsigned char difficulty;bool reduced_debug_info;char *level_type;unsigned char gamemode;int entity_id;} S2C_play_join_game_packet;

typedef struct {char *message;char position;} S2C_play_chat_message_packet;

typedef struct {long world_age;long time_of_day;} S2C_play_time_update_packet;

typedef struct {cmc_slot *item;int entity_id;short slot;} S2C_play_entity_equipment_packet;

typedef struct {cmc_block_pos location;} S2C_play_spawn_position_packet;

typedef struct {int food;float food_saturation;float health;} S2C_play_update_health_packet;

typedef struct {char *level_type;int dimesion;unsigned char difficulty;unsigned char gamemode;} S2C_play_respawn_packet;

typedef struct {float yaw;unsigned char flags;float pitch;double y;double x;double z;} S2C_play_player_look_and_position_packet;

typedef struct {char slot;} S2C_play_held_item_change_packet;

typedef struct {int entity_id;cmc_block_pos location;} S2C_play_use_bed_packet;

typedef struct {int entity_id;unsigned char animation;} S2C_play_animation_packet;

typedef struct {cmc_entity_metadata meta_data;unsigned char pitch;int x;int z;unsigned char yaw;short current_item;int entity_id;int y;unsigned long long uuid;} S2C_play_spawn_player_packet;

typedef struct {int collected_entity_id;int collector_entity_id;} S2C_play_collect_item_packet;

typedef struct {short x_vel;cmc_entity_metadata meta_data;unsigned char pitch;int x;int z;unsigned char yaw;short z_vel;unsigned char type;int entity_id;short y_vel;int y;unsigned char head_pitch;} S2C_play_spawn_mob_packet;

typedef struct {int entity_id;unsigned char direction;char *title;cmc_block_pos location;} S2C_play_spawn_painting_packet;

typedef struct {int x;int z;int entity_id;short count;int y;} S2C_play_spawn_experience_orb_packet;

typedef struct {short x_vel;int entity_id;short y_vel;short z_vel;} S2C_play_entity_velocity_packet;

typedef struct {int entity_id;} S2C_play_entity_packet;

typedef struct {bool on_ground;char delta_z;char delta_y;char delta_x;int entity_id;} S2C_play_entity_relative_move_packet;

typedef struct {unsigned char yaw;unsigned char pitch;int entity_id;bool on_ground;} S2C_play_entity_look_packet;

typedef struct {bool on_ground;char delta_z;char delta_y;unsigned char yaw;char delta_x;int entity_id;unsigned char pitch;} S2C_play_entity_look_and_relative_move_packet;

typedef struct {bool on_ground;unsigned char pitch;int x;int z;unsigned char yaw;int entity_id;int y;} S2C_play_entity_teleport_packet;

typedef struct {int entity_id;unsigned char head_yaw;} S2C_play_entity_head_look_packet;

typedef struct {char entity_status;int entity_id;} S2C_play_entity_status_packet;

typedef struct {bool leash;int vehicle_id;int entity_id;} S2C_play_attach_entity_packet;

typedef struct {int entity_id;cmc_entity_metadata meta_data;} S2C_play_entity_metadata_packet;

typedef struct {char effect_id;bool hide_particles;char amplifier;int entity_id;int duration;} S2C_play_entity_effect_packet;

typedef struct {char effect_id;int entity_id;} S2C_play_remove_entity_effect_packet;

typedef struct {int level;float experience_bar;int total_experience;} S2C_play_set_experience_packet;

typedef struct {cmc_buffer *chunk;unsigned short primary_bitmask;int chunk_x;bool ground_up_continuous;int chunk_z;} S2C_play_chunk_data_packet;

typedef struct {int block_id;cmc_block_pos location;} S2C_play_block_change_packet;

typedef struct {int block_type;unsigned char block_data_1;unsigned char block_data_2;cmc_block_pos location;} S2C_play_block_action_packet;

typedef struct {int entity_id;char destroy_stage;cmc_block_pos location;} S2C_play_block_break_animation_packet;

typedef struct {int effect_id;int particle_id;int sable_relative_volume;float x_offset;int data;cmc_block_pos location;float particle_data;int particle_count;float y_offset;bool long_distances;float z;bool d;float x;float y;float z_offset;} S2C_play_effect_packet;

typedef struct {float volume;int x;unsigned char pitch;int z;char *sound_name;int y;} S2C_play_sound_effect_packet;

typedef struct {float value;unsigned char reason;} S2C_play_change_game_state_packet;

typedef struct {float fov_modifier;char flags;float flying_speed;} S2C_play_player_abilities_packet;

typedef struct {cmc_buffer *data;char *channel;} S2C_play_plugin_message_packet;

typedef struct {char *reason;} S2C_play_disconnect_packet;

typedef struct {unsigned char difficulty;} S2C_play_change_difficulty_packet;

typedef struct {int keep_alive_id;} C2S_play_keep_alive_packet;


// CGSE: packet_types
