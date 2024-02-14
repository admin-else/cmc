#pragma once

#include "buffer.h"
#include "conn.h"
#include "packet_types.h"
#include <jansson.h>
#include <stdbool.h>

char *packet_id_to_string(int packet_id, cmc_conn_state state,
                          packet_direction direction);

// CGSS: packet_ids
enum packetids_C2S_handshake {
  CMC_PACKETID_C2S_HANDSHAKE_HANDSHAKE = 0x00,
};
enum packetids_S2C_status {
  CMC_PACKETID_S2C_STATUS_RESPONSE = 0x00,
  CMC_PACKETID_S2C_STATUS_PONG = 0x01,
};
enum packetids_C2S_status {
  CMC_PACKETID_C2S_STATUS_REQUEST = 0x00,
  CMC_PACKETID_C2S_STATUS_PING = 0x01,
};
enum packetids_S2C_login {
  CMC_PACKETID_S2C_LOGIN_DISCONNECT = 0x00,
  CMC_PACKETID_S2C_LOGIN_ENCRYPTION_REQUEST = 0x01,
  CMC_PACKETID_S2C_LOGIN_SUCCESS = 0x02,
  CMC_PACKETID_S2C_LOGIN_SET_COMPRESSION = 0x03,
};
enum packetids_C2S_login {
  CMC_PACKETID_C2S_LOGIN_START = 0x00,
  CMC_PACKETID_C2S_LOGIN_ENCRYPTION_RESPONSE = 0x01,
};
enum packetids_S2C_play {
  CMC_PACKETID_S2C_PLAY_KEEP_ALIVE = 0x00,
  CMC_PACKETID_S2C_PLAY_JOIN_GAME = 0x01,
  CMC_PACKETID_S2C_PLAY_CHAT_MESSAGE = 0x02,
  CMC_PACKETID_S2C_PLAY_TIME_UPDATE = 0x03,
  CMC_PACKETID_S2C_PLAY_ENTITY_EQUIPMENT = 0x04,
  CMC_PACKETID_S2C_PLAY_SPAWN_POSITION = 0x05,
  CMC_PACKETID_S2C_PLAY_UPDATE_HEALTH = 0x06,
  CMC_PACKETID_S2C_PLAY_RESPAWN = 0x07,
  CMC_PACKETID_S2C_PLAY_PLAYER_LOOK_AND_POSITION = 0x08,
  CMC_PACKETID_S2C_PLAY_HELD_ITEM_CHANGE = 0x09,
  CMC_PACKETID_S2C_PLAY_USE_BED = 0x0A,
  CMC_PACKETID_S2C_PLAY_ANIMATION = 0x0B,
  CMC_PACKETID_S2C_PLAY_SPAWN_PLAYER = 0x0C,
  CMC_PACKETID_S2C_PLAY_COLLECT_ITEM = 0x0D,
  CMC_PACKETID_S2C_PLAY_SPAWN_MOB = 0x0F,
  CMC_PACKETID_S2C_PLAY_SPAWN_PAINTING = 0x10,
  CMC_PACKETID_S2C_PLAY_SPAWN_EXPERIENCE_ORB = 0x11,
  CMC_PACKETID_S2C_PLAY_ENTITY_VELOCITY = 0x12,
  CMC_PACKETID_S2C_PLAY_ENTITY = 0x14,
  CMC_PACKETID_S2C_PLAY_ENTITY_RELATIVE_MOVE = 0x15,
  CMC_PACKETID_S2C_PLAY_ENTITY_LOOK = 0x16,
  CMC_PACKETID_S2C_PLAY_ENTITY_LOOK_AND_RELATIVE_MOVE = 0x17,
  CMC_PACKETID_S2C_PLAY_ENTITY_TELEPORT = 0x18,
  CMC_PACKETID_S2C_PLAY_ENTITY_HEAD_LOOK = 0x19,
  CMC_PACKETID_S2C_PLAY_ENTITY_STATUS = 0x1A,
  CMC_PACKETID_S2C_PLAY_ATTACH_ENTITY = 0x1B,
  CMC_PACKETID_S2C_PLAY_ENTITY_METADATA = 0x1C,
  CMC_PACKETID_S2C_PLAY_ENTITY_EFFECT = 0x1D,
  CMC_PACKETID_S2C_PLAY_REMOVE_ENTITY_EFFECT = 0x1E,
  CMC_PACKETID_S2C_PLAY_SET_EXPERIENCE = 0x1F,
  CMC_PACKETID_S2C_PLAY_CHUNK_DATA = 0x21,
  CMC_PACKETID_S2C_PLAY_BLOCK_CHANGE = 0x23,
  CMC_PACKETID_S2C_PLAY_BLOCK_ACTION = 0x24,
  CMC_PACKETID_S2C_PLAY_BLOCK_BREAK_ANIMATION = 0x25,
  CMC_PACKETID_S2C_PLAY_EFFECT = 0x28,
  CMC_PACKETID_S2C_PLAY_SOUND_EFFECT = 0x29,
  CMC_PACKETID_S2C_PLAY_CHANGE_GAME_STATE = 0x2B,
  CMC_PACKETID_S2C_PLAY_PLAYER_ABILITIES = 0x39,
  CMC_PACKETID_S2C_PLAY_PLUGIN_MESSAGE = 0x3F,
  CMC_PACKETID_S2C_PLAY_DISCONNECT = 0x40,
  CMC_PACKETID_S2C_PLAY_CHANGE_DIFFICULTY = 0x41,
};
enum packetids_C2S_play {
  CMC_PACKETID_C2S_PLAY_KEEP_ALIVE = 0x00,
};
// CGSE: packet_ids

