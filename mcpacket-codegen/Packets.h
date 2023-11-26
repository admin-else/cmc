// This code is generated.
#pragma once

#include "MCbuffer.h"
#include "MConn.h"
#include "MCtypes.h"
#include <jansson.h>
#include <stdbool.h>

#define PACKETID_C2S_HANDSHAKE 0x00
#define PACKETID_S2C_STATUS_RESPONSE 0x00
#define PACKETID_S2C_STATUS_PONG 0x01
#define PACKETID_C2S_STATUS_REQUEST 0x00
#define PACKETID_C2S_STATUS_PING 0x01
#define PACKETID_S2C_LOGIN_DISCONNECT 0x00
#define PACKETID_S2C_LOGIN_ENCRYPTION_REQUEST 0x01
#define PACKETID_S2C_LOGIN_SUCCESS 0x02
#define PACKETID_S2C_LOGIN_SET_COMPRESSION 0x03
#define PACKETID_C2S_LOGIN_START 0x00
#define PACKETID_C2S_LOGIN_ENCRYPTION_RESPONSE 0x01
#define PACKETID_S2C_PLAY_KEEP_ALIVE 0x00
#define PACKETID_S2C_PLAY_JOIN_GAME 0x01
#define PACKETID_S2C_PLAY_CHAT_MESSAGE 0x02
#define PACKETID_S2C_PLAY_TIME_UPDATE 0x03
#define PACKETID_S2C_PLAY_ENTITY_EQUIPMENT 0x04
#define PACKETID_S2C_PLAY_SPAWN_POSITION 0x05
#define PACKETID_S2C_PLAY_UPDATE_HEALTH 0x06
#define PACKETID_S2C_PLAY_RESPAWN 0x07
#define PACKETID_S2C_PLAY_PLAYER_LOOK_AND_POSITION 0x08
#define PACKETID_S2C_PLAY_HELD_ITEM_CHANGE 0x09
#define PACKETID_S2C_PLAY_USE_BED 0x0A
#define PACKETID_S2C_PLAY_ANIMATION 0x0B
#define PACKETID_S2C_PLAY_SPAWN_PLAYER 0x0C
#define PACKETID_S2C_PLAY_COLLECT_ITEM 0x0D
#define PACKETID_S2C_PLAY_SPAWN_MOB 0x0F
#define PACKETID_S2C_PLAY_SPAWN_PAINTING 0x10
#define PACKETID_S2C_PLAY_SPAWN_EXPERIENCE_ORB 0x11
#define PACKETID_S2C_PLAY_ENTITY_VELOCITY 0x12
#define PACKETID_S2C_PLAY_ENTITY 0x14
#define PACKETID_S2C_PLAY_ENTITY_RELATIVE_MOVE 0x15
#define PACKETID_S2C_PLAY_ENTITY_LOOK 0x16
#define PACKETID_S2C_PLAY_ENTITY_LOOK_AND_RELATIVE_MOVE 0x17
#define PACKETID_S2C_PLAY_ENTITY_TELEPORT 0x18
#define PACKETID_S2C_PLAY_ENTITY_HEAD_LOOK 0x19
#define PACKETID_S2C_PLAY_ENTITY_STATUS 0x1A
#define PACKETID_S2C_PLAY_ATTACH_ENTITY 0x1B
#define PACKETID_S2C_PLAY_ENTITY_METADATA 0x1C
#define PACKETID_S2C_PLAY_ENTITY_EFFECT 0x1D
#define PACKETID_S2C_PLAY_REMOVE_ENTITY_EFFECT 0x1E
#define PACKETID_S2C_PLAY_SET_EXPERIENCE 0x1F
#define PACKETID_S2C_PLAY_CHUNK_DATA 0x21
#define PACKETID_S2C_PLAY_BLOCK_CHANGE 0x23
#define PACKETID_S2C_PLAY_BLOCK_ACTION 0x24
#define PACKETID_S2C_PLAY_BLOCK_BREAK_ANIMATION 0x25
#define PACKETID_S2C_PLAY_EFFECT 0x28
#define PACKETID_S2C_PLAY_SOUND_EFFECT 0x29
#define PACKETID_S2C_PLAY_CHANGE_GAME_STATE 0x2B
#define PACKETID_S2C_PLAY_PLAYER_ABILITIES 0x39
#define PACKETID_S2C_PLAY_PLUGIN_MESSAGE 0x3F
#define PACKETID_S2C_PLAY_DISCONNECT 0x40
#define PACKETID_S2C_PLAY_CHANGE_DIFFICULTY 0x41
#define PACKETID_C2S_PLAY_KEEP_ALIVE 0x00

