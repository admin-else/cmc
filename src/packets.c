#include "packets.h"
#include "err.h"
#include "mcbuffer.h"
#include "mconn.h"
#include "mctypes.h"
#include <jansson.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
If you ask yourself WHY THE FUCK is it that instead of this:
sfprintf(*errmsg, "error while unpacking/packing " #pack_name/unpack_name ":
%s", *errmsg); wellll compiler does this if i dont make it like this: argument 3
[-Wrestrict] 11 |     sprintf(*errmsg, "error while packing " #pack_name ": %s",
*errmsg);             |             ^~~~~~~ ~~~~~~~ src/Packets.c:26:3: note: in
expansion of macro 'PACK_ERR_HANDELER' 26 |   PACK_ERR_HANDELER(keep_alive)
MConn_send_and_free_buffer(conn, buff, errmsg); |   ^~~~~~~~~~~~~~~~~

and i dont like that so made it "safer" ig...

*/

#define UNPACK_ERR_HANDELER                                                    \
  ERR_CHECK;                                                                   \
  if (buff->position != buff->length)                                          \
    ERR(ERR_BUFFER_UNDERUN);

char *packet_data_to_string(int packet_id, MConn_state state,
                            packet_direction direction) {
#define PACKET_DATA_TO_STRING_UTIL(packet_id, state, direction, packet_name)   \
  case (packet_id & 0x00FFFFFF) | (state << 24) | (direction << 23):           \
    return packet_name;

  int combined_packet_data =
      (packet_id & 0x00FFFFFF) | (state << 24) | (direction << 23);

  switch (combined_packet_data) {
    // CGSS: packet_data_to_string
    PACKET_DATA_TO_STRING_UTIL(0x00, CONN_STATE_HANDSHAKE, DIRECTION_C2S,
                               "C2S_handshake_handshake");
    PACKET_DATA_TO_STRING_UTIL(0x00, CONN_STATE_STATUS, DIRECTION_S2C,
                               "S2C_status_response");
    PACKET_DATA_TO_STRING_UTIL(0x01, CONN_STATE_STATUS, DIRECTION_S2C,
                               "S2C_status_pong");
    PACKET_DATA_TO_STRING_UTIL(0x00, CONN_STATE_STATUS, DIRECTION_C2S,
                               "C2S_status_request");
    PACKET_DATA_TO_STRING_UTIL(0x01, CONN_STATE_STATUS, DIRECTION_C2S,
                               "C2S_status_ping");
    PACKET_DATA_TO_STRING_UTIL(0x00, CONN_STATE_LOGIN, DIRECTION_S2C,
                               "S2C_login_disconnect");
    PACKET_DATA_TO_STRING_UTIL(0x01, CONN_STATE_LOGIN, DIRECTION_S2C,
                               "S2C_login_encryption_request");
    PACKET_DATA_TO_STRING_UTIL(0x02, CONN_STATE_LOGIN, DIRECTION_S2C,
                               "S2C_login_success");
    PACKET_DATA_TO_STRING_UTIL(0x03, CONN_STATE_LOGIN, DIRECTION_S2C,
                               "S2C_login_set_compression");
    PACKET_DATA_TO_STRING_UTIL(0x00, CONN_STATE_LOGIN, DIRECTION_C2S,
                               "C2S_login_start");
    PACKET_DATA_TO_STRING_UTIL(0x01, CONN_STATE_LOGIN, DIRECTION_C2S,
                               "C2S_login_encryption_response");
    PACKET_DATA_TO_STRING_UTIL(0x00, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_keep_alive");
    PACKET_DATA_TO_STRING_UTIL(0x01, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_join_game");
    PACKET_DATA_TO_STRING_UTIL(0x02, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_chat_message");
    PACKET_DATA_TO_STRING_UTIL(0x03, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_time_update");
    PACKET_DATA_TO_STRING_UTIL(0x04, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_entity_equipment");
    PACKET_DATA_TO_STRING_UTIL(0x05, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_spawn_position");
    PACKET_DATA_TO_STRING_UTIL(0x06, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_update_health");
    PACKET_DATA_TO_STRING_UTIL(0x07, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_respawn");
    PACKET_DATA_TO_STRING_UTIL(0x08, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_player_look_and_position");
    PACKET_DATA_TO_STRING_UTIL(0x09, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_held_item_change");
    PACKET_DATA_TO_STRING_UTIL(0x0A, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_use_bed");
    PACKET_DATA_TO_STRING_UTIL(0x0B, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_animation");
    PACKET_DATA_TO_STRING_UTIL(0x0C, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_spawn_player");
    PACKET_DATA_TO_STRING_UTIL(0x0D, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_collect_item");
    PACKET_DATA_TO_STRING_UTIL(0x0F, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_spawn_mob");
    PACKET_DATA_TO_STRING_UTIL(0x10, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_spawn_painting");
    PACKET_DATA_TO_STRING_UTIL(0x11, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_spawn_experience_orb");
    PACKET_DATA_TO_STRING_UTIL(0x12, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_entity_velocity");
    PACKET_DATA_TO_STRING_UTIL(0x14, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_entity");
    PACKET_DATA_TO_STRING_UTIL(0x15, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_entity_relative_move");
    PACKET_DATA_TO_STRING_UTIL(0x16, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_entity_look");
    PACKET_DATA_TO_STRING_UTIL(0x17, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_entity_look_and_relative_move");
    PACKET_DATA_TO_STRING_UTIL(0x18, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_entity_teleport");
    PACKET_DATA_TO_STRING_UTIL(0x19, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_entity_head_look");
    PACKET_DATA_TO_STRING_UTIL(0x1A, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_entity_status");
    PACKET_DATA_TO_STRING_UTIL(0x1B, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_attach_entity");
    PACKET_DATA_TO_STRING_UTIL(0x1C, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_entity_metadata");
    PACKET_DATA_TO_STRING_UTIL(0x1D, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_entity_effect");
    PACKET_DATA_TO_STRING_UTIL(0x1E, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_remove_entity_effect");
    PACKET_DATA_TO_STRING_UTIL(0x1F, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_set_experience");
    PACKET_DATA_TO_STRING_UTIL(0x21, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_chunk_data");
    PACKET_DATA_TO_STRING_UTIL(0x23, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_block_change");
    PACKET_DATA_TO_STRING_UTIL(0x24, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_block_action");
    PACKET_DATA_TO_STRING_UTIL(0x25, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_block_break_animation");
    PACKET_DATA_TO_STRING_UTIL(0x28, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_effect");
    PACKET_DATA_TO_STRING_UTIL(0x29, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_sound_effect");
    PACKET_DATA_TO_STRING_UTIL(0x2B, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_change_game_state");
    PACKET_DATA_TO_STRING_UTIL(0x39, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_player_abilities");
    PACKET_DATA_TO_STRING_UTIL(0x3F, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_plugin_message");
    PACKET_DATA_TO_STRING_UTIL(0x40, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_disconnect");
    PACKET_DATA_TO_STRING_UTIL(0x41, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_play_change_difficulty");
    PACKET_DATA_TO_STRING_UTIL(0x00, CONN_STATE_PLAY, DIRECTION_C2S,
                               "C2S_play_keep_alive");
    // CGSE: packet_data_to_string
  default:
    return "PACKET_UNKNOWN";
  }

#undef PACKET_DATA_TO_STRING_UTIL
}

#define ERR_ACTION return;
// CGSS: send_methods_c
void send_packet_C2S_handshake_handshake(MConn *conn,
                                         varint_t protocole_version,
                                         char *server_addr,
                                         unsigned short server_port,
                                         varint_t next_state) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_C2S_handshake_handshake);
  MCbuffer_pack_varint(buff, protocole_version);
  MCbuffer_pack_string(buff, server_addr);
  MCbuffer_pack_ushort(buff, server_port);
  MCbuffer_pack_varint(buff, next_state);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_status_response(MConn *conn, char *response) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_status_response);
  MCbuffer_pack_string(buff, response);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_status_pong(MConn *conn, long payload) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_status_pong);
  MCbuffer_pack_long(buff, payload);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_C2S_status_request(MConn *conn) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_C2S_status_request);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_C2S_status_ping(MConn *conn, long payload) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_C2S_status_ping);
  MCbuffer_pack_long(buff, payload);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_login_disconnect(MConn *conn, char *reason) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_login_disconnect);
  MCbuffer_pack_string(buff, reason);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_login_encryption_request(MConn *conn, char *server_id,
                                              MCbuffer *public_key,
                                              MCbuffer *verify_token) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_login_encryption_request);
  MCbuffer_pack_string(buff, server_id);
  MCbuffer_pack_byte_array(buff, public_key);
  MCbuffer_pack_byte_array(buff, verify_token);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_login_success(MConn *conn, char *uuid, char *name) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_login_success);
  MCbuffer_pack_string(buff, uuid);
  MCbuffer_pack_string(buff, name);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_login_set_compression(MConn *conn, varint_t threshold) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_login_set_compression);
  MCbuffer_pack_varint(buff, threshold);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_C2S_login_start(MConn *conn, char *name) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_C2S_login_start);
  MCbuffer_pack_string(buff, name);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_C2S_login_encryption_response(MConn *conn,
                                               MCbuffer *shared_secret,
                                               MCbuffer *verify_token) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_C2S_login_encryption_response);
  MCbuffer_pack_byte_array(buff, shared_secret);
  MCbuffer_pack_byte_array(buff, verify_token);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_keep_alive(MConn *conn, varint_t keep_alive_id) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_keep_alive);
  MCbuffer_pack_varint(buff, keep_alive_id);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_join_game(MConn *conn, int entity_id,
                                    unsigned char gamemode, char dimension,
                                    unsigned char difficulty,
                                    unsigned char max_players, char *level_type,
                                    bool reduced_debug_info) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_join_game);
  MCbuffer_pack_int(buff, entity_id);
  MCbuffer_pack_byte(buff, gamemode);
  MCbuffer_pack_char(buff, dimension);
  MCbuffer_pack_byte(buff, difficulty);
  MCbuffer_pack_byte(buff, max_players);
  MCbuffer_pack_string(buff, level_type);
  MCbuffer_pack_bool(buff, reduced_debug_info);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_chat_message(MConn *conn, char *message,
                                       char position) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_chat_message);
  MCbuffer_pack_string(buff, message);
  MCbuffer_pack_char(buff, position);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_time_update(MConn *conn, long world_age,
                                      long time_of_day) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_time_update);
  MCbuffer_pack_long(buff, world_age);
  MCbuffer_pack_long(buff, time_of_day);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_equipment(MConn *conn, varint_t entity_id,
                                           short slot, slot_t *item) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_entity_equipment);
  MCbuffer_pack_varint(buff, entity_id);
  MCbuffer_pack_short(buff, slot);
  MCbuffer_pack_slot(buff, item);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_spawn_position(MConn *conn, block_pos_t location) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_spawn_position);
  MCbuffer_pack_position(buff, location);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_update_health(MConn *conn, float health,
                                        varint_t food, float food_saturation) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_update_health);
  MCbuffer_pack_float(buff, health);
  MCbuffer_pack_varint(buff, food);
  MCbuffer_pack_float(buff, food_saturation);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_respawn(MConn *conn, int dimesion,
                                  unsigned char difficulty,
                                  unsigned char gamemode, char *level_type) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_respawn);
  MCbuffer_pack_int(buff, dimesion);
  MCbuffer_pack_byte(buff, difficulty);
  MCbuffer_pack_byte(buff, gamemode);
  MCbuffer_pack_string(buff, level_type);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_player_look_and_position(MConn *conn, double x,
                                                   double y, double z,
                                                   float yaw, float pitch,
                                                   unsigned char flags) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_player_look_and_position);
  MCbuffer_pack_double(buff, x);
  MCbuffer_pack_double(buff, y);
  MCbuffer_pack_double(buff, z);
  MCbuffer_pack_float(buff, yaw);
  MCbuffer_pack_float(buff, pitch);
  MCbuffer_pack_byte(buff, flags);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_held_item_change(MConn *conn, char slot) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_held_item_change);
  MCbuffer_pack_char(buff, slot);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_use_bed(MConn *conn, varint_t entity_id,
                                  block_pos_t location) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_use_bed);
  MCbuffer_pack_varint(buff, entity_id);
  MCbuffer_pack_position(buff, location);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_animation(MConn *conn, varint_t entity_id,
                                    unsigned char animation) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_animation);
  MCbuffer_pack_varint(buff, entity_id);
  MCbuffer_pack_byte(buff, animation);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_spawn_player(MConn *conn, varint_t entity_id,
                                       unsigned long long uuid, int x, int y,
                                       int z, unsigned char yaw,
                                       unsigned char pitch, short current_item,
                                       entity_metadata_t meta_data) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_spawn_player);
  MCbuffer_pack_varint(buff, entity_id);
  MCbuffer_pack_ullong(buff, uuid);
  MCbuffer_pack_int(buff, x);
  MCbuffer_pack_int(buff, y);
  MCbuffer_pack_int(buff, z);
  MCbuffer_pack_byte(buff, yaw);
  MCbuffer_pack_byte(buff, pitch);
  MCbuffer_pack_short(buff, current_item);
  MCbuffer_pack_entity_metadata(buff, meta_data);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_collect_item(MConn *conn,
                                       varint_t collected_entity_id,
                                       varint_t collector_entity_id) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_collect_item);
  MCbuffer_pack_varint(buff, collected_entity_id);
  MCbuffer_pack_varint(buff, collector_entity_id);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_spawn_mob(MConn *conn, varint_t entity_id,
                                    unsigned char type, int x, int y, int z,
                                    unsigned char yaw, unsigned char pitch,
                                    unsigned char head_pitch, short x_vel,
                                    short y_vel, short z_vel,
                                    entity_metadata_t meta_data) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_spawn_mob);
  MCbuffer_pack_varint(buff, entity_id);
  MCbuffer_pack_byte(buff, type);
  MCbuffer_pack_int(buff, x);
  MCbuffer_pack_int(buff, y);
  MCbuffer_pack_int(buff, z);
  MCbuffer_pack_byte(buff, yaw);
  MCbuffer_pack_byte(buff, pitch);
  MCbuffer_pack_byte(buff, head_pitch);
  MCbuffer_pack_short(buff, x_vel);
  MCbuffer_pack_short(buff, y_vel);
  MCbuffer_pack_short(buff, z_vel);
  MCbuffer_pack_entity_metadata(buff, meta_data);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_spawn_painting(MConn *conn, varint_t entity_id,
                                         char *title, block_pos_t location,
                                         unsigned char direction) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_spawn_painting);
  MCbuffer_pack_varint(buff, entity_id);
  MCbuffer_pack_string(buff, title);
  MCbuffer_pack_position(buff, location);
  MCbuffer_pack_byte(buff, direction);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_spawn_experience_orb(MConn *conn, varint_t entity_id,
                                               int x, int y, int z,
                                               short count) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_spawn_experience_orb);
  MCbuffer_pack_varint(buff, entity_id);
  MCbuffer_pack_int(buff, x);
  MCbuffer_pack_int(buff, y);
  MCbuffer_pack_int(buff, z);
  MCbuffer_pack_short(buff, count);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_velocity(MConn *conn, varint_t entity_id,
                                          short x_vel, short y_vel,
                                          short z_vel) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_entity_velocity);
  MCbuffer_pack_varint(buff, entity_id);
  MCbuffer_pack_short(buff, x_vel);
  MCbuffer_pack_short(buff, y_vel);
  MCbuffer_pack_short(buff, z_vel);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity(MConn *conn, varint_t entity_id) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_entity);
  MCbuffer_pack_varint(buff, entity_id);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_relative_move(MConn *conn, varint_t entity_id,
                                               char delta_x, char delta_y,
                                               char delta_z, bool on_ground) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_entity_relative_move);
  MCbuffer_pack_varint(buff, entity_id);
  MCbuffer_pack_char(buff, delta_x);
  MCbuffer_pack_char(buff, delta_y);
  MCbuffer_pack_char(buff, delta_z);
  MCbuffer_pack_bool(buff, on_ground);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_look(MConn *conn, varint_t entity_id,
                                      unsigned char yaw, unsigned char pitch,
                                      bool on_ground) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_entity_look);
  MCbuffer_pack_varint(buff, entity_id);
  MCbuffer_pack_byte(buff, yaw);
  MCbuffer_pack_byte(buff, pitch);
  MCbuffer_pack_bool(buff, on_ground);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_look_and_relative_move(
    MConn *conn, varint_t entity_id, char delta_x, char delta_y, char delta_z,
    unsigned char yaw, unsigned char pitch, bool on_ground) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_entity_look_and_relative_move);
  MCbuffer_pack_varint(buff, entity_id);
  MCbuffer_pack_char(buff, delta_x);
  MCbuffer_pack_char(buff, delta_y);
  MCbuffer_pack_char(buff, delta_z);
  MCbuffer_pack_byte(buff, yaw);
  MCbuffer_pack_byte(buff, pitch);
  MCbuffer_pack_bool(buff, on_ground);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_teleport(MConn *conn, varint_t entity_id,
                                          int x, int y, int z,
                                          unsigned char yaw,
                                          unsigned char pitch, bool on_ground) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_entity_teleport);
  MCbuffer_pack_varint(buff, entity_id);
  MCbuffer_pack_int(buff, x);
  MCbuffer_pack_int(buff, y);
  MCbuffer_pack_int(buff, z);
  MCbuffer_pack_byte(buff, yaw);
  MCbuffer_pack_byte(buff, pitch);
  MCbuffer_pack_bool(buff, on_ground);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_head_look(MConn *conn, varint_t entity_id,
                                           unsigned char head_yaw) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_entity_head_look);
  MCbuffer_pack_varint(buff, entity_id);
  MCbuffer_pack_byte(buff, head_yaw);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_status(MConn *conn, int entity_id,
                                        char entity_status) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_entity_status);
  MCbuffer_pack_int(buff, entity_id);
  MCbuffer_pack_char(buff, entity_status);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_attach_entity(MConn *conn, int entity_id,
                                        int vehicle_id, bool leash) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_attach_entity);
  MCbuffer_pack_int(buff, entity_id);
  MCbuffer_pack_int(buff, vehicle_id);
  MCbuffer_pack_bool(buff, leash);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_metadata(MConn *conn, varint_t entity_id,
                                          entity_metadata_t meta_data) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_entity_metadata);
  MCbuffer_pack_varint(buff, entity_id);
  MCbuffer_pack_entity_metadata(buff, meta_data);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_effect(MConn *conn, varint_t entity_id,
                                        char effect_id, char amplifier,
                                        varint_t duration,
                                        bool hide_particles) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_entity_effect);
  MCbuffer_pack_varint(buff, entity_id);
  MCbuffer_pack_char(buff, effect_id);
  MCbuffer_pack_char(buff, amplifier);
  MCbuffer_pack_varint(buff, duration);
  MCbuffer_pack_bool(buff, hide_particles);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_remove_entity_effect(MConn *conn, varint_t entity_id,
                                               char effect_id) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_remove_entity_effect);
  MCbuffer_pack_varint(buff, entity_id);
  MCbuffer_pack_char(buff, effect_id);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_set_experience(MConn *conn, float experience_bar,
                                         varint_t level,
                                         varint_t total_experience) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_set_experience);
  MCbuffer_pack_float(buff, experience_bar);
  MCbuffer_pack_varint(buff, level);
  MCbuffer_pack_varint(buff, total_experience);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_chunk_data(MConn *conn, int chunk_x, int chunk_z,
                                     bool ground_up_continuous,
                                     unsigned short primary_bitmask,
                                     MCbuffer *chunk) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_chunk_data);
  MCbuffer_pack_int(buff, chunk_x);
  MCbuffer_pack_int(buff, chunk_z);
  MCbuffer_pack_bool(buff, ground_up_continuous);
  MCbuffer_pack_ushort(buff, primary_bitmask);
  MCbuffer_pack_byte_array(buff, chunk);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_block_change(MConn *conn, block_pos_t location,
                                       varint_t block_id) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_block_change);
  MCbuffer_pack_position(buff, location);
  MCbuffer_pack_varint(buff, block_id);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_block_action(MConn *conn, block_pos_t location,
                                       unsigned char block_data_1,
                                       unsigned char block_data_2,
                                       varint_t block_type) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_block_action);
  MCbuffer_pack_position(buff, location);
  MCbuffer_pack_byte(buff, block_data_1);
  MCbuffer_pack_byte(buff, block_data_2);
  MCbuffer_pack_varint(buff, block_type);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_block_break_animation(MConn *conn, varint_t entity_id,
                                                block_pos_t location,
                                                char destroy_stage) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_block_break_animation);
  MCbuffer_pack_varint(buff, entity_id);
  MCbuffer_pack_position(buff, location);
  MCbuffer_pack_char(buff, destroy_stage);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_effect(MConn *conn, int effect_id,
                                 block_pos_t location, int data, bool d,
                                 int particle_id, bool long_distances, float x,
                                 float y, float z, float x_offset,
                                 float y_offset, float z_offset,
                                 float particle_data, int particle_count,
                                 int sable_relative_volume) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_effect);
  MCbuffer_pack_int(buff, effect_id);
  MCbuffer_pack_position(buff, location);
  MCbuffer_pack_int(buff, data);
  MCbuffer_pack_bool(buff, d);
  MCbuffer_pack_int(buff, particle_id);
  MCbuffer_pack_bool(buff, long_distances);
  MCbuffer_pack_float(buff, x);
  MCbuffer_pack_float(buff, y);
  MCbuffer_pack_float(buff, z);
  MCbuffer_pack_float(buff, x_offset);
  MCbuffer_pack_float(buff, y_offset);
  MCbuffer_pack_float(buff, z_offset);
  MCbuffer_pack_float(buff, particle_data);
  MCbuffer_pack_int(buff, particle_count);
  MCbuffer_pack_int(buff, sable_relative_volume);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_sound_effect(MConn *conn, char *sound_name, int x,
                                       int y, int z, float volume,
                                       unsigned char pitch) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_sound_effect);
  MCbuffer_pack_string(buff, sound_name);
  MCbuffer_pack_int(buff, x);
  MCbuffer_pack_int(buff, y);
  MCbuffer_pack_int(buff, z);
  MCbuffer_pack_float(buff, volume);
  MCbuffer_pack_byte(buff, pitch);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_change_game_state(MConn *conn, unsigned char reason,
                                            float value) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_change_game_state);
  MCbuffer_pack_byte(buff, reason);
  MCbuffer_pack_float(buff, value);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_player_abilities(MConn *conn, char flags,
                                           float flying_speed,
                                           float fov_modifier) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_player_abilities);
  MCbuffer_pack_char(buff, flags);
  MCbuffer_pack_float(buff, flying_speed);
  MCbuffer_pack_float(buff, fov_modifier);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_plugin_message(MConn *conn, char *channel,
                                         MCbuffer *data) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_plugin_message);
  MCbuffer_pack_string(buff, channel);
  MCbuffer_pack_byte_array(buff, data);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_disconnect(MConn *conn, char *reason) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_disconnect);
  MCbuffer_pack_string(buff, reason);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_S2C_play_change_difficulty(MConn *conn,
                                            unsigned char difficulty) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_S2C_play_change_difficulty);
  MCbuffer_pack_byte(buff, difficulty);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

