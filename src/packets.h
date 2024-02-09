#pragma once

#include "buffer.h"
#include "conn.h"
#include "mctypes.h"
#include "packet_types.h"
#include <jansson.h>
#include <stdbool.h>

char *packet_id_to_string(int packet_id, cmc_conn_state state,
                          packet_direction direction);

// CGSS: packet_ids
enum packetids_C2S_handshake_t {
  packetid_C2S_handshake_handshake = 0x00,
};
enum packetids_S2C_status_t {
  packetid_S2C_status_response = 0x00,
  packetid_S2C_status_pong = 0x01,
};
enum packetids_C2S_status_t {
  packetid_C2S_status_request = 0x00,
  packetid_C2S_status_ping = 0x01,
};
enum packetids_S2C_login_t {
  packetid_S2C_login_disconnect = 0x00,
  packetid_S2C_login_encryption_request = 0x01,
  packetid_S2C_login_success = 0x02,
  packetid_S2C_login_set_compression = 0x03,
};
enum packetids_C2S_login_t {
  packetid_C2S_login_start = 0x00,
  packetid_C2S_login_encryption_response = 0x01,
};
enum packetids_S2C_play_t {
  packetid_S2C_play_keep_alive = 0x00,
  packetid_S2C_play_join_game = 0x01,
  packetid_S2C_play_chat_message = 0x02,
  packetid_S2C_play_time_update = 0x03,
  packetid_S2C_play_entity_equipment = 0x04,
  packetid_S2C_play_spawn_position = 0x05,
  packetid_S2C_play_update_health = 0x06,
  packetid_S2C_play_respawn = 0x07,
  packetid_S2C_play_player_look_and_position = 0x08,
  packetid_S2C_play_held_item_change = 0x09,
  packetid_S2C_play_use_bed = 0x0A,
  packetid_S2C_play_animation = 0x0B,
  packetid_S2C_play_spawn_player = 0x0C,
  packetid_S2C_play_collect_item = 0x0D,
  packetid_S2C_play_spawn_mob = 0x0F,
  packetid_S2C_play_spawn_painting = 0x10,
  packetid_S2C_play_spawn_experience_orb = 0x11,
  packetid_S2C_play_entity_velocity = 0x12,
  packetid_S2C_play_entity = 0x14,
  packetid_S2C_play_entity_relative_move = 0x15,
  packetid_S2C_play_entity_look = 0x16,
  packetid_S2C_play_entity_look_and_relative_move = 0x17,
  packetid_S2C_play_entity_teleport = 0x18,
  packetid_S2C_play_entity_head_look = 0x19,
  packetid_S2C_play_entity_status = 0x1A,
  packetid_S2C_play_attach_entity = 0x1B,
  packetid_S2C_play_entity_metadata = 0x1C,
  packetid_S2C_play_entity_effect = 0x1D,
  packetid_S2C_play_remove_entity_effect = 0x1E,
  packetid_S2C_play_set_experience = 0x1F,
  packetid_S2C_play_chunk_data = 0x21,
  packetid_S2C_play_block_change = 0x23,
  packetid_S2C_play_block_action = 0x24,
  packetid_S2C_play_block_break_animation = 0x25,
  packetid_S2C_play_effect = 0x28,
  packetid_S2C_play_sound_effect = 0x29,
  packetid_S2C_play_change_game_state = 0x2B,
  packetid_S2C_play_player_abilities = 0x39,
  packetid_S2C_play_plugin_message = 0x3F,
  packetid_S2C_play_disconnect = 0x40,
  packetid_S2C_play_change_difficulty = 0x41,
};
enum packetids_C2S_play_t {
  packetid_C2S_play_keep_alive = 0x00,
};
// CGSE: packet_ids

// CGSS: send_methods_h
void send_packet_C2S_handshake_handshake(struct cmc_conn *conn,
                                         varint_t protocole_version,
                                         char *server_addr,
                                         unsigned short server_port,
                                         varint_t next_state);

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
                                           varint_t threshold);

void send_packet_C2S_login_start(struct cmc_conn *conn, char *name);

void send_packet_C2S_login_encryption_response(struct cmc_conn *conn,
                                               cmc_buffer *shared_secret,
                                               cmc_buffer *verify_token);

void send_packet_S2C_play_keep_alive(struct cmc_conn *conn,
                                     varint_t keep_alive_id);

void send_packet_S2C_play_join_game(struct cmc_conn *conn, int entity_id,
                                    unsigned char gamemode, char dimension,
                                    unsigned char difficulty,
                                    unsigned char max_players, char *level_type,
                                    bool reduced_debug_info);