char *packet_data_to_string(int packet_id, MConn_state state,
                            packet_direction direction);

typedef struct {
  varint_t protocole_version;
  char *server_addr;
  unsigned short server_port;
  varint_t next_state;
} C2S_handshake_packet_t;
void send_packet_C2S_handshake(MConn *conn, varint_t protocole_version,
                               char *server_addr, unsigned short server_port,
                               varint_t next_state, char **errmsg);
C2S_handshake_packet_t unpack_C2S_handshake_packet(MCbuffer *buff,
                                                   char **errmsg);
typedef struct {
  json_t *response;
} S2C_status_response_packet_t;
void send_packet_S2C_status_response(MConn *conn, json_t *response,
                                     char **errmsg);
S2C_status_response_packet_t unpack_S2C_status_response_packet(MCbuffer *buff,
                                                               char **errmsg);
typedef struct {
  long payload;
} S2C_status_pong_packet_t;
void send_packet_S2C_status_pong(MConn *conn, long payload, char **errmsg);
S2C_status_pong_packet_t unpack_S2C_status_pong_packet(MCbuffer *buff,
                                                       char **errmsg);
void send_packet_C2S_status_request(MConn *conn, char **errmsg);
typedef struct {
  long payload;
} C2S_status_ping_packet_t;
void send_packet_C2S_status_ping(MConn *conn, long payload, char **errmsg);
C2S_status_ping_packet_t unpack_C2S_status_ping_packet(MCbuffer *buff,
                                                       char **errmsg);
typedef struct {
  json_t *reason;
} S2C_login_disconnect_packet_t;
void send_packet_S2C_login_disconnect(MConn *conn, json_t *reason,
                                      char **errmsg);
S2C_login_disconnect_packet_t unpack_S2C_login_disconnect_packet(MCbuffer *buff,
                                                                 char **errmsg);
typedef struct {
  char *server_id;
  MCbuffer *public_key;
  MCbuffer *verify_token;
} S2C_login_encryption_request_packet_t;
void send_packet_S2C_login_encryption_request(MConn *conn, char *server_id,
                                              MCbuffer *public_key,
                                              MCbuffer *verify_token,
                                              char **errmsg);
S2C_login_encryption_request_packet_t
unpack_S2C_login_encryption_request_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  char *uuid;
  char *name;
} S2C_login_success_packet_t;
void send_packet_S2C_login_success(MConn *conn, char *uuid, char *name,
                                   char **errmsg);
S2C_login_success_packet_t unpack_S2C_login_success_packet(MCbuffer *buff,
                                                           char **errmsg);
typedef struct {
  varint_t threshold;
} S2C_login_set_compression_packet_t;
void send_packet_S2C_login_set_compression(MConn *conn, varint_t threshold,
                                           char **errmsg);
S2C_login_set_compression_packet_t
unpack_S2C_login_set_compression_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  char *name;
} C2S_login_start_packet_t;
void send_packet_C2S_login_start(MConn *conn, char *name, char **errmsg);
C2S_login_start_packet_t unpack_C2S_login_start_packet(MCbuffer *buff,
                                                       char **errmsg);
typedef struct {
  MCbuffer *shared_secret;
  MCbuffer *verify_token;
} C2S_login_encryption_response_packet_t;
void send_packet_C2S_login_encryption_response(MConn *conn,
                                               MCbuffer *shared_secret,
                                               MCbuffer *verify_token,
                                               char **errmsg);
C2S_login_encryption_response_packet_t
unpack_C2S_login_encryption_response_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  varint_t keep_alive_id;
} S2C_play_keep_alive_packet_t;
void send_packet_S2C_play_keep_alive(MConn *conn, varint_t keep_alive_id,
                                     char **errmsg);
