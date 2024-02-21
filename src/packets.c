#include "packets.h"
#include "heap_utils.h"
#include "buffer.h"
#include "conn.h"
#include "err.h"
#include "packet_types.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNPACK_ERR_HANDELER                                                    \
  ERR_CHECK(return packet;);                                                   \
  if (buff->position != buff->length)                                          \
    ERR(ERR_BUFFER_UNDERUN, return packet;);

char *packet_id_to_string(int packet_id, cmc_conn_state state,
                          packet_direction direction) {
#define PACKET_ID(packet_id, state, direction)                                 \
  (packet_id & 0x00FFFFFF) | (state << 24) | (direction << 23)

  int combined_packet_data =
      (packet_id & 0x00FFFFFF) | (state << 24) | (direction << 23);

  switch (combined_packet_data) {
    // CGSS: packet_id_to_string
  case PACKET_ID(0x00, CMC_CONN_STATE_HANDSHAKE, CMC_DIRECTION_C2S):
    return "C2S_handshake_handshake";
  case PACKET_ID(0x00, CMC_CONN_STATE_STATUS, CMC_DIRECTION_S2C):
    return "S2C_status_response";
  case PACKET_ID(0x01, CMC_CONN_STATE_STATUS, CMC_DIRECTION_S2C):
    return "S2C_status_pong";
  case PACKET_ID(0x00, CMC_CONN_STATE_STATUS, CMC_DIRECTION_C2S):
    return "C2S_status_request";
  case PACKET_ID(0x01, CMC_CONN_STATE_STATUS, CMC_DIRECTION_C2S):
    return "C2S_status_ping";
  case PACKET_ID(0x00, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_S2C):
    return "S2C_login_disconnect";
  case PACKET_ID(0x01, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_S2C):
    return "S2C_login_encryption_request";
  case PACKET_ID(0x02, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_S2C):
    return "S2C_login_success";
  case PACKET_ID(0x03, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_S2C):
    return "S2C_login_set_compression";
  case PACKET_ID(0x00, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_C2S):
    return "C2S_login_start";
  case PACKET_ID(0x01, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_C2S):
    return "C2S_login_encryption_response";
  case PACKET_ID(0x00, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_keep_alive";
  case PACKET_ID(0x01, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_join_game";
  case PACKET_ID(0x02, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_chat_message";
  case PACKET_ID(0x03, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_time_update";
  case PACKET_ID(0x04, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_entity_equipment";
  case PACKET_ID(0x05, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_spawn_position";
  case PACKET_ID(0x06, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_update_health";
  case PACKET_ID(0x07, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_respawn";
  case PACKET_ID(0x08, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_player_look_and_position";
  case PACKET_ID(0x09, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_held_item_change";
  case PACKET_ID(0x0A, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_use_bed";
  case PACKET_ID(0x0B, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_animation";
  case PACKET_ID(0x0C, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_spawn_player";
  case PACKET_ID(0x0D, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_collect_item";
  case PACKET_ID(0x0F, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_spawn_mob";
  case PACKET_ID(0x10, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_spawn_painting";
  case PACKET_ID(0x11, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_spawn_experience_orb";
  case PACKET_ID(0x12, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_entity_velocity";
  case PACKET_ID(0x14, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_entity";
  case PACKET_ID(0x15, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_entity_relative_move";
  case PACKET_ID(0x16, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_entity_look";
  case PACKET_ID(0x17, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_entity_look_and_relative_move";
  case PACKET_ID(0x18, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_entity_teleport";
  case PACKET_ID(0x19, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_entity_head_look";
  case PACKET_ID(0x1A, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_entity_status";
  case PACKET_ID(0x1B, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_attach_entity";
  case PACKET_ID(0x1C, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_entity_metadata";
  case PACKET_ID(0x1D, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_entity_effect";
  case PACKET_ID(0x1E, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_remove_entity_effect";
  case PACKET_ID(0x1F, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_set_experience";
  case PACKET_ID(0x21, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_chunk_data";
  case PACKET_ID(0x23, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_block_change";
  case PACKET_ID(0x24, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_block_action";
  case PACKET_ID(0x25, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_block_break_animation";
  case PACKET_ID(0x28, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_effect";
  case PACKET_ID(0x29, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_sound_effect";
  case PACKET_ID(0x2B, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_change_game_state";
  case PACKET_ID(0x39, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_player_abilities";
  case PACKET_ID(0x3F, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_plugin_message";
  case PACKET_ID(0x40, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_disconnect";
  case PACKET_ID(0x41, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_change_difficulty";
  case PACKET_ID(0x00, CMC_CONN_STATE_PLAY, CMC_DIRECTION_C2S):
    return "C2S_play_keep_alive";
    // CGSE: packet_id_to_string
  default:
    return "PACKET_UNKNOWN";
  }

#undef PACKET_ID_TO_STRING_UTIL
}

// CGSS: send_methods_c
void send_packet_C2S_handshake_handshake(struct cmc_conn *conn,
                                         int protocole_version,
                                         char *server_addr,
                                         unsigned short server_port,
                                         int next_state) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_C2S_HANDSHAKE_HANDSHAKE);
  cmc_buffer_pack_varint(buff, protocole_version);
  cmc_buffer_pack_string(buff, server_addr);
  cmc_buffer_pack_ushort(buff, server_port);
  cmc_buffer_pack_varint(buff, next_state);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_status_response(struct cmc_conn *conn, char *response) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_STATUS_RESPONSE);
  cmc_buffer_pack_string(buff, response);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_status_pong(struct cmc_conn *conn, long payload) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_STATUS_PONG);
  cmc_buffer_pack_long(buff, payload);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_C2S_status_request(struct cmc_conn *conn) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_C2S_STATUS_REQUEST);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_C2S_status_ping(struct cmc_conn *conn, long payload) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_C2S_STATUS_PING);
  cmc_buffer_pack_long(buff, payload);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_login_disconnect(struct cmc_conn *conn, char *reason) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_LOGIN_DISCONNECT);
  cmc_buffer_pack_string(buff, reason);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_login_encryption_request(struct cmc_conn *conn,
                                              char *server_id,
                                              cmc_buffer *public_key,
                                              cmc_buffer *verify_token) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_LOGIN_ENCRYPTION_REQUEST);
  cmc_buffer_pack_string(buff, server_id);
  cmc_buffer_pack_byte_array(buff, public_key);
  cmc_buffer_pack_byte_array(buff, verify_token);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_login_success(struct cmc_conn *conn, char *uuid,
                                   char *name) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_LOGIN_SUCCESS);
  cmc_buffer_pack_string(buff, uuid);
  cmc_buffer_pack_string(buff, name);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_login_set_compression(struct cmc_conn *conn,
                                           int threshold) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_LOGIN_SET_COMPRESSION);
  cmc_buffer_pack_varint(buff, threshold);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_C2S_login_start(struct cmc_conn *conn, char *name) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_C2S_LOGIN_START);
  cmc_buffer_pack_string(buff, name);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_C2S_login_encryption_response(struct cmc_conn *conn,
                                               cmc_buffer *shared_secret,
                                               cmc_buffer *verify_token) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_C2S_LOGIN_ENCRYPTION_RESPONSE);
  cmc_buffer_pack_byte_array(buff, shared_secret);
  cmc_buffer_pack_byte_array(buff, verify_token);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_keep_alive(struct cmc_conn *conn, int keep_alive_id) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_KEEP_ALIVE);
  cmc_buffer_pack_varint(buff, keep_alive_id);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_join_game(struct cmc_conn *conn, int entity_id,
                                    unsigned char gamemode, char dimension,
                                    unsigned char difficulty,
                                    unsigned char max_players, char *level_type,
                                    bool reduced_debug_info) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_JOIN_GAME);
  cmc_buffer_pack_int(buff, entity_id);
  cmc_buffer_pack_byte(buff, gamemode);
  cmc_buffer_pack_char(buff, dimension);
  cmc_buffer_pack_byte(buff, difficulty);
  cmc_buffer_pack_byte(buff, max_players);
  cmc_buffer_pack_string(buff, level_type);
  cmc_buffer_pack_bool(buff, reduced_debug_info);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_chat_message(struct cmc_conn *conn, char *message,
                                       char position) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_CHAT_MESSAGE);
  cmc_buffer_pack_string(buff, message);
  cmc_buffer_pack_char(buff, position);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_time_update(struct cmc_conn *conn, long world_age,
                                      long time_of_day) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_TIME_UPDATE);
  cmc_buffer_pack_long(buff, world_age);
  cmc_buffer_pack_long(buff, time_of_day);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_equipment(struct cmc_conn *conn, int entity_id,
                                           short slot, cmc_slot *item) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_ENTITY_EQUIPMENT);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_short(buff, slot);
  cmc_buffer_pack_slot(buff, item);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_spawn_position(struct cmc_conn *conn,
                                         cmc_block_pos location) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_SPAWN_POSITION);
  cmc_buffer_pack_position(buff, location);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_update_health(struct cmc_conn *conn, float health,
                                        int food, float food_saturation) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_UPDATE_HEALTH);
  cmc_buffer_pack_float(buff, health);
  cmc_buffer_pack_varint(buff, food);
  cmc_buffer_pack_float(buff, food_saturation);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_respawn(struct cmc_conn *conn, int dimesion,
                                  unsigned char difficulty,
                                  unsigned char gamemode, char *level_type) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_RESPAWN);
  cmc_buffer_pack_int(buff, dimesion);
  cmc_buffer_pack_byte(buff, difficulty);
  cmc_buffer_pack_byte(buff, gamemode);
  cmc_buffer_pack_string(buff, level_type);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_player_look_and_position(struct cmc_conn *conn,
                                                   double x, double y, double z,
                                                   float yaw, float pitch,
                                                   unsigned char flags) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_PLAYER_LOOK_AND_POSITION);
  cmc_buffer_pack_double(buff, x);
  cmc_buffer_pack_double(buff, y);
  cmc_buffer_pack_double(buff, z);
  cmc_buffer_pack_float(buff, yaw);
  cmc_buffer_pack_float(buff, pitch);
  cmc_buffer_pack_byte(buff, flags);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_held_item_change(struct cmc_conn *conn, char slot) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_HELD_ITEM_CHANGE);
  cmc_buffer_pack_char(buff, slot);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_use_bed(struct cmc_conn *conn, int entity_id,
                                  cmc_block_pos location) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_USE_BED);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_position(buff, location);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_animation(struct cmc_conn *conn, int entity_id,
                                    unsigned char animation) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_ANIMATION);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_byte(buff, animation);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_spawn_player(struct cmc_conn *conn, int entity_id,
                                       unsigned long long uuid, int x, int y,
                                       int z, unsigned char yaw,
                                       unsigned char pitch, short current_item,
                                       cmc_entity_metadata meta_data) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_SPAWN_PLAYER);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_ullong(buff, uuid);
  cmc_buffer_pack_int(buff, x);
  cmc_buffer_pack_int(buff, y);
  cmc_buffer_pack_int(buff, z);
  cmc_buffer_pack_byte(buff, yaw);
  cmc_buffer_pack_byte(buff, pitch);
  cmc_buffer_pack_short(buff, current_item);
  cmc_buffer_pack_entity_metadata(buff, meta_data);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_collect_item(struct cmc_conn *conn,
                                       int collected_entity_id,
                                       int collector_entity_id) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_COLLECT_ITEM);
  cmc_buffer_pack_varint(buff, collected_entity_id);
  cmc_buffer_pack_varint(buff, collector_entity_id);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_spawn_mob(struct cmc_conn *conn, int entity_id,
                                    unsigned char type, int x, int y, int z,
                                    unsigned char yaw, unsigned char pitch,
                                    unsigned char head_pitch, short x_vel,
                                    short y_vel, short z_vel,
                                    cmc_entity_metadata meta_data) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_SPAWN_MOB);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_byte(buff, type);
  cmc_buffer_pack_int(buff, x);
  cmc_buffer_pack_int(buff, y);
  cmc_buffer_pack_int(buff, z);
  cmc_buffer_pack_byte(buff, yaw);
  cmc_buffer_pack_byte(buff, pitch);
  cmc_buffer_pack_byte(buff, head_pitch);
  cmc_buffer_pack_short(buff, x_vel);
  cmc_buffer_pack_short(buff, y_vel);
  cmc_buffer_pack_short(buff, z_vel);
  cmc_buffer_pack_entity_metadata(buff, meta_data);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_spawn_painting(struct cmc_conn *conn, int entity_id,
                                         char *title, cmc_block_pos location,
                                         unsigned char direction) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_SPAWN_PAINTING);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_string(buff, title);
  cmc_buffer_pack_position(buff, location);
  cmc_buffer_pack_byte(buff, direction);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_spawn_experience_orb(struct cmc_conn *conn,
                                               int entity_id, int x, int y,
                                               int z, short count) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_SPAWN_EXPERIENCE_ORB);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_int(buff, x);
  cmc_buffer_pack_int(buff, y);
  cmc_buffer_pack_int(buff, z);
  cmc_buffer_pack_short(buff, count);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_velocity(struct cmc_conn *conn, int entity_id,
                                          short x_vel, short y_vel,
                                          short z_vel) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_ENTITY_VELOCITY);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_short(buff, x_vel);
  cmc_buffer_pack_short(buff, y_vel);
  cmc_buffer_pack_short(buff, z_vel);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity(struct cmc_conn *conn, int entity_id) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_ENTITY);
  cmc_buffer_pack_varint(buff, entity_id);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_relative_move(struct cmc_conn *conn,
                                               int entity_id, char delta_x,
                                               char delta_y, char delta_z,
                                               bool on_ground) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_ENTITY_RELATIVE_MOVE);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_char(buff, delta_x);
  cmc_buffer_pack_char(buff, delta_y);
  cmc_buffer_pack_char(buff, delta_z);
  cmc_buffer_pack_bool(buff, on_ground);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_look(struct cmc_conn *conn, int entity_id,
                                      unsigned char yaw, unsigned char pitch,
                                      bool on_ground) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_ENTITY_LOOK);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_byte(buff, yaw);
  cmc_buffer_pack_byte(buff, pitch);
  cmc_buffer_pack_bool(buff, on_ground);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_look_and_relative_move(
    struct cmc_conn *conn, int entity_id, char delta_x, char delta_y,
    char delta_z, unsigned char yaw, unsigned char pitch, bool on_ground) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff,
                         CMC_PACKETID_S2C_PLAY_ENTITY_LOOK_AND_RELATIVE_MOVE);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_char(buff, delta_x);
  cmc_buffer_pack_char(buff, delta_y);
  cmc_buffer_pack_char(buff, delta_z);
  cmc_buffer_pack_byte(buff, yaw);
  cmc_buffer_pack_byte(buff, pitch);
  cmc_buffer_pack_bool(buff, on_ground);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_teleport(struct cmc_conn *conn, int entity_id,
                                          int x, int y, int z,
                                          unsigned char yaw,
                                          unsigned char pitch, bool on_ground) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_ENTITY_TELEPORT);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_int(buff, x);
  cmc_buffer_pack_int(buff, y);
  cmc_buffer_pack_int(buff, z);
  cmc_buffer_pack_byte(buff, yaw);
  cmc_buffer_pack_byte(buff, pitch);
  cmc_buffer_pack_bool(buff, on_ground);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_head_look(struct cmc_conn *conn, int entity_id,
                                           unsigned char head_yaw) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_ENTITY_HEAD_LOOK);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_byte(buff, head_yaw);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_status(struct cmc_conn *conn, int entity_id,
                                        char entity_status) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_ENTITY_STATUS);
  cmc_buffer_pack_int(buff, entity_id);
  cmc_buffer_pack_char(buff, entity_status);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_attach_entity(struct cmc_conn *conn, int entity_id,
                                        int vehicle_id, bool leash) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_ATTACH_ENTITY);
  cmc_buffer_pack_int(buff, entity_id);
  cmc_buffer_pack_int(buff, vehicle_id);
  cmc_buffer_pack_bool(buff, leash);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_metadata(struct cmc_conn *conn, int entity_id,
                                          cmc_entity_metadata meta_data) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_ENTITY_METADATA);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_entity_metadata(buff, meta_data);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_effect(struct cmc_conn *conn, int entity_id,
                                        char effect_id, char amplifier,
                                        int duration, bool hide_particles) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_ENTITY_EFFECT);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_char(buff, effect_id);
  cmc_buffer_pack_char(buff, amplifier);
  cmc_buffer_pack_varint(buff, duration);
  cmc_buffer_pack_bool(buff, hide_particles);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_remove_entity_effect(struct cmc_conn *conn,
                                               int entity_id, char effect_id) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_REMOVE_ENTITY_EFFECT);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_char(buff, effect_id);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_set_experience(struct cmc_conn *conn,
                                         float experience_bar, int level,
                                         int total_experience) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_SET_EXPERIENCE);
  cmc_buffer_pack_float(buff, experience_bar);
  cmc_buffer_pack_varint(buff, level);
  cmc_buffer_pack_varint(buff, total_experience);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_chunk_data(struct cmc_conn *conn, int chunk_x,
                                     int chunk_z, bool ground_up_continuous,
                                     unsigned short primary_bitmask,
                                     cmc_buffer *chunk) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_CHUNK_DATA);
  cmc_buffer_pack_int(buff, chunk_x);
  cmc_buffer_pack_int(buff, chunk_z);
  cmc_buffer_pack_bool(buff, ground_up_continuous);
  cmc_buffer_pack_ushort(buff, primary_bitmask);
  cmc_buffer_pack_byte_array(buff, chunk);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_block_change(struct cmc_conn *conn,
                                       cmc_block_pos location, int block_id) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_BLOCK_CHANGE);
  cmc_buffer_pack_position(buff, location);
  cmc_buffer_pack_varint(buff, block_id);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_block_action(struct cmc_conn *conn,
                                       cmc_block_pos location,
                                       unsigned char block_data_1,
                                       unsigned char block_data_2,
                                       int block_type) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_BLOCK_ACTION);
  cmc_buffer_pack_position(buff, location);
  cmc_buffer_pack_byte(buff, block_data_1);
  cmc_buffer_pack_byte(buff, block_data_2);
  cmc_buffer_pack_varint(buff, block_type);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_block_break_animation(struct cmc_conn *conn,
                                                int entity_id,
                                                cmc_block_pos location,
                                                char destroy_stage) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_BLOCK_BREAK_ANIMATION);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_position(buff, location);
  cmc_buffer_pack_char(buff, destroy_stage);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_effect(struct cmc_conn *conn, int effect_id,
                                 cmc_block_pos location, int data, bool d,
                                 int particle_id, bool long_distances, float x,
                                 float y, float z, float x_offset,
                                 float y_offset, float z_offset,
                                 float particle_data, int particle_count,
                                 int sable_relative_volume) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_EFFECT);
  cmc_buffer_pack_int(buff, effect_id);
  cmc_buffer_pack_position(buff, location);
  cmc_buffer_pack_int(buff, data);
  cmc_buffer_pack_bool(buff, d);
  cmc_buffer_pack_int(buff, particle_id);
  cmc_buffer_pack_bool(buff, long_distances);
  cmc_buffer_pack_float(buff, x);
  cmc_buffer_pack_float(buff, y);
  cmc_buffer_pack_float(buff, z);
  cmc_buffer_pack_float(buff, x_offset);
  cmc_buffer_pack_float(buff, y_offset);
  cmc_buffer_pack_float(buff, z_offset);
  cmc_buffer_pack_float(buff, particle_data);
  cmc_buffer_pack_int(buff, particle_count);
  cmc_buffer_pack_int(buff, sable_relative_volume);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_sound_effect(struct cmc_conn *conn, char *sound_name,
                                       int x, int y, int z, float volume,
                                       unsigned char pitch) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_SOUND_EFFECT);
  cmc_buffer_pack_string(buff, sound_name);
  cmc_buffer_pack_int(buff, x);
  cmc_buffer_pack_int(buff, y);
  cmc_buffer_pack_int(buff, z);
  cmc_buffer_pack_float(buff, volume);
  cmc_buffer_pack_byte(buff, pitch);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_change_game_state(struct cmc_conn *conn,
                                            unsigned char reason, float value) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_CHANGE_GAME_STATE);
  cmc_buffer_pack_byte(buff, reason);
  cmc_buffer_pack_float(buff, value);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_player_abilities(struct cmc_conn *conn, char flags,
                                           float flying_speed,
                                           float fov_modifier) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_PLAYER_ABILITIES);
  cmc_buffer_pack_char(buff, flags);
  cmc_buffer_pack_float(buff, flying_speed);
  cmc_buffer_pack_float(buff, fov_modifier);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_plugin_message(struct cmc_conn *conn, char *channel,
                                         cmc_buffer *data) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_PLUGIN_MESSAGE);
  cmc_buffer_pack_string(buff, channel);
  cmc_buffer_pack_byte_array(buff, data);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_disconnect(struct cmc_conn *conn, char *reason) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_DISCONNECT);
  cmc_buffer_pack_string(buff, reason);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_change_difficulty(struct cmc_conn *conn,
                                            unsigned char difficulty) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_S2C_PLAY_CHANGE_DIFFICULTY);
  cmc_buffer_pack_byte(buff, difficulty);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