void send_packet_S2C_play_chat_message(struct cmc_conn *conn, char *message,
                                       char position);

void send_packet_S2C_play_time_update(struct cmc_conn *conn, long world_age,
                                      long time_of_day);

void send_packet_S2C_play_entity_equipment(struct cmc_conn *conn,
                                           varint_t entity_id, short slot,
                                           slot_t *item);

void send_packet_S2C_play_spawn_position(struct cmc_conn *conn,
                                         block_pos_t location);

void send_packet_S2C_play_update_health(struct cmc_conn *conn, float health,
                                        varint_t food, float food_saturation);

void send_packet_S2C_play_respawn(struct cmc_conn *conn, int dimesion,
                                  unsigned char difficulty,
                                  unsigned char gamemode, char *level_type);

void send_packet_S2C_play_player_look_and_position(struct cmc_conn *conn,
                                                   double x, double y, double z,
                                                   float yaw, float pitch,
                                                   unsigned char flags);

void send_packet_S2C_play_held_item_change(struct cmc_conn *conn, char slot);

void send_packet_S2C_play_use_bed(struct cmc_conn *conn, varint_t entity_id,
                                  block_pos_t location);

void send_packet_S2C_play_animation(struct cmc_conn *conn, varint_t entity_id,
                                    unsigned char animation);

void send_packet_S2C_play_spawn_player(struct cmc_conn *conn,
                                       varint_t entity_id,
                                       unsigned long long uuid, int x, int y,
                                       int z, unsigned char yaw,
                                       unsigned char pitch, short current_item,
                                       entity_metadata_t meta_data);

void send_packet_S2C_play_collect_item(struct cmc_conn *conn,
                                       varint_t collected_entity_id,
                                       varint_t collector_entity_id);

void send_packet_S2C_play_spawn_mob(struct cmc_conn *conn, varint_t entity_id,
                                    unsigned char type, int x, int y, int z,
                                    unsigned char yaw, unsigned char pitch,
                                    unsigned char head_pitch, short x_vel,
                                    short y_vel, short z_vel,
                                    entity_metadata_t meta_data);

void send_packet_S2C_play_spawn_painting(struct cmc_conn *conn,
                                         varint_t entity_id, char *title,
                                         block_pos_t location,
                                         unsigned char direction);

void send_packet_S2C_play_spawn_experience_orb(struct cmc_conn *conn,
                                               varint_t entity_id, int x, int y,
                                               int z, short count);

void send_packet_S2C_play_entity_velocity(struct cmc_conn *conn,
                                          varint_t entity_id, short x_vel,
                                          short y_vel, short z_vel);

void send_packet_S2C_play_entity(struct cmc_conn *conn, varint_t entity_id);

void send_packet_S2C_play_entity_relative_move(struct cmc_conn *conn,
                                               varint_t entity_id, char delta_x,
                                               char delta_y, char delta_z,
                                               bool on_ground);

void send_packet_S2C_play_entity_look(struct cmc_conn *conn, varint_t entity_id,
                                      unsigned char yaw, unsigned char pitch,
                                      bool on_ground);

void send_packet_S2C_play_entity_look_and_relative_move(
    struct cmc_conn *conn, varint_t entity_id, char delta_x, char delta_y,
    char delta_z, unsigned char yaw, unsigned char pitch, bool on_ground);

void send_packet_S2C_play_entity_teleport(struct cmc_conn *conn,
                                          varint_t entity_id, int x, int y,
                                          int z, unsigned char yaw,
                                          unsigned char pitch, bool on_ground);

void send_packet_S2C_play_entity_head_look(struct cmc_conn *conn,
                                           varint_t entity_id,
                                           unsigned char head_yaw);

void send_packet_S2C_play_entity_status(struct cmc_conn *conn, int entity_id,
                                        char entity_status);

void send_packet_S2C_play_attach_entity(struct cmc_conn *conn, int entity_id,
                                        int vehicle_id, bool leash);

void send_packet_S2C_play_entity_metadata(struct cmc_conn *conn,
                                          varint_t entity_id,
                                          entity_metadata_t meta_data);

void send_packet_S2C_play_entity_effect(struct cmc_conn *conn,
                                        varint_t entity_id, char effect_id,
                                        char amplifier, varint_t duration,
                                        bool hide_particles);

void send_packet_S2C_play_remove_entity_effect(struct cmc_conn *conn,
                                               varint_t entity_id,
                                               char effect_id);

void send_packet_S2C_play_set_experience(struct cmc_conn *conn,
                                         float experience_bar, varint_t level,
                                         varint_t total_experience);