void send_packet_C2S_play_keep_alive(MConn *conn, varint_t keep_alive_id) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, packetid_C2S_play_keep_alive);
  MCbuffer_pack_varint(buff, keep_alive_id);
  ERR_CHECK;
  MConn_send_packet(conn, buff);
}

// CGSE: send_methods_c

#define ERR_ACTION return packet;
// CGSS: unpack_methods_c
C2S_handshake_handshake_packet_t
unpack_C2S_handshake_handshake_packet(MCbuffer *buff) {
  C2S_handshake_handshake_packet_t packet;
  packet.protocole_version = MCbuffer_unpack_varint(buff);
  packet.server_addr = MCbuffer_unpack_string(buff);
  packet.server_port = MCbuffer_unpack_ushort(buff);
  packet.next_state = MCbuffer_unpack_varint(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_status_response_packet_t unpack_S2C_status_response_packet(MCbuffer *buff) {
  S2C_status_response_packet_t packet;
  packet.response = MCbuffer_unpack_string(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_status_pong_packet_t unpack_S2C_status_pong_packet(MCbuffer *buff) {
  S2C_status_pong_packet_t packet;
  packet.payload = MCbuffer_unpack_long(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

C2S_status_ping_packet_t unpack_C2S_status_ping_packet(MCbuffer *buff) {
  C2S_status_ping_packet_t packet;
  packet.payload = MCbuffer_unpack_long(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_login_disconnect_packet_t
unpack_S2C_login_disconnect_packet(MCbuffer *buff) {
  S2C_login_disconnect_packet_t packet;
  packet.reason = MCbuffer_unpack_string(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_login_encryption_request_packet_t
unpack_S2C_login_encryption_request_packet(MCbuffer *buff) {
  S2C_login_encryption_request_packet_t packet;
  packet.server_id = MCbuffer_unpack_string(buff);
  packet.public_key = MCbuffer_unpack_byte_array(buff);
  packet.verify_token = MCbuffer_unpack_byte_array(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_login_success_packet_t unpack_S2C_login_success_packet(MCbuffer *buff) {
  S2C_login_success_packet_t packet;
  packet.uuid = MCbuffer_unpack_string(buff);
  packet.name = MCbuffer_unpack_string(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_login_set_compression_packet_t
unpack_S2C_login_set_compression_packet(MCbuffer *buff) {
  S2C_login_set_compression_packet_t packet;
  packet.threshold = MCbuffer_unpack_varint(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

C2S_login_start_packet_t unpack_C2S_login_start_packet(MCbuffer *buff) {
  C2S_login_start_packet_t packet;
  packet.name = MCbuffer_unpack_string(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

C2S_login_encryption_response_packet_t
unpack_C2S_login_encryption_response_packet(MCbuffer *buff) {
  C2S_login_encryption_response_packet_t packet;
  packet.shared_secret = MCbuffer_unpack_byte_array(buff);
  packet.verify_token = MCbuffer_unpack_byte_array(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_keep_alive_packet_t unpack_S2C_play_keep_alive_packet(MCbuffer *buff) {
  S2C_play_keep_alive_packet_t packet;
  packet.keep_alive_id = MCbuffer_unpack_varint(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_join_game_packet_t unpack_S2C_play_join_game_packet(MCbuffer *buff) {
  S2C_play_join_game_packet_t packet;
  packet.entity_id = MCbuffer_unpack_int(buff);
  packet.gamemode = MCbuffer_unpack_byte(buff);
  packet.dimension = MCbuffer_unpack_char(buff);
  packet.difficulty = MCbuffer_unpack_byte(buff);
  packet.max_players = MCbuffer_unpack_byte(buff);
  packet.level_type = MCbuffer_unpack_string(buff);
  packet.reduced_debug_info = MCbuffer_unpack_bool(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_chat_message_packet_t
unpack_S2C_play_chat_message_packet(MCbuffer *buff) {
  S2C_play_chat_message_packet_t packet;
  packet.message = MCbuffer_unpack_string(buff);
  packet.position = MCbuffer_unpack_char(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_time_update_packet_t
unpack_S2C_play_time_update_packet(MCbuffer *buff) {
  S2C_play_time_update_packet_t packet;
  packet.world_age = MCbuffer_unpack_long(buff);
  packet.time_of_day = MCbuffer_unpack_long(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_equipment_packet_t
unpack_S2C_play_entity_equipment_packet(MCbuffer *buff) {
  S2C_play_entity_equipment_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff);
  packet.slot = MCbuffer_unpack_short(buff);
  packet.item = MCbuffer_unpack_slot(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_spawn_position_packet_t
unpack_S2C_play_spawn_position_packet(MCbuffer *buff) {
  S2C_play_spawn_position_packet_t packet;
  packet.location = MCbuffer_unpack_position(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_update_health_packet_t
unpack_S2C_play_update_health_packet(MCbuffer *buff) {
  S2C_play_update_health_packet_t packet;
  packet.health = MCbuffer_unpack_float(buff);
  packet.food = MCbuffer_unpack_varint(buff);
  packet.food_saturation = MCbuffer_unpack_float(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_respawn_packet_t unpack_S2C_play_respawn_packet(MCbuffer *buff) {
  S2C_play_respawn_packet_t packet;
  packet.dimesion = MCbuffer_unpack_int(buff);
  packet.difficulty = MCbuffer_unpack_byte(buff);
  packet.gamemode = MCbuffer_unpack_byte(buff);
  packet.level_type = MCbuffer_unpack_string(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_player_look_and_position_packet_t
unpack_S2C_play_player_look_and_position_packet(MCbuffer *buff) {
  S2C_play_player_look_and_position_packet_t packet;
  packet.x = MCbuffer_unpack_double(buff);
  packet.y = MCbuffer_unpack_double(buff);
  packet.z = MCbuffer_unpack_double(buff);
  packet.yaw = MCbuffer_unpack_float(buff);
  packet.pitch = MCbuffer_unpack_float(buff);
  packet.flags = MCbuffer_unpack_byte(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_held_item_change_packet_t
unpack_S2C_play_held_item_change_packet(MCbuffer *buff) {
  S2C_play_held_item_change_packet_t packet;
  packet.slot = MCbuffer_unpack_char(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_use_bed_packet_t unpack_S2C_play_use_bed_packet(MCbuffer *buff) {
  S2C_play_use_bed_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff);
  packet.location = MCbuffer_unpack_position(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_animation_packet_t unpack_S2C_play_animation_packet(MCbuffer *buff) {
  S2C_play_animation_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff);
  packet.animation = MCbuffer_unpack_byte(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_spawn_player_packet_t
unpack_S2C_play_spawn_player_packet(MCbuffer *buff) {
  S2C_play_spawn_player_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff);
  packet.uuid = MCbuffer_unpack_ullong(buff);
  packet.x = MCbuffer_unpack_int(buff);
  packet.y = MCbuffer_unpack_int(buff);
  packet.z = MCbuffer_unpack_int(buff);
  packet.yaw = MCbuffer_unpack_byte(buff);
  packet.pitch = MCbuffer_unpack_byte(buff);
  packet.current_item = MCbuffer_unpack_short(buff);
  packet.meta_data = MCbuffer_unpack_entity_metadata(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_collect_item_packet_t
unpack_S2C_play_collect_item_packet(MCbuffer *buff) {
  S2C_play_collect_item_packet_t packet;
  packet.collected_entity_id = MCbuffer_unpack_varint(buff);
  packet.collector_entity_id = MCbuffer_unpack_varint(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_spawn_mob_packet_t unpack_S2C_play_spawn_mob_packet(MCbuffer *buff) {
  S2C_play_spawn_mob_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff);
  packet.type = MCbuffer_unpack_byte(buff);
  packet.x = MCbuffer_unpack_int(buff);
  packet.y = MCbuffer_unpack_int(buff);
  packet.z = MCbuffer_unpack_int(buff);
  packet.yaw = MCbuffer_unpack_byte(buff);
  packet.pitch = MCbuffer_unpack_byte(buff);
  packet.head_pitch = MCbuffer_unpack_byte(buff);
  packet.x_vel = MCbuffer_unpack_short(buff);
  packet.y_vel = MCbuffer_unpack_short(buff);
  packet.z_vel = MCbuffer_unpack_short(buff);
  packet.meta_data = MCbuffer_unpack_entity_metadata(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_spawn_painting_packet_t
unpack_S2C_play_spawn_painting_packet(MCbuffer *buff) {
  S2C_play_spawn_painting_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff);
  packet.title = MCbuffer_unpack_string(buff);
  packet.location = MCbuffer_unpack_position(buff);
  packet.direction = MCbuffer_unpack_byte(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_spawn_experience_orb_packet_t
unpack_S2C_play_spawn_experience_orb_packet(MCbuffer *buff) {
  S2C_play_spawn_experience_orb_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff);
  packet.x = MCbuffer_unpack_int(buff);
  packet.y = MCbuffer_unpack_int(buff);
  packet.z = MCbuffer_unpack_int(buff);
  packet.count = MCbuffer_unpack_short(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_velocity_packet_t
unpack_S2C_play_entity_velocity_packet(MCbuffer *buff) {
  S2C_play_entity_velocity_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff);
  packet.x_vel = MCbuffer_unpack_short(buff);
  packet.y_vel = MCbuffer_unpack_short(buff);
  packet.z_vel = MCbuffer_unpack_short(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_packet_t unpack_S2C_play_entity_packet(MCbuffer *buff) {
  S2C_play_entity_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_relative_move_packet_t
unpack_S2C_play_entity_relative_move_packet(MCbuffer *buff) {
  S2C_play_entity_relative_move_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff);
  packet.delta_x = MCbuffer_unpack_char(buff);
  packet.delta_y = MCbuffer_unpack_char(buff);
  packet.delta_z = MCbuffer_unpack_char(buff);
  packet.on_ground = MCbuffer_unpack_bool(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_look_packet_t
unpack_S2C_play_entity_look_packet(MCbuffer *buff) {
  S2C_play_entity_look_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff);
  packet.yaw = MCbuffer_unpack_byte(buff);
  packet.pitch = MCbuffer_unpack_byte(buff);
  packet.on_ground = MCbuffer_unpack_bool(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_look_and_relative_move_packet_t
unpack_S2C_play_entity_look_and_relative_move_packet(MCbuffer *buff) {
  S2C_play_entity_look_and_relative_move_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff);
  packet.delta_x = MCbuffer_unpack_char(buff);
  packet.delta_y = MCbuffer_unpack_char(buff);
  packet.delta_z = MCbuffer_unpack_char(buff);
  packet.yaw = MCbuffer_unpack_byte(buff);
  packet.pitch = MCbuffer_unpack_byte(buff);
  packet.on_ground = MCbuffer_unpack_bool(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_teleport_packet_t
unpack_S2C_play_entity_teleport_packet(MCbuffer *buff) {
  S2C_play_entity_teleport_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff);
  packet.x = MCbuffer_unpack_int(buff);
  packet.y = MCbuffer_unpack_int(buff);
  packet.z = MCbuffer_unpack_int(buff);
  packet.yaw = MCbuffer_unpack_byte(buff);
  packet.pitch = MCbuffer_unpack_byte(buff);
  packet.on_ground = MCbuffer_unpack_bool(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_head_look_packet_t
unpack_S2C_play_entity_head_look_packet(MCbuffer *buff) {
  S2C_play_entity_head_look_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff);
  packet.head_yaw = MCbuffer_unpack_byte(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_status_packet_t
unpack_S2C_play_entity_status_packet(MCbuffer *buff) {
  S2C_play_entity_status_packet_t packet;
  packet.entity_id = MCbuffer_unpack_int(buff);
  packet.entity_status = MCbuffer_unpack_char(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_attach_entity_packet_t
unpack_S2C_play_attach_entity_packet(MCbuffer *buff) {
  S2C_play_attach_entity_packet_t packet;
  packet.entity_id = MCbuffer_unpack_int(buff);
  packet.vehicle_id = MCbuffer_unpack_int(buff);
  packet.leash = MCbuffer_unpack_bool(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_metadata_packet_t
unpack_S2C_play_entity_metadata_packet(MCbuffer *buff) {
  S2C_play_entity_metadata_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff);
  packet.meta_data = MCbuffer_unpack_entity_metadata(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_effect_packet_t
unpack_S2C_play_entity_effect_packet(MCbuffer *buff) {
  S2C_play_entity_effect_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff);
  packet.effect_id = MCbuffer_unpack_char(buff);
  packet.amplifier = MCbuffer_unpack_char(buff);
  packet.duration = MCbuffer_unpack_varint(buff);
  packet.hide_particles = MCbuffer_unpack_bool(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_remove_entity_effect_packet_t
unpack_S2C_play_remove_entity_effect_packet(MCbuffer *buff) {
  S2C_play_remove_entity_effect_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff);
  packet.effect_id = MCbuffer_unpack_char(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_set_experience_packet_t
unpack_S2C_play_set_experience_packet(MCbuffer *buff) {
  S2C_play_set_experience_packet_t packet;
  packet.experience_bar = MCbuffer_unpack_float(buff);
  packet.level = MCbuffer_unpack_varint(buff);
  packet.total_experience = MCbuffer_unpack_varint(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_chunk_data_packet_t unpack_S2C_play_chunk_data_packet(MCbuffer *buff) {
  S2C_play_chunk_data_packet_t packet;
  packet.chunk_x = MCbuffer_unpack_int(buff);
  packet.chunk_z = MCbuffer_unpack_int(buff);
  packet.ground_up_continuous = MCbuffer_unpack_bool(buff);
  packet.primary_bitmask = MCbuffer_unpack_ushort(buff);
  packet.chunk = MCbuffer_unpack_byte_array(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_block_change_packet_t
unpack_S2C_play_block_change_packet(MCbuffer *buff) {
  S2C_play_block_change_packet_t packet;
  packet.location = MCbuffer_unpack_position(buff);
  packet.block_id = MCbuffer_unpack_varint(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_block_action_packet_t
unpack_S2C_play_block_action_packet(MCbuffer *buff) {
  S2C_play_block_action_packet_t packet;
  packet.location = MCbuffer_unpack_position(buff);
  packet.block_data_1 = MCbuffer_unpack_byte(buff);
  packet.block_data_2 = MCbuffer_unpack_byte(buff);
  packet.block_type = MCbuffer_unpack_varint(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_block_break_animation_packet_t
unpack_S2C_play_block_break_animation_packet(MCbuffer *buff) {
  S2C_play_block_break_animation_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff);
  packet.location = MCbuffer_unpack_position(buff);
  packet.destroy_stage = MCbuffer_unpack_char(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_effect_packet_t unpack_S2C_play_effect_packet(MCbuffer *buff) {
  S2C_play_effect_packet_t packet;
  packet.effect_id = MCbuffer_unpack_int(buff);
  packet.location = MCbuffer_unpack_position(buff);
  packet.data = MCbuffer_unpack_int(buff);
  packet.d = MCbuffer_unpack_bool(buff);
  packet.particle_id = MCbuffer_unpack_int(buff);
  packet.long_distances = MCbuffer_unpack_bool(buff);
  packet.x = MCbuffer_unpack_float(buff);
  packet.y = MCbuffer_unpack_float(buff);
  packet.z = MCbuffer_unpack_float(buff);
  packet.x_offset = MCbuffer_unpack_float(buff);
  packet.y_offset = MCbuffer_unpack_float(buff);
  packet.z_offset = MCbuffer_unpack_float(buff);
  packet.particle_data = MCbuffer_unpack_float(buff);
  packet.particle_count = MCbuffer_unpack_int(buff);
  packet.sable_relative_volume = MCbuffer_unpack_int(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_sound_effect_packet_t
unpack_S2C_play_sound_effect_packet(MCbuffer *buff) {
  S2C_play_sound_effect_packet_t packet;
  packet.sound_name = MCbuffer_unpack_string(buff);
  packet.x = MCbuffer_unpack_int(buff);
  packet.y = MCbuffer_unpack_int(buff);
  packet.z = MCbuffer_unpack_int(buff);
  packet.volume = MCbuffer_unpack_float(buff);
  packet.pitch = MCbuffer_unpack_byte(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_change_game_state_packet_t
unpack_S2C_play_change_game_state_packet(MCbuffer *buff) {
  S2C_play_change_game_state_packet_t packet;
  packet.reason = MCbuffer_unpack_byte(buff);
  packet.value = MCbuffer_unpack_float(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_player_abilities_packet_t
unpack_S2C_play_player_abilities_packet(MCbuffer *buff) {
  S2C_play_player_abilities_packet_t packet;
  packet.flags = MCbuffer_unpack_char(buff);
  packet.flying_speed = MCbuffer_unpack_float(buff);
  packet.fov_modifier = MCbuffer_unpack_float(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_plugin_message_packet_t
unpack_S2C_play_plugin_message_packet(MCbuffer *buff) {
  S2C_play_plugin_message_packet_t packet;
  packet.channel = MCbuffer_unpack_string(buff);
  packet.data = MCbuffer_unpack_byte_array(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_disconnect_packet_t unpack_S2C_play_disconnect_packet(MCbuffer *buff) {
  S2C_play_disconnect_packet_t packet;
  packet.reason = MCbuffer_unpack_string(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_change_difficulty_packet_t
unpack_S2C_play_change_difficulty_packet(MCbuffer *buff) {
  S2C_play_change_difficulty_packet_t packet;
  packet.difficulty = MCbuffer_unpack_byte(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

C2S_play_keep_alive_packet_t unpack_C2S_play_keep_alive_packet(MCbuffer *buff) {
  C2S_play_keep_alive_packet_t packet;
  packet.keep_alive_id = MCbuffer_unpack_varint(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

// CGSE: unpack_methods_c