// CGSS: send_methods_h
void send_packet_C2S_handshake_handshake(struct cmc_conn *conn,
                                         int protocole_version,
                                         char *server_addr,
                                         unsigned short server_port,
                                         int next_state);

void send_packet_S2C_status_response(struct cmc_conn *conn, char *response);

void send_packet_S2C_status_pong(struct cmc_conn *conn, long payload);

void send_packet_C2S_status_request(struct cmc_conn *conn);

void send_packet_C2S_status_ping(struct cmc_conn *conn, long payload);

void send_packet_S2C_login_disconnect(struct cmc_conn *conn, char *reason);

void send_packet_S2C_login_encryption_request(struct cmc_conn *conn,
                                              char *server_id,
                                              cmc_buffer *public_key,
                                              cmc_buffer *verify_token);

void send_packet_S2C_login_success(struct cmc_conn *conn, char *uuid,
                                   char *name);

void send_packet_S2C_login_set_compression(struct cmc_conn *conn,
                                           int threshold);

void send_packet_C2S_login_start(struct cmc_conn *conn, char *name);

void send_packet_C2S_login_encryption_response(struct cmc_conn *conn,
                                               cmc_buffer *shared_secret,
                                               cmc_buffer *verify_token);

void send_packet_S2C_play_keep_alive(struct cmc_conn *conn, int keep_alive_id);

void send_packet_S2C_play_join_game(struct cmc_conn *conn, int entity_id,
                                    unsigned char gamemode, char dimension,
                                    unsigned char difficulty,
                                    unsigned char max_players, char *level_type,
                                    bool reduced_debug_info);

void send_packet_S2C_play_chat_message(struct cmc_conn *conn, char *message,
                                       char position);

void send_packet_S2C_play_time_update(struct cmc_conn *conn, long world_age,
                                      long time_of_day);

void send_packet_S2C_play_entity_equipment(struct cmc_conn *conn, int entity_id,
                                           short slot, cmc_slot *item);

void send_packet_S2C_play_spawn_position(struct cmc_conn *conn,
                                         cmc_block_pos location);

void send_packet_S2C_play_update_health(struct cmc_conn *conn, float health,
                                        int food, float food_saturation);

void send_packet_S2C_play_respawn(struct cmc_conn *conn, int dimesion,
                                  unsigned char difficulty,
                                  unsigned char gamemode, char *level_type);

void send_packet_S2C_play_player_look_and_position(struct cmc_conn *conn,
                                                   double x, double y, double z,
                                                   float yaw, float pitch,
                                                   unsigned char flags);

void send_packet_S2C_play_held_item_change(struct cmc_conn *conn, char slot);

void send_packet_S2C_play_use_bed(struct cmc_conn *conn, int entity_id,
                                  cmc_block_pos location);

void send_packet_S2C_play_animation(struct cmc_conn *conn, int entity_id,
                                    unsigned char animation);