S2C_play_keep_alive_packet_t unpack_S2C_play_keep_alive_packet(MCbuffer *buff,
                                                               char **errmsg);
typedef struct {
  int entity_id;
  byte_t gamemode;
  char dimension;
  byte_t difficulty;
  byte_t max_players;
  char *level_type;
  bool reduced_debug_info;
} S2C_play_join_game_packet_t;
void send_packet_S2C_play_join_game(MConn *conn, int entity_id, byte_t gamemode,
                                    char dimension, byte_t difficulty,
                                    byte_t max_players, char *level_type,
                                    bool reduced_debug_info, char **errmsg);
S2C_play_join_game_packet_t unpack_S2C_play_join_game_packet(MCbuffer *buff,
                                                             char **errmsg);
typedef struct {
  json_t *message;
  char position;
} S2C_play_chat_message_packet_t;
void send_packet_S2C_play_chat_message(MConn *conn, json_t *message,
                                       char position, char **errmsg);
S2C_play_chat_message_packet_t
unpack_S2C_play_chat_message_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  long world_age;
  long time_of_day;
} S2C_play_time_update_packet_t;
void send_packet_S2C_play_time_update(MConn *conn, long world_age,
                                      long time_of_day, char **errmsg);
S2C_play_time_update_packet_t unpack_S2C_play_time_update_packet(MCbuffer *buff,
                                                                 char **errmsg);
typedef struct {
  varint_t entity_id;
  short slot;
  slot_t *item;
} S2C_play_entity_equipment_packet_t;
void send_packet_S2C_play_entity_equipment(MConn *conn, varint_t entity_id,
                                           short slot, slot_t *item,
                                           char **errmsg);
S2C_play_entity_equipment_packet_t
unpack_S2C_play_entity_equipment_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  block_pos_t location;
} S2C_play_spawn_position_packet_t;
void send_packet_S2C_play_spawn_position(MConn *conn, block_pos_t location,
                                         char **errmsg);
S2C_play_spawn_position_packet_t
unpack_S2C_play_spawn_position_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  float health;
  varint_t food;
  float food_saturation;
} S2C_play_update_health_packet_t;
void send_packet_S2C_play_update_health(MConn *conn, float health,
                                        varint_t food, float food_saturation,
                                        char **errmsg);
S2C_play_update_health_packet_t
unpack_S2C_play_update_health_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  int dimesion;
  byte_t difficulty;
  byte_t gamemode;
  char *level_type;
} S2C_play_respawn_packet_t;
void send_packet_S2C_play_respawn(MConn *conn, int dimesion, byte_t difficulty,
                                  byte_t gamemode, char *level_type,
                                  char **errmsg);
S2C_play_respawn_packet_t unpack_S2C_play_respawn_packet(MCbuffer *buff,
                                                         char **errmsg);
typedef struct {
  double x;
  double y;
  double z;
  float yaw;
  float pitch;
  byte_t flags;
} S2C_play_player_look_and_position_packet_t;
void send_packet_S2C_play_player_look_and_position(MConn *conn, double x,
                                                   double y, double z,
                                                   float yaw, float pitch,
                                                   byte_t flags, char **errmsg);
S2C_play_player_look_and_position_packet_t
unpack_S2C_play_player_look_and_position_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  char slot;
} S2C_play_held_item_change_packet_t;
void send_packet_S2C_play_held_item_change(MConn *conn, char slot,
                                           char **errmsg);
S2C_play_held_item_change_packet_t
unpack_S2C_play_held_item_change_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  varint_t entity_id;
  block_pos_t location;
} S2C_play_use_bed_packet_t;
void send_packet_S2C_play_use_bed(MConn *conn, varint_t entity_id,
                                  block_pos_t location, char **errmsg);
S2C_play_use_bed_packet_t unpack_S2C_play_use_bed_packet(MCbuffer *buff,
                                                         char **errmsg);
typedef struct {
  varint_t entity_id;
  byte_t animation;
} S2C_play_animation_packet_t;
void send_packet_S2C_play_animation(MConn *conn, varint_t entity_id,
                                    byte_t animation, char **errmsg);
S2C_play_animation_packet_t unpack_S2C_play_animation_packet(MCbuffer *buff,
                                                             char **errmsg);