void send_packet_C2S_play_keep_alive(struct cmc_conn *conn, int keep_alive_id) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, CMC_PACKETID_C2S_PLAY_KEEP_ALIVE);
  cmc_buffer_pack_varint(buff, keep_alive_id);
  ERR_CHECK(return;);
  cmc_conn_send_packet(conn, buff);
}

// CGSE: send_methods_c

// CGSS: unpack_methods_c
C2S_handshake_handshake_packet *unpack_C2S_handshake_handshake_packet(cmc_buffer *buff) {C2S_handshake_handshake_packet packet;switch(buff->protocol_version) {case 47: {packet.protocole_version=cmc_buffer_unpack_varint(buff);packet.server_addr=cmc_buffer_unpack_string(buff);packet.server_port=cmc_buffer_unpack_ushort(buff);packet.next_state=cmc_buffer_unpack_varint(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_status_response_packet *unpack_S2C_status_response_packet(cmc_buffer *buff) {S2C_status_response_packet packet;switch(buff->protocol_version) {case 47: {packet.response=cmc_buffer_unpack_string(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_status_pong_packet *unpack_S2C_status_pong_packet(cmc_buffer *buff) {S2C_status_pong_packet packet;switch(buff->protocol_version) {case 47: {packet.payload=cmc_buffer_unpack_long(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

C2S_status_ping_packet *unpack_C2S_status_ping_packet(cmc_buffer *buff) {C2S_status_ping_packet packet;switch(buff->protocol_version) {case 47: {packet.payload=cmc_buffer_unpack_long(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_login_disconnect_packet *unpack_S2C_login_disconnect_packet(cmc_buffer *buff) {S2C_login_disconnect_packet packet;switch(buff->protocol_version) {case 47: {packet.reason=cmc_buffer_unpack_string(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_login_encryption_request_packet *unpack_S2C_login_encryption_request_packet(cmc_buffer *buff) {S2C_login_encryption_request_packet packet;switch(buff->protocol_version) {case 47: {packet.server_id=cmc_buffer_unpack_string(buff);packet.public_key=cmc_buffer_unpack_byte_array(buff);packet.verify_token=cmc_buffer_unpack_byte_array(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_login_success_packet *unpack_S2C_login_success_packet(cmc_buffer *buff) {S2C_login_success_packet packet;switch(buff->protocol_version) {case 47: {packet.uuid=cmc_buffer_unpack_string(buff);packet.name=cmc_buffer_unpack_string(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_login_set_compression_packet *unpack_S2C_login_set_compression_packet(cmc_buffer *buff) {S2C_login_set_compression_packet packet;switch(buff->protocol_version) {case 47: {packet.threshold=cmc_buffer_unpack_varint(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

C2S_login_start_packet *unpack_C2S_login_start_packet(cmc_buffer *buff) {C2S_login_start_packet packet;switch(buff->protocol_version) {case 47: {packet.name=cmc_buffer_unpack_string(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

C2S_login_encryption_response_packet *unpack_C2S_login_encryption_response_packet(cmc_buffer *buff) {C2S_login_encryption_response_packet packet;switch(buff->protocol_version) {case 47: {packet.shared_secret=cmc_buffer_unpack_byte_array(buff);packet.verify_token=cmc_buffer_unpack_byte_array(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_keep_alive_packet *unpack_S2C_play_keep_alive_packet(cmc_buffer *buff) {S2C_play_keep_alive_packet packet;switch(buff->protocol_version) {case 47: {packet.keep_alive_id=cmc_buffer_unpack_varint(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_join_game_packet *unpack_S2C_play_join_game_packet(cmc_buffer *buff) {S2C_play_join_game_packet packet;switch(buff->protocol_version) {case 47: {packet.entity_id=cmc_buffer_unpack_int(buff);packet.gamemode=cmc_buffer_unpack_byte(buff);packet.dimension=cmc_buffer_unpack_char(buff);packet.difficulty=cmc_buffer_unpack_byte(buff);packet.max_players=cmc_buffer_unpack_byte(buff);packet.level_type=cmc_buffer_unpack_string(buff);packet.reduced_debug_info=cmc_buffer_unpack_bool(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_chat_message_packet *unpack_S2C_play_chat_message_packet(cmc_buffer *buff) {S2C_play_chat_message_packet packet;switch(buff->protocol_version) {case 47: {packet.message=cmc_buffer_unpack_string(buff);packet.position=cmc_buffer_unpack_char(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_time_update_packet *unpack_S2C_play_time_update_packet(cmc_buffer *buff) {S2C_play_time_update_packet packet;switch(buff->protocol_version) {case 47: {packet.world_age=cmc_buffer_unpack_long(buff);packet.time_of_day=cmc_buffer_unpack_long(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_entity_equipment_packet *unpack_S2C_play_entity_equipment_packet(cmc_buffer *buff) {S2C_play_entity_equipment_packet packet;switch(buff->protocol_version) {case 47: {packet.entity_id=cmc_buffer_unpack_varint(buff);packet.slot=cmc_buffer_unpack_short(buff);packet.item=cmc_buffer_unpack_slot(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_spawn_position_packet *unpack_S2C_play_spawn_position_packet(cmc_buffer *buff) {S2C_play_spawn_position_packet packet;switch(buff->protocol_version) {case 47: {packet.location=cmc_buffer_unpack_position(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_update_health_packet *unpack_S2C_play_update_health_packet(cmc_buffer *buff) {S2C_play_update_health_packet packet;switch(buff->protocol_version) {case 47: {packet.health=cmc_buffer_unpack_float(buff);packet.food=cmc_buffer_unpack_varint(buff);packet.food_saturation=cmc_buffer_unpack_float(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_respawn_packet *unpack_S2C_play_respawn_packet(cmc_buffer *buff) {S2C_play_respawn_packet packet;switch(buff->protocol_version) {case 47: {packet.dimesion=cmc_buffer_unpack_int(buff);packet.difficulty=cmc_buffer_unpack_byte(buff);packet.gamemode=cmc_buffer_unpack_byte(buff);packet.level_type=cmc_buffer_unpack_string(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_player_look_and_position_packet *unpack_S2C_play_player_look_and_position_packet(cmc_buffer *buff) {S2C_play_player_look_and_position_packet packet;switch(buff->protocol_version) {case 47: {packet.x=cmc_buffer_unpack_double(buff);packet.y=cmc_buffer_unpack_double(buff);packet.z=cmc_buffer_unpack_double(buff);packet.yaw=cmc_buffer_unpack_float(buff);packet.pitch=cmc_buffer_unpack_float(buff);packet.flags=cmc_buffer_unpack_byte(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_held_item_change_packet *unpack_S2C_play_held_item_change_packet(cmc_buffer *buff) {S2C_play_held_item_change_packet packet;switch(buff->protocol_version) {case 47: {packet.slot=cmc_buffer_unpack_char(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_use_bed_packet *unpack_S2C_play_use_bed_packet(cmc_buffer *buff) {S2C_play_use_bed_packet packet;switch(buff->protocol_version) {case 47: {packet.entity_id=cmc_buffer_unpack_varint(buff);packet.location=cmc_buffer_unpack_position(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_animation_packet *unpack_S2C_play_animation_packet(cmc_buffer *buff) {S2C_play_animation_packet packet;switch(buff->protocol_version) {case 47: {packet.entity_id=cmc_buffer_unpack_varint(buff);packet.animation=cmc_buffer_unpack_byte(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_spawn_player_packet *unpack_S2C_play_spawn_player_packet(cmc_buffer *buff) {S2C_play_spawn_player_packet packet;switch(buff->protocol_version) {case 47: {packet.entity_id=cmc_buffer_unpack_varint(buff);packet.uuid=cmc_buffer_unpack_ullong(buff);packet.x=cmc_buffer_unpack_int(buff);packet.y=cmc_buffer_unpack_int(buff);packet.z=cmc_buffer_unpack_int(buff);packet.yaw=cmc_buffer_unpack_byte(buff);packet.pitch=cmc_buffer_unpack_byte(buff);packet.current_item=cmc_buffer_unpack_short(buff);packet.meta_data=cmc_buffer_unpack_entity_metadata(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_collect_item_packet *unpack_S2C_play_collect_item_packet(cmc_buffer *buff) {S2C_play_collect_item_packet packet;switch(buff->protocol_version) {case 47: {packet.collected_entity_id=cmc_buffer_unpack_varint(buff);packet.collector_entity_id=cmc_buffer_unpack_varint(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_spawn_mob_packet *unpack_S2C_play_spawn_mob_packet(cmc_buffer *buff) {S2C_play_spawn_mob_packet packet;switch(buff->protocol_version) {case 47: {packet.entity_id=cmc_buffer_unpack_varint(buff);packet.type=cmc_buffer_unpack_byte(buff);packet.x=cmc_buffer_unpack_int(buff);packet.y=cmc_buffer_unpack_int(buff);packet.z=cmc_buffer_unpack_int(buff);packet.yaw=cmc_buffer_unpack_byte(buff);packet.pitch=cmc_buffer_unpack_byte(buff);packet.head_pitch=cmc_buffer_unpack_byte(buff);packet.x_vel=cmc_buffer_unpack_short(buff);packet.y_vel=cmc_buffer_unpack_short(buff);packet.z_vel=cmc_buffer_unpack_short(buff);packet.meta_data=cmc_buffer_unpack_entity_metadata(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_spawn_painting_packet *unpack_S2C_play_spawn_painting_packet(cmc_buffer *buff) {S2C_play_spawn_painting_packet packet;switch(buff->protocol_version) {case 47: {packet.entity_id=cmc_buffer_unpack_varint(buff);packet.title=cmc_buffer_unpack_string(buff);packet.location=cmc_buffer_unpack_position(buff);packet.direction=cmc_buffer_unpack_byte(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_spawn_experience_orb_packet *unpack_S2C_play_spawn_experience_orb_packet(cmc_buffer *buff) {S2C_play_spawn_experience_orb_packet packet;switch(buff->protocol_version) {case 47: {packet.entity_id=cmc_buffer_unpack_varint(buff);packet.x=cmc_buffer_unpack_int(buff);packet.y=cmc_buffer_unpack_int(buff);packet.z=cmc_buffer_unpack_int(buff);packet.count=cmc_buffer_unpack_short(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_entity_velocity_packet *unpack_S2C_play_entity_velocity_packet(cmc_buffer *buff) {S2C_play_entity_velocity_packet packet;switch(buff->protocol_version) {case 47: {packet.entity_id=cmc_buffer_unpack_varint(buff);packet.x_vel=cmc_buffer_unpack_short(buff);packet.y_vel=cmc_buffer_unpack_short(buff);packet.z_vel=cmc_buffer_unpack_short(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_entity_packet *unpack_S2C_play_entity_packet(cmc_buffer *buff) {S2C_play_entity_packet packet;switch(buff->protocol_version) {case 47: {packet.entity_id=cmc_buffer_unpack_varint(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_entity_relative_move_packet *unpack_S2C_play_entity_relative_move_packet(cmc_buffer *buff) {S2C_play_entity_relative_move_packet packet;switch(buff->protocol_version) {case 47: {packet.entity_id=cmc_buffer_unpack_varint(buff);packet.delta_x=cmc_buffer_unpack_char(buff);packet.delta_y=cmc_buffer_unpack_char(buff);packet.delta_z=cmc_buffer_unpack_char(buff);packet.on_ground=cmc_buffer_unpack_bool(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_entity_look_packet *unpack_S2C_play_entity_look_packet(cmc_buffer *buff) {S2C_play_entity_look_packet packet;switch(buff->protocol_version) {case 47: {packet.entity_id=cmc_buffer_unpack_varint(buff);packet.yaw=cmc_buffer_unpack_byte(buff);packet.pitch=cmc_buffer_unpack_byte(buff);packet.on_ground=cmc_buffer_unpack_bool(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_entity_look_and_relative_move_packet *unpack_S2C_play_entity_look_and_relative_move_packet(cmc_buffer *buff) {S2C_play_entity_look_and_relative_move_packet packet;switch(buff->protocol_version) {case 47: {packet.entity_id=cmc_buffer_unpack_varint(buff);packet.delta_x=cmc_buffer_unpack_char(buff);packet.delta_y=cmc_buffer_unpack_char(buff);packet.delta_z=cmc_buffer_unpack_char(buff);packet.yaw=cmc_buffer_unpack_byte(buff);packet.pitch=cmc_buffer_unpack_byte(buff);packet.on_ground=cmc_buffer_unpack_bool(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_entity_teleport_packet *unpack_S2C_play_entity_teleport_packet(cmc_buffer *buff) {S2C_play_entity_teleport_packet packet;switch(buff->protocol_version) {case 47: {packet.entity_id=cmc_buffer_unpack_varint(buff);packet.x=cmc_buffer_unpack_int(buff);packet.y=cmc_buffer_unpack_int(buff);packet.z=cmc_buffer_unpack_int(buff);packet.yaw=cmc_buffer_unpack_byte(buff);packet.pitch=cmc_buffer_unpack_byte(buff);packet.on_ground=cmc_buffer_unpack_bool(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_entity_head_look_packet *unpack_S2C_play_entity_head_look_packet(cmc_buffer *buff) {S2C_play_entity_head_look_packet packet;switch(buff->protocol_version) {case 47: {packet.entity_id=cmc_buffer_unpack_varint(buff);packet.head_yaw=cmc_buffer_unpack_byte(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_entity_status_packet *unpack_S2C_play_entity_status_packet(cmc_buffer *buff) {S2C_play_entity_status_packet packet;switch(buff->protocol_version) {case 47: {packet.entity_id=cmc_buffer_unpack_int(buff);packet.entity_status=cmc_buffer_unpack_char(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_attach_entity_packet *unpack_S2C_play_attach_entity_packet(cmc_buffer *buff) {S2C_play_attach_entity_packet packet;switch(buff->protocol_version) {case 47: {packet.entity_id=cmc_buffer_unpack_int(buff);packet.vehicle_id=cmc_buffer_unpack_int(buff);packet.leash=cmc_buffer_unpack_bool(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_entity_metadata_packet *unpack_S2C_play_entity_metadata_packet(cmc_buffer *buff) {S2C_play_entity_metadata_packet packet;switch(buff->protocol_version) {case 47: {packet.entity_id=cmc_buffer_unpack_varint(buff);packet.meta_data=cmc_buffer_unpack_entity_metadata(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_entity_effect_packet *unpack_S2C_play_entity_effect_packet(cmc_buffer *buff) {S2C_play_entity_effect_packet packet;switch(buff->protocol_version) {case 47: {packet.entity_id=cmc_buffer_unpack_varint(buff);packet.effect_id=cmc_buffer_unpack_char(buff);packet.amplifier=cmc_buffer_unpack_char(buff);packet.duration=cmc_buffer_unpack_varint(buff);packet.hide_particles=cmc_buffer_unpack_bool(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_remove_entity_effect_packet *unpack_S2C_play_remove_entity_effect_packet(cmc_buffer *buff) {S2C_play_remove_entity_effect_packet packet;switch(buff->protocol_version) {case 47: {packet.entity_id=cmc_buffer_unpack_varint(buff);packet.effect_id=cmc_buffer_unpack_char(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_set_experience_packet *unpack_S2C_play_set_experience_packet(cmc_buffer *buff) {S2C_play_set_experience_packet packet;switch(buff->protocol_version) {case 47: {packet.experience_bar=cmc_buffer_unpack_float(buff);packet.level=cmc_buffer_unpack_varint(buff);packet.total_experience=cmc_buffer_unpack_varint(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_chunk_data_packet *unpack_S2C_play_chunk_data_packet(cmc_buffer *buff) {S2C_play_chunk_data_packet packet;switch(buff->protocol_version) {case 47: {packet.chunk_x=cmc_buffer_unpack_int(buff);packet.chunk_z=cmc_buffer_unpack_int(buff);packet.ground_up_continuous=cmc_buffer_unpack_bool(buff);packet.primary_bitmask=cmc_buffer_unpack_ushort(buff);packet.chunk=cmc_buffer_unpack_byte_array(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_block_change_packet *unpack_S2C_play_block_change_packet(cmc_buffer *buff) {S2C_play_block_change_packet packet;switch(buff->protocol_version) {case 47: {packet.location=cmc_buffer_unpack_position(buff);packet.block_id=cmc_buffer_unpack_varint(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_block_action_packet *unpack_S2C_play_block_action_packet(cmc_buffer *buff) {S2C_play_block_action_packet packet;switch(buff->protocol_version) {case 47: {packet.location=cmc_buffer_unpack_position(buff);packet.block_data_1=cmc_buffer_unpack_byte(buff);packet.block_data_2=cmc_buffer_unpack_byte(buff);packet.block_type=cmc_buffer_unpack_varint(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_block_break_animation_packet *unpack_S2C_play_block_break_animation_packet(cmc_buffer *buff) {S2C_play_block_break_animation_packet packet;switch(buff->protocol_version) {case 47: {packet.entity_id=cmc_buffer_unpack_varint(buff);packet.location=cmc_buffer_unpack_position(buff);packet.destroy_stage=cmc_buffer_unpack_char(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_effect_packet *unpack_S2C_play_effect_packet(cmc_buffer *buff) {S2C_play_effect_packet packet;switch(buff->protocol_version) {case 47: {packet.effect_id=cmc_buffer_unpack_int(buff);packet.location=cmc_buffer_unpack_position(buff);packet.data=cmc_buffer_unpack_int(buff);packet.d=cmc_buffer_unpack_bool(buff);packet.particle_id=cmc_buffer_unpack_int(buff);packet.long_distances=cmc_buffer_unpack_bool(buff);packet.x=cmc_buffer_unpack_float(buff);packet.y=cmc_buffer_unpack_float(buff);packet.z=cmc_buffer_unpack_float(buff);packet.x_offset=cmc_buffer_unpack_float(buff);packet.y_offset=cmc_buffer_unpack_float(buff);packet.z_offset=cmc_buffer_unpack_float(buff);packet.particle_data=cmc_buffer_unpack_float(buff);packet.particle_count=cmc_buffer_unpack_int(buff);packet.sable_relative_volume=cmc_buffer_unpack_int(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_sound_effect_packet *unpack_S2C_play_sound_effect_packet(cmc_buffer *buff) {S2C_play_sound_effect_packet packet;switch(buff->protocol_version) {case 47: {packet.sound_name=cmc_buffer_unpack_string(buff);packet.x=cmc_buffer_unpack_int(buff);packet.y=cmc_buffer_unpack_int(buff);packet.z=cmc_buffer_unpack_int(buff);packet.volume=cmc_buffer_unpack_float(buff);packet.pitch=cmc_buffer_unpack_byte(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_change_game_state_packet *unpack_S2C_play_change_game_state_packet(cmc_buffer *buff) {S2C_play_change_game_state_packet packet;switch(buff->protocol_version) {case 47: {packet.reason=cmc_buffer_unpack_byte(buff);packet.value=cmc_buffer_unpack_float(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_player_abilities_packet *unpack_S2C_play_player_abilities_packet(cmc_buffer *buff) {S2C_play_player_abilities_packet packet;switch(buff->protocol_version) {case 47: {packet.flags=cmc_buffer_unpack_char(buff);packet.flying_speed=cmc_buffer_unpack_float(buff);packet.fov_modifier=cmc_buffer_unpack_float(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_plugin_message_packet *unpack_S2C_play_plugin_message_packet(cmc_buffer *buff) {S2C_play_plugin_message_packet packet;switch(buff->protocol_version) {case 47: {packet.channel=cmc_buffer_unpack_string(buff);packet.data=cmc_buffer_unpack_byte_array(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_disconnect_packet *unpack_S2C_play_disconnect_packet(cmc_buffer *buff) {S2C_play_disconnect_packet packet;switch(buff->protocol_version) {case 47: {packet.reason=cmc_buffer_unpack_string(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_change_difficulty_packet *unpack_S2C_play_change_difficulty_packet(cmc_buffer *buff) {S2C_play_change_difficulty_packet packet;switch(buff->protocol_version) {case 47: {packet.difficulty=cmc_buffer_unpack_byte(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}

C2S_play_keep_alive_packet *unpack_C2S_play_keep_alive_packet(cmc_buffer *buff) {C2S_play_keep_alive_packet packet;switch(buff->protocol_version) {case 47: {packet.keep_alive_id=cmc_buffer_unpack_varint(buff);goto done_unpacking;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL)}
done_unpacking:
               UNPACK_ERR_HANDELER;return packet;}


// CGSE: unpack_methods_c

// CGSS: free_methods_c
void cmc_free_C2S_handshake_handshake_packet(
    C2S_handshake_handshake_packet packet) {
  free_string(packet.server_addr);
}
void cmc_free_S2C_status_response_packet(S2C_status_response_packet packet) {
  free_string(packet.response);
}

void cmc_free_S2C_login_disconnect_packet(S2C_login_disconnect_packet packet) {
  free_string(packet.reason);
}
void cmc_free_S2C_login_encryption_request_packet(
    S2C_login_encryption_request_packet packet) {
  free_string(packet.server_id);
  free_byte_array(packet.public_key);
  free_byte_array(packet.verify_token);
}
void cmc_free_S2C_login_success_packet(S2C_login_success_packet packet) {
  free_string(packet.uuid);
  free_string(packet.name);
}

void cmc_free_C2S_login_start_packet(C2S_login_start_packet packet) {
  free_string(packet.name);
}
void cmc_free_C2S_login_encryption_response_packet(
    C2S_login_encryption_response_packet packet) {
  free_byte_array(packet.shared_secret);
  free_byte_array(packet.verify_token);
}

void cmc_free_S2C_play_join_game_packet(S2C_play_join_game_packet packet) {
  free_string(packet.level_type);
}
void cmc_free_S2C_play_chat_message_packet(
    S2C_play_chat_message_packet packet) {
  free_string(packet.message);
}

void cmc_free_S2C_play_entity_equipment_packet(
    S2C_play_entity_equipment_packet packet) {
  free_slot(packet.item);
}

void cmc_free_S2C_play_respawn_packet(S2C_play_respawn_packet packet) {
  free_string(packet.level_type);
}

void cmc_free_S2C_play_spawn_player_packet(
    S2C_play_spawn_player_packet packet) {
  free_entity_metadata(packet.meta_data);
}

void cmc_free_S2C_play_spawn_mob_packet(S2C_play_spawn_mob_packet packet) {
  free_entity_metadata(packet.meta_data);
}
void cmc_free_S2C_play_spawn_painting_packet(
    S2C_play_spawn_painting_packet packet) {
  free_string(packet.title);
}

void cmc_free_S2C_play_entity_metadata_packet(
    S2C_play_entity_metadata_packet packet) {
  free_entity_metadata(packet.meta_data);
}

void cmc_free_S2C_play_chunk_data_packet(S2C_play_chunk_data_packet packet) {
  free_byte_array(packet.chunk);
}

void cmc_free_S2C_play_sound_effect_packet(
    S2C_play_sound_effect_packet packet) {
  free_string(packet.sound_name);
}

void cmc_free_S2C_play_plugin_message_packet(
    S2C_play_plugin_message_packet packet) {
  free_string(packet.channel);
  free_byte_array(packet.data);
}
void cmc_free_S2C_play_disconnect_packet(S2C_play_disconnect_packet packet) {
  free_string(packet.reason);
}

// CGSE: free_methods_c