void send_packet_S2C_play_spawn_player(struct cmc_conn *conn, int entity_id,
                                       unsigned long long uuid, int x, int y,
                                       int z, unsigned char yaw,
                                       unsigned char pitch, short current_item,
                                       cmc_entity_metadata meta_data);

void send_packet_S2C_play_collect_item(struct cmc_conn *conn,
                                       int collected_entity_id,
                                       int collector_entity_id);

void send_packet_S2C_play_spawn_mob(struct cmc_conn *conn, int entity_id,
                                    unsigned char type, int x, int y, int z,
                                    unsigned char yaw, unsigned char pitch,
                                    unsigned char head_pitch, short x_vel,
                                    short y_vel, short z_vel,
                                    cmc_entity_metadata meta_data);

void send_packet_S2C_play_spawn_painting(struct cmc_conn *conn, int entity_id,
                                         char *title, cmc_block_pos location,
                                         unsigned char direction);

void send_packet_S2C_play_spawn_experience_orb(struct cmc_conn *conn,
                                               int entity_id, int x, int y,
                                               int z, short count);

void send_packet_S2C_play_entity_velocity(struct cmc_conn *conn, int entity_id,
                                          short x_vel, short y_vel,
                                          short z_vel);

void send_packet_S2C_play_entity(struct cmc_conn *conn, int entity_id);

void send_packet_S2C_play_entity_relative_move(struct cmc_conn *conn,
                                               int entity_id, char delta_x,
                                               char delta_y, char delta_z,
                                               bool on_ground);

void send_packet_S2C_play_entity_look(struct cmc_conn *conn, int entity_id,
                                      unsigned char yaw, unsigned char pitch,
                                      bool on_ground);

void send_packet_S2C_play_entity_look_and_relative_move(
    struct cmc_conn *conn, int entity_id, char delta_x, char delta_y,
    char delta_z, unsigned char yaw, unsigned char pitch, bool on_ground);

void send_packet_S2C_play_entity_teleport(struct cmc_conn *conn, int entity_id,
                                          int x, int y, int z,
                                          unsigned char yaw,
                                          unsigned char pitch, bool on_ground);

void send_packet_S2C_play_entity_head_look(struct cmc_conn *conn, int entity_id,
                                           unsigned char head_yaw);

void send_packet_S2C_play_entity_status(struct cmc_conn *conn, int entity_id,
                                        char entity_status);

void send_packet_S2C_play_attach_entity(struct cmc_conn *conn, int entity_id,
                                        int vehicle_id, bool leash);

void send_packet_S2C_play_entity_metadata(struct cmc_conn *conn, int entity_id,
                                          cmc_entity_metadata meta_data);

void send_packet_S2C_play_entity_effect(struct cmc_conn *conn, int entity_id,
                                        char effect_id, char amplifier,
                                        int duration, bool hide_particles);

void send_packet_S2C_play_remove_entity_effect(struct cmc_conn *conn,
                                               int entity_id, char effect_id);

void send_packet_S2C_play_set_experience(struct cmc_conn *conn,
                                         float experience_bar, int level,
                                         int total_experience);

void send_packet_S2C_play_chunk_data(struct cmc_conn *conn, int chunk_x,
                                     int chunk_z, bool ground_up_continuous,
                                     unsigned short primary_bitmask,
                                     cmc_buffer *chunk);

void send_packet_S2C_play_block_change(struct cmc_conn *conn,
                                       cmc_block_pos location, int block_id);

void send_packet_S2C_play_block_action(struct cmc_conn *conn,
                                       cmc_block_pos location,
                                       unsigned char block_data_1,
                                       unsigned char block_data_2,
                                       int block_type);

void send_packet_S2C_play_block_break_animation(struct cmc_conn *conn,
                                                int entity_id,
                                                cmc_block_pos location,
                                                char destroy_stage);

void send_packet_S2C_play_effect(struct cmc_conn *conn, int effect_id,
                                 cmc_block_pos location, int data, bool d,
                                 int particle_id, bool long_distances, float x,
                                 float y, float z, float x_offset,
                                 float y_offset, float z_offset,
                                 float particle_data, int particle_count,
                                 int sable_relative_volume);

void send_packet_S2C_play_sound_effect(struct cmc_conn *conn, char *sound_name,
                                       int x, int y, int z, float volume,
                                       unsigned char pitch);