typedef struct {
  varint_t entity_id;
  unsigned long long uuid;
  int x;
  int y;
  int z;
  byte_t yaw;
  byte_t pitch;
  short current_item;
  entity_metadata_t meta_data;
} S2C_play_spawn_player_packet_t;
void send_packet_S2C_play_spawn_player(MConn *conn, varint_t entity_id,
                                       unsigned long long uuid, int x, int y,
                                       int z, byte_t yaw, byte_t pitch,
                                       short current_item,
                                       entity_metadata_t meta_data,
                                       char **errmsg);
S2C_play_spawn_player_packet_t
unpack_S2C_play_spawn_player_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  varint_t collected_entity_id;
  varint_t collector_entity_id;
} S2C_play_collect_item_packet_t;
void send_packet_S2C_play_collect_item(MConn *conn,
                                       varint_t collected_entity_id,
                                       varint_t collector_entity_id,
                                       char **errmsg);
S2C_play_collect_item_packet_t
unpack_S2C_play_collect_item_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  varint_t entity_id;
  byte_t type;
  int x;
  int y;
  int z;
  byte_t yaw;
  byte_t pitch;
  byte_t head_pitch;
  short x_vel;
  short y_vel;
  short z_vel;
  entity_metadata_t meta_data;
} S2C_play_spawn_mob_packet_t;
void send_packet_S2C_play_spawn_mob(MConn *conn, varint_t entity_id,
                                    byte_t type, int x, int y, int z,
                                    byte_t yaw, byte_t pitch, byte_t head_pitch,
                                    short x_vel, short y_vel, short z_vel,
                                    entity_metadata_t meta_data, char **errmsg);
S2C_play_spawn_mob_packet_t unpack_S2C_play_spawn_mob_packet(MCbuffer *buff,
                                                             char **errmsg);
typedef struct {
  varint_t entity_id;
  char *title;
  block_pos_t location;
  byte_t direction;
} S2C_play_spawn_painting_packet_t;
void send_packet_S2C_play_spawn_painting(MConn *conn, varint_t entity_id,
                                         char *title, block_pos_t location,
                                         byte_t direction, char **errmsg);
S2C_play_spawn_painting_packet_t
unpack_S2C_play_spawn_painting_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  varint_t entity_id;
  int x;
  int y;
  int z;
  short count;
} S2C_play_spawn_experience_orb_packet_t;
void send_packet_S2C_play_spawn_experience_orb(MConn *conn, varint_t entity_id,
                                               int x, int y, int z, short count,
                                               char **errmsg);
S2C_play_spawn_experience_orb_packet_t
unpack_S2C_play_spawn_experience_orb_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  varint_t entity_id;
  short x_vel;
  short y_vel;
  short z_vel;
} S2C_play_entity_velocity_packet_t;
void send_packet_S2C_play_entity_velocity(MConn *conn, varint_t entity_id,
                                          short x_vel, short y_vel, short z_vel,
                                          char **errmsg);
S2C_play_entity_velocity_packet_t
unpack_S2C_play_entity_velocity_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  varint_t entity_id;
} S2C_play_entity_packet_t;
void send_packet_S2C_play_entity(MConn *conn, varint_t entity_id,
                                 char **errmsg);
S2C_play_entity_packet_t unpack_S2C_play_entity_packet(MCbuffer *buff,
                                                       char **errmsg);
typedef struct {
  varint_t entity_id;
  char delta_x;
  char delta_y;
  char delta_z;
  bool on_ground;
} S2C_play_entity_relative_move_packet_t;
void send_packet_S2C_play_entity_relative_move(MConn *conn, varint_t entity_id,
                                               char delta_x, char delta_y,
                                               char delta_z, bool on_ground,
                                               char **errmsg);
S2C_play_entity_relative_move_packet_t
unpack_S2C_play_entity_relative_move_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  varint_t entity_id;
  byte_t yaw;
  byte_t pitch;
  bool on_ground;
} S2C_play_entity_look_packet_t;
void send_packet_S2C_play_entity_look(MConn *conn, varint_t entity_id,
                                      byte_t yaw, byte_t pitch, bool on_ground,
                                      char **errmsg);
