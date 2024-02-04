#pragma once

#include "mcbuffer.h"
#include "mconn.h"
#include "mctypes.h"
#include "packet_types.h"
#include <jansson.h>
#include <stdbool.h>

char *packet_data_to_string(int packet_id, MConn_state state,
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
void send_packet_C2S_handshake_handshake(struct MConn *conn,
                                         varint_t protocole_version,
                                         char *server_addr,
                                         unsigned short server_port,
                                         varint_t next_state);

void send_packet_S2C_status_response(struct MConn *conn, char *response);

void send_packet_S2C_status_pong(struct MConn *conn, long payload);

void send_packet_C2S_status_request(struct MConn *conn);

void send_packet_C2S_status_ping(struct MConn *conn, long payload);

void send_packet_S2C_login_disconnect(struct MConn *conn, char *reason);

void send_packet_S2C_login_encryption_request(struct MConn *conn,
                                              char *server_id,
                                              MCbuffer *public_key,
                                              MCbuffer *verify_token);

void send_packet_S2C_login_success(struct MConn *conn, char *uuid, char *name);

void send_packet_S2C_login_set_compression(struct MConn *conn,
                                           varint_t threshold);

void send_packet_C2S_login_start(struct MConn *conn, char *name);

void send_packet_C2S_login_encryption_response(struct MConn *conn,
                                               MCbuffer *shared_secret,
                                               MCbuffer *verify_token);

void send_packet_S2C_play_keep_alive(struct MConn *conn,
                                     varint_t keep_alive_id);

void send_packet_S2C_play_join_game(struct MConn *conn, int entity_id,
                                    unsigned char gamemode, char dimension,
                                    unsigned char difficulty,
                                    unsigned char max_players, char *level_type,
                                    bool reduced_debug_info);

void send_packet_S2C_play_chat_message(struct MConn *conn, char *message,
                                       char position);

void send_packet_S2C_play_time_update(struct MConn *conn, long world_age,
                                      long time_of_day);

void send_packet_S2C_play_entity_equipment(struct MConn *conn,
                                           varint_t entity_id, short slot,
                                           slot_t *item);

void send_packet_S2C_play_spawn_position(struct MConn *conn,
                                         block_pos_t location);

void send_packet_S2C_play_update_health(struct MConn *conn, float health,
                                        varint_t food, float food_saturation);

void send_packet_S2C_play_respawn(struct MConn *conn, int dimesion,
                                  unsigned char difficulty,
                                  unsigned char gamemode, char *level_type);

void send_packet_S2C_play_player_look_and_position(struct MConn *conn, double x,
                                                   double y, double z,
                                                   float yaw, float pitch,
                                                   unsigned char flags);

void send_packet_S2C_play_held_item_change(struct MConn *conn, char slot);

void send_packet_S2C_play_use_bed(struct MConn *conn, varint_t entity_id,
                                  block_pos_t location);

void send_packet_S2C_play_animation(struct MConn *conn, varint_t entity_id,
                                    unsigned char animation);

void send_packet_S2C_play_spawn_player(struct MConn *conn, varint_t entity_id,
                                       unsigned long long uuid, int x, int y,
                                       int z, unsigned char yaw,
                                       unsigned char pitch, short current_item,
                                       entity_metadata_t meta_data);

void send_packet_S2C_play_collect_item(struct MConn *conn,
                                       varint_t collected_entity_id,
                                       varint_t collector_entity_id);

void send_packet_S2C_play_spawn_mob(struct MConn *conn, varint_t entity_id,
                                    unsigned char type, int x, int y, int z,
                                    unsigned char yaw, unsigned char pitch,
                                    unsigned char head_pitch, short x_vel,
                                    short y_vel, short z_vel,
                                    entity_metadata_t meta_data);

void send_packet_S2C_play_spawn_painting(struct MConn *conn, varint_t entity_id,
                                         char *title, block_pos_t location,
                                         unsigned char direction);

void send_packet_S2C_play_spawn_experience_orb(struct MConn *conn,
                                               varint_t entity_id, int x, int y,
                                               int z, short count);

void send_packet_S2C_play_entity_velocity(struct MConn *conn,
                                          varint_t entity_id, short x_vel,
                                          short y_vel, short z_vel);

void send_packet_S2C_play_entity(struct MConn *conn, varint_t entity_id);

void send_packet_S2C_play_entity_relative_move(struct MConn *conn,
                                               varint_t entity_id, char delta_x,
                                               char delta_y, char delta_z,
                                               bool on_ground);

void send_packet_S2C_play_entity_look(struct MConn *conn, varint_t entity_id,
                                      unsigned char yaw, unsigned char pitch,
                                      bool on_ground);

void send_packet_S2C_play_entity_look_and_relative_move(
    struct MConn *conn, varint_t entity_id, char delta_x, char delta_y,
    char delta_z, unsigned char yaw, unsigned char pitch, bool on_ground);

void send_packet_S2C_play_entity_teleport(struct MConn *conn,
                                          varint_t entity_id, int x, int y,
                                          int z, unsigned char yaw,
                                          unsigned char pitch, bool on_ground);

void send_packet_S2C_play_entity_head_look(struct MConn *conn,
                                           varint_t entity_id,
                                           unsigned char head_yaw);

void send_packet_S2C_play_entity_status(struct MConn *conn, int entity_id,
                                        char entity_status);

void send_packet_S2C_play_attach_entity(struct MConn *conn, int entity_id,
                                        int vehicle_id, bool leash);

void send_packet_S2C_play_entity_metadata(struct MConn *conn,
                                          varint_t entity_id,
                                          entity_metadata_t meta_data);

void send_packet_S2C_play_entity_effect(struct MConn *conn, varint_t entity_id,
                                        char effect_id, char amplifier,
                                        varint_t duration, bool hide_particles);

void send_packet_S2C_play_remove_entity_effect(struct MConn *conn,
                                               varint_t entity_id,
                                               char effect_id);

void send_packet_S2C_play_set_experience(struct MConn *conn,
                                         float experience_bar, varint_t level,
                                         varint_t total_experience);

void send_packet_S2C_play_chunk_data(struct MConn *conn, int chunk_x,
                                     int chunk_z, bool ground_up_continuous,
                                     unsigned short primary_bitmask,
                                     MCbuffer *chunk);

void send_packet_S2C_play_block_change(struct MConn *conn, block_pos_t location,
                                       varint_t block_id);

void send_packet_S2C_play_block_action(struct MConn *conn, block_pos_t location,
                                       unsigned char block_data_1,
                                       unsigned char block_data_2,
                                       varint_t block_type);

void send_packet_S2C_play_block_break_animation(struct MConn *conn,
                                                varint_t entity_id,
                                                block_pos_t location,
                                                char destroy_stage);

void send_packet_S2C_play_effect(struct MConn *conn, int effect_id,
                                 block_pos_t location, int data, bool d,
                                 int particle_id, bool long_distances, float x,
                                 float y, float z, float x_offset,
                                 float y_offset, float z_offset,
                                 float particle_data, int particle_count,
                                 int sable_relative_volume);

void send_packet_S2C_play_sound_effect(struct MConn *conn, char *sound_name,
                                       int x, int y, int z, float volume,
                                       unsigned char pitch);

void send_packet_S2C_play_change_game_state(struct MConn *conn,
                                            unsigned char reason, float value);

void send_packet_S2C_play_player_abilities(struct MConn *conn, char flags,
                                           float flying_speed,
                                           float fov_modifier);

void send_packet_S2C_play_plugin_message(struct MConn *conn, char *channel,
                                         MCbuffer *data);

void send_packet_S2C_play_disconnect(struct MConn *conn, char *reason);

void send_packet_S2C_play_change_difficulty(struct MConn *conn,
                                            unsigned char difficulty);

void send_packet_C2S_play_keep_alive(struct MConn *conn,
                                     varint_t keep_alive_id);

// CGSE: send_methods_h

// CGSS: unpack_methods_h
C2S_handshake_handshake_packet_t
unpack_C2S_handshake_handshake_packet(MCbuffer *buff);

S2C_status_response_packet_t unpack_S2C_status_response_packet(MCbuffer *buff);

S2C_status_pong_packet_t unpack_S2C_status_pong_packet(MCbuffer *buff);

C2S_status_ping_packet_t unpack_C2S_status_ping_packet(MCbuffer *buff);

S2C_login_disconnect_packet_t
unpack_S2C_login_disconnect_packet(MCbuffer *buff);

S2C_login_encryption_request_packet_t
unpack_S2C_login_encryption_request_packet(MCbuffer *buff);

S2C_login_success_packet_t unpack_S2C_login_success_packet(MCbuffer *buff);

S2C_login_set_compression_packet_t
unpack_S2C_login_set_compression_packet(MCbuffer *buff);

C2S_login_start_packet_t unpack_C2S_login_start_packet(MCbuffer *buff);

C2S_login_encryption_response_packet_t
unpack_C2S_login_encryption_response_packet(MCbuffer *buff);

S2C_play_keep_alive_packet_t unpack_S2C_play_keep_alive_packet(MCbuffer *buff);

S2C_play_join_game_packet_t unpack_S2C_play_join_game_packet(MCbuffer *buff);

S2C_play_chat_message_packet_t
unpack_S2C_play_chat_message_packet(MCbuffer *buff);

S2C_play_time_update_packet_t
unpack_S2C_play_time_update_packet(MCbuffer *buff);

S2C_play_entity_equipment_packet_t
unpack_S2C_play_entity_equipment_packet(MCbuffer *buff);

S2C_play_spawn_position_packet_t
unpack_S2C_play_spawn_position_packet(MCbuffer *buff);

S2C_play_update_health_packet_t
unpack_S2C_play_update_health_packet(MCbuffer *buff);

S2C_play_respawn_packet_t unpack_S2C_play_respawn_packet(MCbuffer *buff);

S2C_play_player_look_and_position_packet_t
unpack_S2C_play_player_look_and_position_packet(MCbuffer *buff);

S2C_play_held_item_change_packet_t
unpack_S2C_play_held_item_change_packet(MCbuffer *buff);

S2C_play_use_bed_packet_t unpack_S2C_play_use_bed_packet(MCbuffer *buff);

S2C_play_animation_packet_t unpack_S2C_play_animation_packet(MCbuffer *buff);

S2C_play_spawn_player_packet_t
unpack_S2C_play_spawn_player_packet(MCbuffer *buff);

S2C_play_collect_item_packet_t
unpack_S2C_play_collect_item_packet(MCbuffer *buff);

S2C_play_spawn_mob_packet_t unpack_S2C_play_spawn_mob_packet(MCbuffer *buff);

S2C_play_spawn_painting_packet_t
unpack_S2C_play_spawn_painting_packet(MCbuffer *buff);

S2C_play_spawn_experience_orb_packet_t
unpack_S2C_play_spawn_experience_orb_packet(MCbuffer *buff);

S2C_play_entity_velocity_packet_t
unpack_S2C_play_entity_velocity_packet(MCbuffer *buff);

S2C_play_entity_packet_t unpack_S2C_play_entity_packet(MCbuffer *buff);

S2C_play_entity_relative_move_packet_t
unpack_S2C_play_entity_relative_move_packet(MCbuffer *buff);

S2C_play_entity_look_packet_t
unpack_S2C_play_entity_look_packet(MCbuffer *buff);

S2C_play_entity_look_and_relative_move_packet_t
unpack_S2C_play_entity_look_and_relative_move_packet(MCbuffer *buff);

S2C_play_entity_teleport_packet_t
unpack_S2C_play_entity_teleport_packet(MCbuffer *buff);

S2C_play_entity_head_look_packet_t
unpack_S2C_play_entity_head_look_packet(MCbuffer *buff);

S2C_play_entity_status_packet_t
unpack_S2C_play_entity_status_packet(MCbuffer *buff);

S2C_play_attach_entity_packet_t
unpack_S2C_play_attach_entity_packet(MCbuffer *buff);

S2C_play_entity_metadata_packet_t
unpack_S2C_play_entity_metadata_packet(MCbuffer *buff);

S2C_play_entity_effect_packet_t
unpack_S2C_play_entity_effect_packet(MCbuffer *buff);

S2C_play_remove_entity_effect_packet_t
unpack_S2C_play_remove_entity_effect_packet(MCbuffer *buff);

S2C_play_set_experience_packet_t
unpack_S2C_play_set_experience_packet(MCbuffer *buff);

S2C_play_chunk_data_packet_t unpack_S2C_play_chunk_data_packet(MCbuffer *buff);

S2C_play_block_change_packet_t
unpack_S2C_play_block_change_packet(MCbuffer *buff);

S2C_play_block_action_packet_t
unpack_S2C_play_block_action_packet(MCbuffer *buff);

S2C_play_block_break_animation_packet_t
unpack_S2C_play_block_break_animation_packet(MCbuffer *buff);

S2C_play_effect_packet_t unpack_S2C_play_effect_packet(MCbuffer *buff);

S2C_play_sound_effect_packet_t
unpack_S2C_play_sound_effect_packet(MCbuffer *buff);

S2C_play_change_game_state_packet_t
unpack_S2C_play_change_game_state_packet(MCbuffer *buff);

S2C_play_player_abilities_packet_t
unpack_S2C_play_player_abilities_packet(MCbuffer *buff);

S2C_play_plugin_message_packet_t
unpack_S2C_play_plugin_message_packet(MCbuffer *buff);

S2C_play_disconnect_packet_t unpack_S2C_play_disconnect_packet(MCbuffer *buff);

S2C_play_change_difficulty_packet_t
unpack_S2C_play_change_difficulty_packet(MCbuffer *buff);

C2S_play_keep_alive_packet_t unpack_C2S_play_keep_alive_packet(MCbuffer *buff);

// CGSE: unpack_methods_h