void send_packet_S2C_play_change_game_state(struct cmc_conn *conn,
                                            unsigned char reason, float value);

void send_packet_S2C_play_player_abilities(struct cmc_conn *conn, char flags,
                                           float flying_speed,
                                           float fov_modifier);

void send_packet_S2C_play_plugin_message(struct cmc_conn *conn, char *channel,
                                         cmc_buffer *data);

void send_packet_S2C_play_disconnect(struct cmc_conn *conn, char *reason);

void send_packet_S2C_play_change_difficulty(struct cmc_conn *conn,
                                            unsigned char difficulty);

void send_packet_C2S_play_keep_alive(struct cmc_conn *conn, int keep_alive_id);

// CGSE: send_methods_h

// CGSS: unpack_methods_h
C2S_handshake_handshake_packet
unpack_C2S_handshake_handshake_packet(cmc_buffer *buff);

S2C_status_response_packet unpack_S2C_status_response_packet(cmc_buffer *buff);

S2C_status_pong_packet unpack_S2C_status_pong_packet(cmc_buffer *buff);

C2S_status_ping_packet unpack_C2S_status_ping_packet(cmc_buffer *buff);

S2C_login_disconnect_packet
unpack_S2C_login_disconnect_packet(cmc_buffer *buff);

S2C_login_encryption_request_packet
unpack_S2C_login_encryption_request_packet(cmc_buffer *buff);

S2C_login_success_packet unpack_S2C_login_success_packet(cmc_buffer *buff);

S2C_login_set_compression_packet
unpack_S2C_login_set_compression_packet(cmc_buffer *buff);

C2S_login_start_packet unpack_C2S_login_start_packet(cmc_buffer *buff);

C2S_login_encryption_response_packet
unpack_C2S_login_encryption_response_packet(cmc_buffer *buff);

S2C_play_keep_alive_packet unpack_S2C_play_keep_alive_packet(cmc_buffer *buff);

S2C_play_join_game_packet unpack_S2C_play_join_game_packet(cmc_buffer *buff);

S2C_play_chat_message_packet
unpack_S2C_play_chat_message_packet(cmc_buffer *buff);

S2C_play_time_update_packet
unpack_S2C_play_time_update_packet(cmc_buffer *buff);

S2C_play_entity_equipment_packet
unpack_S2C_play_entity_equipment_packet(cmc_buffer *buff);

S2C_play_spawn_position_packet
unpack_S2C_play_spawn_position_packet(cmc_buffer *buff);

S2C_play_update_health_packet
unpack_S2C_play_update_health_packet(cmc_buffer *buff);

S2C_play_respawn_packet unpack_S2C_play_respawn_packet(cmc_buffer *buff);

S2C_play_player_look_and_position_packet
unpack_S2C_play_player_look_and_position_packet(cmc_buffer *buff);

S2C_play_held_item_change_packet
unpack_S2C_play_held_item_change_packet(cmc_buffer *buff);

S2C_play_use_bed_packet unpack_S2C_play_use_bed_packet(cmc_buffer *buff);

S2C_play_animation_packet unpack_S2C_play_animation_packet(cmc_buffer *buff);

S2C_play_spawn_player_packet
unpack_S2C_play_spawn_player_packet(cmc_buffer *buff);

S2C_play_collect_item_packet
unpack_S2C_play_collect_item_packet(cmc_buffer *buff);

S2C_play_spawn_mob_packet unpack_S2C_play_spawn_mob_packet(cmc_buffer *buff);

S2C_play_spawn_painting_packet
unpack_S2C_play_spawn_painting_packet(cmc_buffer *buff);

S2C_play_spawn_experience_orb_packet
unpack_S2C_play_spawn_experience_orb_packet(cmc_buffer *buff);

S2C_play_entity_velocity_packet
unpack_S2C_play_entity_velocity_packet(cmc_buffer *buff);

S2C_play_entity_packet unpack_S2C_play_entity_packet(cmc_buffer *buff);

S2C_play_entity_relative_move_packet
unpack_S2C_play_entity_relative_move_packet(cmc_buffer *buff);

S2C_play_entity_look_packet
unpack_S2C_play_entity_look_packet(cmc_buffer *buff);

S2C_play_entity_look_and_relative_move_packet
unpack_S2C_play_entity_look_and_relative_move_packet(cmc_buffer *buff);