S2C_play_entity_look_packet_t unpack_S2C_play_entity_look_packet(MCbuffer *buff,
                                                                 char **errmsg);
typedef struct {
  varint_t entity_id;
  char delta_x;
  char delta_y;
  char delta_z;
  byte_t yaw;
  byte_t pitch;
  bool on_ground;
} S2C_play_entity_look_and_relative_move_packet_t;
void send_packet_S2C_play_entity_look_and_relative_move(
    MConn *conn, varint_t entity_id, char delta_x, char delta_y, char delta_z,
    byte_t yaw, byte_t pitch, bool on_ground, char **errmsg);
S2C_play_entity_look_and_relative_move_packet_t
unpack_S2C_play_entity_look_and_relative_move_packet(MCbuffer *buff,
                                                     char **errmsg);
typedef struct {
  varint_t entity_id;
  int x;
  int y;
  int z;
  byte_t yaw;
  byte_t pitch;
  bool on_ground;
} S2C_play_entity_teleport_packet_t;
void send_packet_S2C_play_entity_teleport(MConn *conn, varint_t entity_id,
                                          int x, int y, int z, byte_t yaw,
                                          byte_t pitch, bool on_ground,
                                          char **errmsg);
S2C_play_entity_teleport_packet_t
unpack_S2C_play_entity_teleport_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  varint_t entity_id;
  byte_t head_yaw;
} S2C_play_entity_head_look_packet_t;
void send_packet_S2C_play_entity_head_look(MConn *conn, varint_t entity_id,
                                           byte_t head_yaw, char **errmsg);
S2C_play_entity_head_look_packet_t
unpack_S2C_play_entity_head_look_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  int entity_id;
  char entity_status;
} S2C_play_entity_status_packet_t;
void send_packet_S2C_play_entity_status(MConn *conn, int entity_id,
                                        char entity_status, char **errmsg);
S2C_play_entity_status_packet_t
unpack_S2C_play_entity_status_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  int entity_id;
  int vehicle_id;
  bool leash;
} S2C_play_attach_entity_packet_t;
void send_packet_S2C_play_attach_entity(MConn *conn, int entity_id,
                                        int vehicle_id, bool leash,
                                        char **errmsg);
S2C_play_attach_entity_packet_t
unpack_S2C_play_attach_entity_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  varint_t entity_id;
  entity_metadata_t meta_data;
} S2C_play_entity_metadata_packet_t;
void send_packet_S2C_play_entity_metadata(MConn *conn, varint_t entity_id,
                                          entity_metadata_t meta_data,
                                          char **errmsg);
S2C_play_entity_metadata_packet_t
unpack_S2C_play_entity_metadata_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  varint_t entity_id;
  char effect_id;
  char amplifier;
  varint_t duration;
  bool hide_particles;
} S2C_play_entity_effect_packet_t;
void send_packet_S2C_play_entity_effect(MConn *conn, varint_t entity_id,
                                        char effect_id, char amplifier,
                                        varint_t duration, bool hide_particles,
                                        char **errmsg);
S2C_play_entity_effect_packet_t
unpack_S2C_play_entity_effect_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  varint_t entity_id;
  char effect_id;
} S2C_play_remove_entity_effect_packet_t;
void send_packet_S2C_play_remove_entity_effect(MConn *conn, varint_t entity_id,
                                               char effect_id, char **errmsg);
S2C_play_remove_entity_effect_packet_t
unpack_S2C_play_remove_entity_effect_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  float experience_bar;
  varint_t level;
  varint_t total_experience;
} S2C_play_set_experience_packet_t;
void send_packet_S2C_play_set_experience(MConn *conn, float experience_bar,
                                         varint_t level,
                                         varint_t total_experience,
                                         char **errmsg);
S2C_play_set_experience_packet_t
unpack_S2C_play_set_experience_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  int chunk_x;
  int chunk_z;
  bool ground_up_continuous;
  unsigned short primary_bitmask;
  MCbuffer *chunk;
} S2C_play_chunk_data_packet_t;
void send_packet_S2C_play_chunk_data(MConn *conn, int chunk_x, int chunk_z,
                                     bool ground_up_continuous,
                                     unsigned short primary_bitmask,
                                     MCbuffer *chunk, char **errmsg);