void send_packet_S2C_play_chunk_data(struct cmc_conn *conn, int chunk_x,
                                     int chunk_z, bool ground_up_continuous,
                                     unsigned short primary_bitmask,
                                     cmc_buffer *chunk);

void send_packet_S2C_play_block_change(struct cmc_conn *conn,
                                       block_pos_t location, varint_t block_id);

void send_packet_S2C_play_block_action(struct cmc_conn *conn,
                                       block_pos_t location,
                                       unsigned char block_data_1,
                                       unsigned char block_data_2,
                                       varint_t block_type);

void send_packet_S2C_play_block_break_animation(struct cmc_conn *conn,
                                                varint_t entity_id,
                                                block_pos_t location,
                                                char destroy_stage);

void send_packet_S2C_play_effect(struct cmc_conn *conn, int effect_id,
                                 block_pos_t location, int data, bool d,
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

void send_packet_C2S_play_keep_alive(struct cmc_conn *conn,
                                     varint_t keep_alive_id);

// CGSE: send_methods_h

// CGSS: unpack_methods_h
C2S_handshake_handshake_packet_t
unpack_C2S_handshake_handshake_packet(cmc_buffer *buff);

S2C_status_response_packet_t
unpack_S2C_status_response_packet(cmc_buffer *buff);

S2C_status_pong_packet_t unpack_S2C_status_pong_packet(cmc_buffer *buff);

C2S_status_ping_packet_t unpack_C2S_status_ping_packet(cmc_buffer *buff);

S2C_login_disconnect_packet_t
unpack_S2C_login_disconnect_packet(cmc_buffer *buff);

S2C_login_encryption_request_packet_t
unpack_S2C_login_encryption_request_packet(cmc_buffer *buff);

S2C_login_success_packet_t unpack_S2C_login_success_packet(cmc_buffer *buff);

S2C_login_set_compression_packet_t
unpack_S2C_login_set_compression_packet(cmc_buffer *buff);

C2S_login_start_packet_t unpack_C2S_login_start_packet(cmc_buffer *buff);

C2S_login_encryption_response_packet_t
unpack_C2S_login_encryption_response_packet(cmc_buffer *buff);

S2C_play_keep_alive_packet_t
unpack_S2C_play_keep_alive_packet(cmc_buffer *buff);

S2C_play_join_game_packet_t unpack_S2C_play_join_game_packet(cmc_buffer *buff);

S2C_play_chat_message_packet_t
unpack_S2C_play_chat_message_packet(cmc_buffer *buff);

S2C_play_time_update_packet_t
unpack_S2C_play_time_update_packet(cmc_buffer *buff);

S2C_play_entity_equipment_packet_t
unpack_S2C_play_entity_equipment_packet(cmc_buffer *buff);

S2C_play_spawn_position_packet_t
unpack_S2C_play_spawn_position_packet(cmc_buffer *buff);

S2C_play_update_health_packet_t
unpack_S2C_play_update_health_packet(cmc_buffer *buff);

S2C_play_respawn_packet_t unpack_S2C_play_respawn_packet(cmc_buffer *buff);

S2C_play_player_look_and_position_packet_t
unpack_S2C_play_player_look_and_position_packet(cmc_buffer *buff);

S2C_play_held_item_change_packet_t
unpack_S2C_play_held_item_change_packet(cmc_buffer *buff);

S2C_play_use_bed_packet_t unpack_S2C_play_use_bed_packet(cmc_buffer *buff);

S2C_play_animation_packet_t unpack_S2C_play_animation_packet(cmc_buffer *buff);

S2C_play_spawn_player_packet_t
unpack_S2C_play_spawn_player_packet(cmc_buffer *buff);

S2C_play_collect_item_packet_t
unpack_S2C_play_collect_item_packet(cmc_buffer *buff);

S2C_play_spawn_mob_packet_t unpack_S2C_play_spawn_mob_packet(cmc_buffer *buff);

S2C_play_spawn_painting_packet_t
unpack_S2C_play_spawn_painting_packet(cmc_buffer *buff);

S2C_play_spawn_experience_orb_packet_t
unpack_S2C_play_spawn_experience_orb_packet(cmc_buffer *buff);

S2C_play_entity_velocity_packet_t
unpack_S2C_play_entity_velocity_packet(cmc_buffer *buff);

S2C_play_entity_packet_t unpack_S2C_play_entity_packet(cmc_buffer *buff);

S2C_play_entity_relative_move_packet_t
unpack_S2C_play_entity_relative_move_packet(cmc_buffer *buff);

S2C_play_entity_look_packet_t
unpack_S2C_play_entity_look_packet(cmc_buffer *buff);

S2C_play_entity_look_and_relative_move_packet_t
unpack_S2C_play_entity_look_and_relative_move_packet(cmc_buffer *buff);

S2C_play_entity_teleport_packet_t
unpack_S2C_play_entity_teleport_packet(cmc_buffer *buff);

S2C_play_entity_head_look_packet_t
unpack_S2C_play_entity_head_look_packet(cmc_buffer *buff);

S2C_play_entity_status_packet_t
unpack_S2C_play_entity_status_packet(cmc_buffer *buff);

S2C_play_attach_entity_packet_t
unpack_S2C_play_attach_entity_packet(cmc_buffer *buff);

S2C_play_entity_metadata_packet_t
unpack_S2C_play_entity_metadata_packet(cmc_buffer *buff);

S2C_play_entity_effect_packet_t
unpack_S2C_play_entity_effect_packet(cmc_buffer *buff);

S2C_play_remove_entity_effect_packet_t
unpack_S2C_play_remove_entity_effect_packet(cmc_buffer *buff);

S2C_play_set_experience_packet_t
unpack_S2C_play_set_experience_packet(cmc_buffer *buff);

S2C_play_chunk_data_packet_t
unpack_S2C_play_chunk_data_packet(cmc_buffer *buff);

S2C_play_block_change_packet_t
unpack_S2C_play_block_change_packet(cmc_buffer *buff);

S2C_play_block_action_packet_t
unpack_S2C_play_block_action_packet(cmc_buffer *buff);

S2C_play_block_break_animation_packet_t
unpack_S2C_play_block_break_animation_packet(cmc_buffer *buff);

S2C_play_effect_packet_t unpack_S2C_play_effect_packet(cmc_buffer *buff);

S2C_play_sound_effect_packet_t
unpack_S2C_play_sound_effect_packet(cmc_buffer *buff);

S2C_play_change_game_state_packet_t
unpack_S2C_play_change_game_state_packet(cmc_buffer *buff);

S2C_play_player_abilities_packet_t
unpack_S2C_play_player_abilities_packet(cmc_buffer *buff);

S2C_play_plugin_message_packet_t
unpack_S2C_play_plugin_message_packet(cmc_buffer *buff);

S2C_play_disconnect_packet_t
unpack_S2C_play_disconnect_packet(cmc_buffer *buff);

S2C_play_change_difficulty_packet_t
unpack_S2C_play_change_difficulty_packet(cmc_buffer *buff);

C2S_play_keep_alive_packet_t
unpack_C2S_play_keep_alive_packet(cmc_buffer *buff);

// CGSE: unpack_methods_h

// CGSS: free_methods_h
void free_C2S_handshake_handshake_packet(
    C2S_handshake_handshake_packet_t packet);
void free_S2C_status_response_packet(S2C_status_response_packet_t packet);
void free_S2C_login_disconnect_packet(S2C_login_disconnect_packet_t packet);
void free_S2C_login_encryption_request_packet(
    S2C_login_encryption_request_packet_t packet);
void free_S2C_login_success_packet(S2C_login_success_packet_t packet);
void free_C2S_login_start_packet(C2S_login_start_packet_t packet);
void free_C2S_login_encryption_response_packet(
    C2S_login_encryption_response_packet_t packet);
void free_S2C_play_join_game_packet(S2C_play_join_game_packet_t packet);
void free_S2C_play_chat_message_packet(S2C_play_chat_message_packet_t packet);
void free_S2C_play_entity_equipment_packet(
    S2C_play_entity_equipment_packet_t packet);
void free_S2C_play_respawn_packet(S2C_play_respawn_packet_t packet);
void free_S2C_play_spawn_player_packet(S2C_play_spawn_player_packet_t packet);
void free_S2C_play_spawn_mob_packet(S2C_play_spawn_mob_packet_t packet);
void free_S2C_play_spawn_painting_packet(
    S2C_play_spawn_painting_packet_t packet);
void free_S2C_play_entity_metadata_packet(
    S2C_play_entity_metadata_packet_t packet);
void free_S2C_play_chunk_data_packet(S2C_play_chunk_data_packet_t packet);
void free_S2C_play_sound_effect_packet(S2C_play_sound_effect_packet_t packet);
void free_S2C_play_plugin_message_packet(
    S2C_play_plugin_message_packet_t packet);
void free_S2C_play_disconnect_packet(S2C_play_disconnect_packet_t packet);
// CGSE: free_methods_h