S2C_play_entity_teleport_packet
unpack_S2C_play_entity_teleport_packet(cmc_buffer *buff);

S2C_play_entity_head_look_packet
unpack_S2C_play_entity_head_look_packet(cmc_buffer *buff);

S2C_play_entity_status_packet
unpack_S2C_play_entity_status_packet(cmc_buffer *buff);

S2C_play_attach_entity_packet
unpack_S2C_play_attach_entity_packet(cmc_buffer *buff);

S2C_play_entity_metadata_packet
unpack_S2C_play_entity_metadata_packet(cmc_buffer *buff);

S2C_play_entity_effect_packet
unpack_S2C_play_entity_effect_packet(cmc_buffer *buff);

S2C_play_remove_entity_effect_packet
unpack_S2C_play_remove_entity_effect_packet(cmc_buffer *buff);

S2C_play_set_experience_packet
unpack_S2C_play_set_experience_packet(cmc_buffer *buff);

S2C_play_chunk_data_packet unpack_S2C_play_chunk_data_packet(cmc_buffer *buff);

S2C_play_block_change_packet
unpack_S2C_play_block_change_packet(cmc_buffer *buff);

S2C_play_block_action_packet
unpack_S2C_play_block_action_packet(cmc_buffer *buff);

S2C_play_block_break_animation_packet
unpack_S2C_play_block_break_animation_packet(cmc_buffer *buff);

S2C_play_effect_packet unpack_S2C_play_effect_packet(cmc_buffer *buff);

S2C_play_sound_effect_packet
unpack_S2C_play_sound_effect_packet(cmc_buffer *buff);

S2C_play_change_game_state_packet
unpack_S2C_play_change_game_state_packet(cmc_buffer *buff);

S2C_play_player_abilities_packet
unpack_S2C_play_player_abilities_packet(cmc_buffer *buff);

S2C_play_plugin_message_packet
unpack_S2C_play_plugin_message_packet(cmc_buffer *buff);

S2C_play_disconnect_packet unpack_S2C_play_disconnect_packet(cmc_buffer *buff);

S2C_play_change_difficulty_packet
unpack_S2C_play_change_difficulty_packet(cmc_buffer *buff);

C2S_play_keep_alive_packet unpack_C2S_play_keep_alive_packet(cmc_buffer *buff);

// CGSE: unpack_methods_h

// CGSS: free_methods_h
void cmc_free_C2S_handshake_handshake_packet(
    C2S_handshake_handshake_packet packet);
void cmc_free_S2C_status_response_packet(S2C_status_response_packet packet);
void cmc_free_S2C_login_disconnect_packet(S2C_login_disconnect_packet packet);
void cmc_free_S2C_login_encryption_request_packet(
    S2C_login_encryption_request_packet packet);
void cmc_free_S2C_login_success_packet(S2C_login_success_packet packet);
void cmc_free_C2S_login_start_packet(C2S_login_start_packet packet);
void cmc_free_C2S_login_encryption_response_packet(
    C2S_login_encryption_response_packet packet);
void cmc_free_S2C_play_join_game_packet(S2C_play_join_game_packet packet);
void cmc_free_S2C_play_chat_message_packet(S2C_play_chat_message_packet packet);
void cmc_free_S2C_play_entity_equipment_packet(
    S2C_play_entity_equipment_packet packet);
void cmc_free_S2C_play_respawn_packet(S2C_play_respawn_packet packet);
void cmc_free_S2C_play_spawn_player_packet(S2C_play_spawn_player_packet packet);
void cmc_free_S2C_play_spawn_mob_packet(S2C_play_spawn_mob_packet packet);
void cmc_free_S2C_play_spawn_painting_packet(
    S2C_play_spawn_painting_packet packet);
void cmc_free_S2C_play_entity_metadata_packet(
    S2C_play_entity_metadata_packet packet);
void cmc_free_S2C_play_chunk_data_packet(S2C_play_chunk_data_packet packet);
void cmc_free_S2C_play_sound_effect_packet(S2C_play_sound_effect_packet packet);
void cmc_free_S2C_play_plugin_message_packet(
    S2C_play_plugin_message_packet packet);
void cmc_free_S2C_play_disconnect_packet(S2C_play_disconnect_packet packet);
// CGSE: free_methods_h