S2C_play_chunk_data_packet_t unpack_S2C_play_chunk_data_packet(MCbuffer *buff,
                                                               char **errmsg);
typedef struct {
  block_pos_t location;
  varint_t block_id;
} S2C_play_block_change_packet_t;
void send_packet_S2C_play_block_change(MConn *conn, block_pos_t location,
                                       varint_t block_id, char **errmsg);
S2C_play_block_change_packet_t
unpack_S2C_play_block_change_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  block_pos_t location;
  byte_t block_data_1;
  byte_t block_data_2;
  varint_t block_type;
} S2C_play_block_action_packet_t;
void send_packet_S2C_play_block_action(MConn *conn, block_pos_t location,
                                       byte_t block_data_1, byte_t block_data_2,
                                       varint_t block_type, char **errmsg);
S2C_play_block_action_packet_t
unpack_S2C_play_block_action_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  varint_t entity_id;
  block_pos_t location;
  char destroy_stage;
} S2C_play_block_break_animation_packet_t;
void send_packet_S2C_play_block_break_animation(MConn *conn, varint_t entity_id,
                                                block_pos_t location,
                                                char destroy_stage,
                                                char **errmsg);
S2C_play_block_break_animation_packet_t
unpack_S2C_play_block_break_animation_packet(MCbuffer *buff, char **errmsg);
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
void send_packet_S2C_play_effect(MConn *conn, int effect_id,
                                 block_pos_t location, int data, bool d,
                                 int particle_id, bool long_distances, float x,
                                 float y, float z, float x_offset,
                                 float y_offset, float z_offset,
                                 float particle_data, int particle_count,
                                 int sable_relative_volume, char **errmsg);
S2C_play_effect_packet_t unpack_S2C_play_effect_packet(MCbuffer *buff,
                                                       char **errmsg);
typedef struct {
  char *sound_name;
  int x;
  int y;
  int z;
  float volume;
  byte_t pitch;
} S2C_play_sound_effect_packet_t;
void send_packet_S2C_play_sound_effect(MConn *conn, char *sound_name, int x,
                                       int y, int z, float volume, byte_t pitch,
                                       char **errmsg);
S2C_play_sound_effect_packet_t
unpack_S2C_play_sound_effect_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  byte_t reason;
  float value;
} S2C_play_change_game_state_packet_t;
void send_packet_S2C_play_change_game_state(MConn *conn, byte_t reason,
                                            float value, char **errmsg);
S2C_play_change_game_state_packet_t
unpack_S2C_play_change_game_state_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  char flags;
  float flying_speed;
  float fov_modifier;
} S2C_play_player_abilities_packet_t;
void send_packet_S2C_play_player_abilities(MConn *conn, char flags,
                                           float flying_speed,
                                           float fov_modifier, char **errmsg);
S2C_play_player_abilities_packet_t
unpack_S2C_play_player_abilities_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  char *channel;
  MCbuffer *data;
} S2C_play_plugin_message_packet_t;
void send_packet_S2C_play_plugin_message(MConn *conn, char *channel,
                                         MCbuffer *data, char **errmsg);
S2C_play_plugin_message_packet_t
unpack_S2C_play_plugin_message_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  char *reason;
} S2C_play_disconnect_packet_t;
void send_packet_S2C_play_disconnect(MConn *conn, char *reason, char **errmsg);
S2C_play_disconnect_packet_t unpack_S2C_play_disconnect_packet(MCbuffer *buff,
                                                               char **errmsg);
typedef struct {
  byte_t difficulty;
} S2C_play_change_difficulty_packet_t;
void send_packet_S2C_play_change_difficulty(MConn *conn, byte_t difficulty,
                                            char **errmsg);
S2C_play_change_difficulty_packet_t
unpack_S2C_play_change_difficulty_packet(MCbuffer *buff, char **errmsg);
typedef struct {
  varint_t keep_alive_id;
} C2S_play_keep_alive_packet_t;
void send_packet_C2S_play_keep_alive(MConn *conn, varint_t keep_alive_id,
                                     char **errmsg);
C2S_play_keep_alive_packet_t unpack_C2S_play_keep_alive_packet(MCbuffer *buff,
                                                               char **errmsg);