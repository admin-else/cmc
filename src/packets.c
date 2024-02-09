#include "packets.h"
#include "buffer.h"
#include "conn.h"
#include "err.h"
#include "mctypes.h"
#include "packet_types.h"
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
cmc_conn_send_and_free_buffer(conn, buff, errmsg); |   ^~~~~~~~~~~~~~~~~

and i dont like that so made it "safer" ig...

*/

#define UNPACK_ERR_HANDELER                                                    \
  ERR_CHECK;                                                                   \
  if (buff->position != buff->length)                                          \
    ERR(ERR_BUFFER_UNDERUN);

char *packet_id_to_string(int packet_id, cmc_conn_state state,
                          packet_direction direction) {
#define PACKET_ID_TO_STRING_UTIL(packet_id, state, direction, packet_name)     \
  case (packet_id & 0x00FFFFFF) | (state << 24) | (direction << 23):           \
    return packet_name;

  int combined_packet_data =
      (packet_id & 0x00FFFFFF) | (state << 24) | (direction << 23);

  switch (combined_packet_data) {
    // CGSS: packet_id_to_string
    PACKET_ID_TO_STRING_UTIL(0x00, CONN_STATE_HANDSHAKE, DIRECTION_C2S,
                             "C2S_handshake_handshake");
    PACKET_ID_TO_STRING_UTIL(0x00, CONN_STATE_STATUS, DIRECTION_S2C,
                             "S2C_status_response");
    PACKET_ID_TO_STRING_UTIL(0x01, CONN_STATE_STATUS, DIRECTION_S2C,
                             "S2C_status_pong");
    PACKET_ID_TO_STRING_UTIL(0x00, CONN_STATE_STATUS, DIRECTION_C2S,
                             "C2S_status_request");
    PACKET_ID_TO_STRING_UTIL(0x01, CONN_STATE_STATUS, DIRECTION_C2S,
                             "C2S_status_ping");
    PACKET_ID_TO_STRING_UTIL(0x00, CONN_STATE_LOGIN, DIRECTION_S2C,
                             "S2C_login_disconnect");
    PACKET_ID_TO_STRING_UTIL(0x01, CONN_STATE_LOGIN, DIRECTION_S2C,
                             "S2C_login_encryption_request");
    PACKET_ID_TO_STRING_UTIL(0x02, CONN_STATE_LOGIN, DIRECTION_S2C,
                             "S2C_login_success");
    PACKET_ID_TO_STRING_UTIL(0x03, CONN_STATE_LOGIN, DIRECTION_S2C,
                             "S2C_login_set_compression");
    PACKET_ID_TO_STRING_UTIL(0x00, CONN_STATE_LOGIN, DIRECTION_C2S,
                             "C2S_login_start");
    PACKET_ID_TO_STRING_UTIL(0x01, CONN_STATE_LOGIN, DIRECTION_C2S,
                             "C2S_login_encryption_response");
    PACKET_ID_TO_STRING_UTIL(0x00, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_keep_alive");
    PACKET_ID_TO_STRING_UTIL(0x01, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_join_game");
    PACKET_ID_TO_STRING_UTIL(0x02, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_chat_message");
    PACKET_ID_TO_STRING_UTIL(0x03, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_time_update");
    PACKET_ID_TO_STRING_UTIL(0x04, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_entity_equipment");
    PACKET_ID_TO_STRING_UTIL(0x05, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_spawn_position");
    PACKET_ID_TO_STRING_UTIL(0x06, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_update_health");
    PACKET_ID_TO_STRING_UTIL(0x07, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_respawn");
    PACKET_ID_TO_STRING_UTIL(0x08, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_player_look_and_position");
    PACKET_ID_TO_STRING_UTIL(0x09, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_held_item_change");
    PACKET_ID_TO_STRING_UTIL(0x0A, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_use_bed");
    PACKET_ID_TO_STRING_UTIL(0x0B, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_animation");
    PACKET_ID_TO_STRING_UTIL(0x0C, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_spawn_player");
    PACKET_ID_TO_STRING_UTIL(0x0D, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_collect_item");
    PACKET_ID_TO_STRING_UTIL(0x0F, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_spawn_mob");
    PACKET_ID_TO_STRING_UTIL(0x10, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_spawn_painting");
    PACKET_ID_TO_STRING_UTIL(0x11, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_spawn_experience_orb");
    PACKET_ID_TO_STRING_UTIL(0x12, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_entity_velocity");
    PACKET_ID_TO_STRING_UTIL(0x14, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_entity");
    PACKET_ID_TO_STRING_UTIL(0x15, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_entity_relative_move");
    PACKET_ID_TO_STRING_UTIL(0x16, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_entity_look");
    PACKET_ID_TO_STRING_UTIL(0x17, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_entity_look_and_relative_move");
    PACKET_ID_TO_STRING_UTIL(0x18, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_entity_teleport");
    PACKET_ID_TO_STRING_UTIL(0x19, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_entity_head_look");
    PACKET_ID_TO_STRING_UTIL(0x1A, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_entity_status");
    PACKET_ID_TO_STRING_UTIL(0x1B, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_attach_entity");
    PACKET_ID_TO_STRING_UTIL(0x1C, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_entity_metadata");
    PACKET_ID_TO_STRING_UTIL(0x1D, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_entity_effect");
    PACKET_ID_TO_STRING_UTIL(0x1E, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_remove_entity_effect");
    PACKET_ID_TO_STRING_UTIL(0x1F, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_set_experience");
    PACKET_ID_TO_STRING_UTIL(0x21, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_chunk_data");
    PACKET_ID_TO_STRING_UTIL(0x23, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_block_change");
    PACKET_ID_TO_STRING_UTIL(0x24, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_block_action");
    PACKET_ID_TO_STRING_UTIL(0x25, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_block_break_animation");
    PACKET_ID_TO_STRING_UTIL(0x28, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_effect");
    PACKET_ID_TO_STRING_UTIL(0x29, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_sound_effect");
    PACKET_ID_TO_STRING_UTIL(0x2B, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_change_game_state");
    PACKET_ID_TO_STRING_UTIL(0x39, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_player_abilities");
    PACKET_ID_TO_STRING_UTIL(0x3F, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_plugin_message");
    PACKET_ID_TO_STRING_UTIL(0x40, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_disconnect");
    PACKET_ID_TO_STRING_UTIL(0x41, CONN_STATE_PLAY, DIRECTION_S2C,
                             "S2C_play_change_difficulty");
    PACKET_ID_TO_STRING_UTIL(0x00, CONN_STATE_PLAY, DIRECTION_C2S,
                             "C2S_play_keep_alive");
    // CGSE: packet_id_to_string
  default:
    return "PACKET_UNKNOWN";
  }

#undef PACKET_ID_TO_STRING_UTIL
}

#define ERR_ACTION return;
// CGSS: send_methods_c
void send_packet_C2S_handshake_handshake(struct cmc_conn *conn,
                                         varint_t protocole_version,
                                         char *server_addr,
                                         unsigned short server_port,
                                         varint_t next_state) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_C2S_handshake_handshake);
  cmc_buffer_pack_varint(buff, protocole_version);
  cmc_buffer_pack_string(buff, server_addr);
  cmc_buffer_pack_ushort(buff, server_port);
  cmc_buffer_pack_varint(buff, next_state);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_status_response(struct cmc_conn *conn, char *response) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_status_response);
  cmc_buffer_pack_string(buff, response);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_status_pong(struct cmc_conn *conn, long payload) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_status_pong);
  cmc_buffer_pack_long(buff, payload);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_C2S_status_request(struct cmc_conn *conn) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_C2S_status_request);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_C2S_status_ping(struct cmc_conn *conn, long payload) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_C2S_status_ping);
  cmc_buffer_pack_long(buff, payload);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_login_disconnect(struct cmc_conn *conn, char *reason) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_login_disconnect);
  cmc_buffer_pack_string(buff, reason);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_login_encryption_request(struct cmc_conn *conn,
                                              char *server_id,
                                              cmc_buffer *public_key,
                                              cmc_buffer *verify_token) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_login_encryption_request);
  cmc_buffer_pack_string(buff, server_id);
  cmc_buffer_pack_byte_array(buff, public_key);
  cmc_buffer_pack_byte_array(buff, verify_token);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_login_success(struct cmc_conn *conn, char *uuid,
                                   char *name) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_login_success);
  cmc_buffer_pack_string(buff, uuid);
  cmc_buffer_pack_string(buff, name);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_login_set_compression(struct cmc_conn *conn,
                                           varint_t threshold) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_login_set_compression);
  cmc_buffer_pack_varint(buff, threshold);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_C2S_login_start(struct cmc_conn *conn, char *name) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_C2S_login_start);
  cmc_buffer_pack_string(buff, name);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_C2S_login_encryption_response(struct cmc_conn *conn,
                                               cmc_buffer *shared_secret,
                                               cmc_buffer *verify_token) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_C2S_login_encryption_response);
  cmc_buffer_pack_byte_array(buff, shared_secret);
  cmc_buffer_pack_byte_array(buff, verify_token);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_keep_alive(struct cmc_conn *conn,
                                     varint_t keep_alive_id) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_keep_alive);
  cmc_buffer_pack_varint(buff, keep_alive_id);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_join_game(struct cmc_conn *conn, int entity_id,
                                    unsigned char gamemode, char dimension,
                                    unsigned char difficulty,
                                    unsigned char max_players, char *level_type,
                                    bool reduced_debug_info) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_join_game);
  cmc_buffer_pack_int(buff, entity_id);
  cmc_buffer_pack_byte(buff, gamemode);
  cmc_buffer_pack_char(buff, dimension);
  cmc_buffer_pack_byte(buff, difficulty);
  cmc_buffer_pack_byte(buff, max_players);
  cmc_buffer_pack_string(buff, level_type);
  cmc_buffer_pack_bool(buff, reduced_debug_info);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_chat_message(struct cmc_conn *conn, char *message,
                                       char position) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_chat_message);
  cmc_buffer_pack_string(buff, message);
  cmc_buffer_pack_char(buff, position);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_time_update(struct cmc_conn *conn, long world_age,
                                      long time_of_day) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_time_update);
  cmc_buffer_pack_long(buff, world_age);
  cmc_buffer_pack_long(buff, time_of_day);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_equipment(struct cmc_conn *conn,
                                           varint_t entity_id, short slot,
                                           slot_t *item) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_entity_equipment);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_short(buff, slot);
  cmc_buffer_pack_slot(buff, item);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_spawn_position(struct cmc_conn *conn,
                                         block_pos_t location) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_spawn_position);
  cmc_buffer_pack_position(buff, location);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_update_health(struct cmc_conn *conn, float health,
                                        varint_t food, float food_saturation) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_update_health);
  cmc_buffer_pack_float(buff, health);
  cmc_buffer_pack_varint(buff, food);
  cmc_buffer_pack_float(buff, food_saturation);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_respawn(struct cmc_conn *conn, int dimesion,
                                  unsigned char difficulty,
                                  unsigned char gamemode, char *level_type) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_respawn);
  cmc_buffer_pack_int(buff, dimesion);
  cmc_buffer_pack_byte(buff, difficulty);
  cmc_buffer_pack_byte(buff, gamemode);
  cmc_buffer_pack_string(buff, level_type);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_player_look_and_position(struct cmc_conn *conn,
                                                   double x, double y, double z,
                                                   float yaw, float pitch,
                                                   unsigned char flags) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_player_look_and_position);
  cmc_buffer_pack_double(buff, x);
  cmc_buffer_pack_double(buff, y);
  cmc_buffer_pack_double(buff, z);
  cmc_buffer_pack_float(buff, yaw);
  cmc_buffer_pack_float(buff, pitch);
  cmc_buffer_pack_byte(buff, flags);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_held_item_change(struct cmc_conn *conn, char slot) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_held_item_change);
  cmc_buffer_pack_char(buff, slot);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_use_bed(struct cmc_conn *conn, varint_t entity_id,
                                  block_pos_t location) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_use_bed);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_position(buff, location);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_animation(struct cmc_conn *conn, varint_t entity_id,
                                    unsigned char animation) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_animation);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_byte(buff, animation);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_spawn_player(struct cmc_conn *conn,
                                       varint_t entity_id,
                                       unsigned long long uuid, int x, int y,
                                       int z, unsigned char yaw,
                                       unsigned char pitch, short current_item,
                                       entity_metadata_t meta_data) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_spawn_player);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_ullong(buff, uuid);
  cmc_buffer_pack_int(buff, x);
  cmc_buffer_pack_int(buff, y);
  cmc_buffer_pack_int(buff, z);
  cmc_buffer_pack_byte(buff, yaw);
  cmc_buffer_pack_byte(buff, pitch);
  cmc_buffer_pack_short(buff, current_item);
  cmc_buffer_pack_entity_metadata(buff, meta_data);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_collect_item(struct cmc_conn *conn,
                                       varint_t collected_entity_id,
                                       varint_t collector_entity_id) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_collect_item);
  cmc_buffer_pack_varint(buff, collected_entity_id);
  cmc_buffer_pack_varint(buff, collector_entity_id);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_spawn_mob(struct cmc_conn *conn, varint_t entity_id,
                                    unsigned char type, int x, int y, int z,
                                    unsigned char yaw, unsigned char pitch,
                                    unsigned char head_pitch, short x_vel,
                                    short y_vel, short z_vel,
                                    entity_metadata_t meta_data) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_spawn_mob);
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
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_spawn_painting(struct cmc_conn *conn,
                                         varint_t entity_id, char *title,
                                         block_pos_t location,
                                         unsigned char direction) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_spawn_painting);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_string(buff, title);
  cmc_buffer_pack_position(buff, location);
  cmc_buffer_pack_byte(buff, direction);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_spawn_experience_orb(struct cmc_conn *conn,
                                               varint_t entity_id, int x, int y,
                                               int z, short count) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_spawn_experience_orb);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_int(buff, x);
  cmc_buffer_pack_int(buff, y);
  cmc_buffer_pack_int(buff, z);
  cmc_buffer_pack_short(buff, count);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_velocity(struct cmc_conn *conn,
                                          varint_t entity_id, short x_vel,
                                          short y_vel, short z_vel) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_entity_velocity);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_short(buff, x_vel);
  cmc_buffer_pack_short(buff, y_vel);
  cmc_buffer_pack_short(buff, z_vel);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity(struct cmc_conn *conn, varint_t entity_id) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_entity);
  cmc_buffer_pack_varint(buff, entity_id);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_relative_move(struct cmc_conn *conn,
                                               varint_t entity_id, char delta_x,
                                               char delta_y, char delta_z,
                                               bool on_ground) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_entity_relative_move);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_char(buff, delta_x);
  cmc_buffer_pack_char(buff, delta_y);
  cmc_buffer_pack_char(buff, delta_z);
  cmc_buffer_pack_bool(buff, on_ground);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_look(struct cmc_conn *conn, varint_t entity_id,
                                      unsigned char yaw, unsigned char pitch,
                                      bool on_ground) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_entity_look);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_byte(buff, yaw);
  cmc_buffer_pack_byte(buff, pitch);
  cmc_buffer_pack_bool(buff, on_ground);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_look_and_relative_move(
    struct cmc_conn *conn, varint_t entity_id, char delta_x, char delta_y,
    char delta_z, unsigned char yaw, unsigned char pitch, bool on_ground) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_entity_look_and_relative_move);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_char(buff, delta_x);
  cmc_buffer_pack_char(buff, delta_y);
  cmc_buffer_pack_char(buff, delta_z);
  cmc_buffer_pack_byte(buff, yaw);
  cmc_buffer_pack_byte(buff, pitch);
  cmc_buffer_pack_bool(buff, on_ground);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_teleport(struct cmc_conn *conn,
                                          varint_t entity_id, int x, int y,
                                          int z, unsigned char yaw,
                                          unsigned char pitch, bool on_ground) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_entity_teleport);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_int(buff, x);
  cmc_buffer_pack_int(buff, y);
  cmc_buffer_pack_int(buff, z);
  cmc_buffer_pack_byte(buff, yaw);
  cmc_buffer_pack_byte(buff, pitch);
  cmc_buffer_pack_bool(buff, on_ground);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_head_look(struct cmc_conn *conn,
                                           varint_t entity_id,
                                           unsigned char head_yaw) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_entity_head_look);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_byte(buff, head_yaw);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_status(struct cmc_conn *conn, int entity_id,
                                        char entity_status) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_entity_status);
  cmc_buffer_pack_int(buff, entity_id);
  cmc_buffer_pack_char(buff, entity_status);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_attach_entity(struct cmc_conn *conn, int entity_id,
                                        int vehicle_id, bool leash) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_attach_entity);
  cmc_buffer_pack_int(buff, entity_id);
  cmc_buffer_pack_int(buff, vehicle_id);
  cmc_buffer_pack_bool(buff, leash);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_metadata(struct cmc_conn *conn,
                                          varint_t entity_id,
                                          entity_metadata_t meta_data) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_entity_metadata);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_entity_metadata(buff, meta_data);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_entity_effect(struct cmc_conn *conn,
                                        varint_t entity_id, char effect_id,
                                        char amplifier, varint_t duration,
                                        bool hide_particles) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_entity_effect);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_char(buff, effect_id);
  cmc_buffer_pack_char(buff, amplifier);
  cmc_buffer_pack_varint(buff, duration);
  cmc_buffer_pack_bool(buff, hide_particles);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_remove_entity_effect(struct cmc_conn *conn,
                                               varint_t entity_id,
                                               char effect_id) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_remove_entity_effect);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_char(buff, effect_id);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_set_experience(struct cmc_conn *conn,
                                         float experience_bar, varint_t level,
                                         varint_t total_experience) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_set_experience);
  cmc_buffer_pack_float(buff, experience_bar);
  cmc_buffer_pack_varint(buff, level);
  cmc_buffer_pack_varint(buff, total_experience);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_chunk_data(struct cmc_conn *conn, int chunk_x,
                                     int chunk_z, bool ground_up_continuous,
                                     unsigned short primary_bitmask,
                                     cmc_buffer *chunk) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_chunk_data);
  cmc_buffer_pack_int(buff, chunk_x);
  cmc_buffer_pack_int(buff, chunk_z);
  cmc_buffer_pack_bool(buff, ground_up_continuous);
  cmc_buffer_pack_ushort(buff, primary_bitmask);
  cmc_buffer_pack_byte_array(buff, chunk);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_block_change(struct cmc_conn *conn,
                                       block_pos_t location,
                                       varint_t block_id) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_block_change);
  cmc_buffer_pack_position(buff, location);
  cmc_buffer_pack_varint(buff, block_id);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_block_action(struct cmc_conn *conn,
                                       block_pos_t location,
                                       unsigned char block_data_1,
                                       unsigned char block_data_2,
                                       varint_t block_type) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_block_action);
  cmc_buffer_pack_position(buff, location);
  cmc_buffer_pack_byte(buff, block_data_1);
  cmc_buffer_pack_byte(buff, block_data_2);
  cmc_buffer_pack_varint(buff, block_type);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_block_break_animation(struct cmc_conn *conn,
                                                varint_t entity_id,
                                                block_pos_t location,
                                                char destroy_stage) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_block_break_animation);
  cmc_buffer_pack_varint(buff, entity_id);
  cmc_buffer_pack_position(buff, location);
  cmc_buffer_pack_char(buff, destroy_stage);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_effect(struct cmc_conn *conn, int effect_id,
                                 block_pos_t location, int data, bool d,
                                 int particle_id, bool long_distances, float x,
                                 float y, float z, float x_offset,
                                 float y_offset, float z_offset,
                                 float particle_data, int particle_count,
                                 int sable_relative_volume) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_effect);
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
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_sound_effect(struct cmc_conn *conn, char *sound_name,
                                       int x, int y, int z, float volume,
                                       unsigned char pitch) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_sound_effect);
  cmc_buffer_pack_string(buff, sound_name);
  cmc_buffer_pack_int(buff, x);
  cmc_buffer_pack_int(buff, y);
  cmc_buffer_pack_int(buff, z);
  cmc_buffer_pack_float(buff, volume);
  cmc_buffer_pack_byte(buff, pitch);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_change_game_state(struct cmc_conn *conn,
                                            unsigned char reason, float value) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_change_game_state);
  cmc_buffer_pack_byte(buff, reason);
  cmc_buffer_pack_float(buff, value);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_player_abilities(struct cmc_conn *conn, char flags,
                                           float flying_speed,
                                           float fov_modifier) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_player_abilities);
  cmc_buffer_pack_char(buff, flags);
  cmc_buffer_pack_float(buff, flying_speed);
  cmc_buffer_pack_float(buff, fov_modifier);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_plugin_message(struct cmc_conn *conn, char *channel,
                                         cmc_buffer *data) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_plugin_message);
  cmc_buffer_pack_string(buff, channel);
  cmc_buffer_pack_byte_array(buff, data);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_disconnect(struct cmc_conn *conn, char *reason) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_disconnect);
  cmc_buffer_pack_string(buff, reason);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_S2C_play_change_difficulty(struct cmc_conn *conn,
                                            unsigned char difficulty) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_S2C_play_change_difficulty);
  cmc_buffer_pack_byte(buff, difficulty);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

void send_packet_C2S_play_keep_alive(struct cmc_conn *conn,
                                     varint_t keep_alive_id) {
  cmc_buffer *buff = cmc_buffer_init();
  cmc_buffer_pack_varint(buff, packetid_C2S_play_keep_alive);
  cmc_buffer_pack_varint(buff, keep_alive_id);
  ERR_CHECK;
  cmc_conn_send_packet(conn, buff);
}

// CGSE: send_methods_c

#define ERR_ACTION return packet;
// CGSS: unpack_methods_c
C2S_handshake_handshake_packet_t
unpack_C2S_handshake_handshake_packet(cmc_buffer *buff) {
  C2S_handshake_handshake_packet_t packet;
  packet.protocole_version = cmc_buffer_unpack_varint(buff);
  packet.server_addr = cmc_buffer_unpack_string(buff);
  packet.server_port = cmc_buffer_unpack_ushort(buff);
  packet.next_state = cmc_buffer_unpack_varint(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_status_response_packet_t
unpack_S2C_status_response_packet(cmc_buffer *buff) {
  S2C_status_response_packet_t packet;
  packet.response = cmc_buffer_unpack_string(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_status_pong_packet_t unpack_S2C_status_pong_packet(cmc_buffer *buff) {
  S2C_status_pong_packet_t packet;
  packet.payload = cmc_buffer_unpack_long(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

C2S_status_ping_packet_t unpack_C2S_status_ping_packet(cmc_buffer *buff) {
  C2S_status_ping_packet_t packet;
  packet.payload = cmc_buffer_unpack_long(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_login_disconnect_packet_t
unpack_S2C_login_disconnect_packet(cmc_buffer *buff) {
  S2C_login_disconnect_packet_t packet;
  packet.reason = cmc_buffer_unpack_string(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_login_encryption_request_packet_t
unpack_S2C_login_encryption_request_packet(cmc_buffer *buff) {
  S2C_login_encryption_request_packet_t packet;
  packet.server_id = cmc_buffer_unpack_string(buff);
  packet.public_key = cmc_buffer_unpack_byte_array(buff);
  packet.verify_token = cmc_buffer_unpack_byte_array(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_login_success_packet_t unpack_S2C_login_success_packet(cmc_buffer *buff) {
  S2C_login_success_packet_t packet;
  packet.uuid = cmc_buffer_unpack_string(buff);
  packet.name = cmc_buffer_unpack_string(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_login_set_compression_packet_t
unpack_S2C_login_set_compression_packet(cmc_buffer *buff) {
  S2C_login_set_compression_packet_t packet;
  packet.threshold = cmc_buffer_unpack_varint(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

C2S_login_start_packet_t unpack_C2S_login_start_packet(cmc_buffer *buff) {
  C2S_login_start_packet_t packet;
  packet.name = cmc_buffer_unpack_string(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

C2S_login_encryption_response_packet_t
unpack_C2S_login_encryption_response_packet(cmc_buffer *buff) {
  C2S_login_encryption_response_packet_t packet;
  packet.shared_secret = cmc_buffer_unpack_byte_array(buff);
  packet.verify_token = cmc_buffer_unpack_byte_array(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_keep_alive_packet_t
unpack_S2C_play_keep_alive_packet(cmc_buffer *buff) {
  S2C_play_keep_alive_packet_t packet;
  packet.keep_alive_id = cmc_buffer_unpack_varint(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_join_game_packet_t unpack_S2C_play_join_game_packet(cmc_buffer *buff) {
  S2C_play_join_game_packet_t packet;
  packet.entity_id = cmc_buffer_unpack_int(buff);
  packet.gamemode = cmc_buffer_unpack_byte(buff);
  packet.dimension = cmc_buffer_unpack_char(buff);
  packet.difficulty = cmc_buffer_unpack_byte(buff);
  packet.max_players = cmc_buffer_unpack_byte(buff);
  packet.level_type = cmc_buffer_unpack_string(buff);
  packet.reduced_debug_info = cmc_buffer_unpack_bool(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_chat_message_packet_t
unpack_S2C_play_chat_message_packet(cmc_buffer *buff) {
  S2C_play_chat_message_packet_t packet;
  packet.message = cmc_buffer_unpack_string(buff);
  packet.position = cmc_buffer_unpack_char(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_time_update_packet_t
unpack_S2C_play_time_update_packet(cmc_buffer *buff) {
  S2C_play_time_update_packet_t packet;
  packet.world_age = cmc_buffer_unpack_long(buff);
  packet.time_of_day = cmc_buffer_unpack_long(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_equipment_packet_t
unpack_S2C_play_entity_equipment_packet(cmc_buffer *buff) {
  S2C_play_entity_equipment_packet_t packet;
  packet.entity_id = cmc_buffer_unpack_varint(buff);
  packet.slot = cmc_buffer_unpack_short(buff);
  packet.item = cmc_buffer_unpack_slot(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_spawn_position_packet_t
unpack_S2C_play_spawn_position_packet(cmc_buffer *buff) {
  S2C_play_spawn_position_packet_t packet;
  packet.location = cmc_buffer_unpack_position(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_update_health_packet_t
unpack_S2C_play_update_health_packet(cmc_buffer *buff) {
  S2C_play_update_health_packet_t packet;
  packet.health = cmc_buffer_unpack_float(buff);
  packet.food = cmc_buffer_unpack_varint(buff);
  packet.food_saturation = cmc_buffer_unpack_float(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_respawn_packet_t unpack_S2C_play_respawn_packet(cmc_buffer *buff) {
  S2C_play_respawn_packet_t packet;
  packet.dimesion = cmc_buffer_unpack_int(buff);
  packet.difficulty = cmc_buffer_unpack_byte(buff);
  packet.gamemode = cmc_buffer_unpack_byte(buff);
  packet.level_type = cmc_buffer_unpack_string(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_player_look_and_position_packet_t
unpack_S2C_play_player_look_and_position_packet(cmc_buffer *buff) {
  S2C_play_player_look_and_position_packet_t packet;
  packet.x = cmc_buffer_unpack_double(buff);
  packet.y = cmc_buffer_unpack_double(buff);
  packet.z = cmc_buffer_unpack_double(buff);
  packet.yaw = cmc_buffer_unpack_float(buff);
  packet.pitch = cmc_buffer_unpack_float(buff);
  packet.flags = cmc_buffer_unpack_byte(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_held_item_change_packet_t
unpack_S2C_play_held_item_change_packet(cmc_buffer *buff) {
  S2C_play_held_item_change_packet_t packet;
  packet.slot = cmc_buffer_unpack_char(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_use_bed_packet_t unpack_S2C_play_use_bed_packet(cmc_buffer *buff) {
  S2C_play_use_bed_packet_t packet;
  packet.entity_id = cmc_buffer_unpack_varint(buff);
  packet.location = cmc_buffer_unpack_position(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_animation_packet_t unpack_S2C_play_animation_packet(cmc_buffer *buff) {
  S2C_play_animation_packet_t packet;
  packet.entity_id = cmc_buffer_unpack_varint(buff);
  packet.animation = cmc_buffer_unpack_byte(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_spawn_player_packet_t
unpack_S2C_play_spawn_player_packet(cmc_buffer *buff) {
  S2C_play_spawn_player_packet_t packet;
  packet.entity_id = cmc_buffer_unpack_varint(buff);
  packet.uuid = cmc_buffer_unpack_ullong(buff);
  packet.x = cmc_buffer_unpack_int(buff);
  packet.y = cmc_buffer_unpack_int(buff);
  packet.z = cmc_buffer_unpack_int(buff);
  packet.yaw = cmc_buffer_unpack_byte(buff);
  packet.pitch = cmc_buffer_unpack_byte(buff);
  packet.current_item = cmc_buffer_unpack_short(buff);
  packet.meta_data = cmc_buffer_unpack_entity_metadata(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_collect_item_packet_t
unpack_S2C_play_collect_item_packet(cmc_buffer *buff) {
  S2C_play_collect_item_packet_t packet;
  packet.collected_entity_id = cmc_buffer_unpack_varint(buff);
  packet.collector_entity_id = cmc_buffer_unpack_varint(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_spawn_mob_packet_t unpack_S2C_play_spawn_mob_packet(cmc_buffer *buff) {
  S2C_play_spawn_mob_packet_t packet;
  packet.entity_id = cmc_buffer_unpack_varint(buff);
  packet.type = cmc_buffer_unpack_byte(buff);
  packet.x = cmc_buffer_unpack_int(buff);
  packet.y = cmc_buffer_unpack_int(buff);
  packet.z = cmc_buffer_unpack_int(buff);
  packet.yaw = cmc_buffer_unpack_byte(buff);
  packet.pitch = cmc_buffer_unpack_byte(buff);
  packet.head_pitch = cmc_buffer_unpack_byte(buff);
  packet.x_vel = cmc_buffer_unpack_short(buff);
  packet.y_vel = cmc_buffer_unpack_short(buff);
  packet.z_vel = cmc_buffer_unpack_short(buff);
  packet.meta_data = cmc_buffer_unpack_entity_metadata(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_spawn_painting_packet_t
unpack_S2C_play_spawn_painting_packet(cmc_buffer *buff) {
  S2C_play_spawn_painting_packet_t packet;
  packet.entity_id = cmc_buffer_unpack_varint(buff);
  packet.title = cmc_buffer_unpack_string(buff);
  packet.location = cmc_buffer_unpack_position(buff);
  packet.direction = cmc_buffer_unpack_byte(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_spawn_experience_orb_packet_t
unpack_S2C_play_spawn_experience_orb_packet(cmc_buffer *buff) {
  S2C_play_spawn_experience_orb_packet_t packet;
  packet.entity_id = cmc_buffer_unpack_varint(buff);
  packet.x = cmc_buffer_unpack_int(buff);
  packet.y = cmc_buffer_unpack_int(buff);
  packet.z = cmc_buffer_unpack_int(buff);
  packet.count = cmc_buffer_unpack_short(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_velocity_packet_t
unpack_S2C_play_entity_velocity_packet(cmc_buffer *buff) {
  S2C_play_entity_velocity_packet_t packet;
  packet.entity_id = cmc_buffer_unpack_varint(buff);
  packet.x_vel = cmc_buffer_unpack_short(buff);
  packet.y_vel = cmc_buffer_unpack_short(buff);
  packet.z_vel = cmc_buffer_unpack_short(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_packet_t unpack_S2C_play_entity_packet(cmc_buffer *buff) {
  S2C_play_entity_packet_t packet;
  packet.entity_id = cmc_buffer_unpack_varint(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_relative_move_packet_t
unpack_S2C_play_entity_relative_move_packet(cmc_buffer *buff) {
  S2C_play_entity_relative_move_packet_t packet;
  packet.entity_id = cmc_buffer_unpack_varint(buff);
  packet.delta_x = cmc_buffer_unpack_char(buff);
  packet.delta_y = cmc_buffer_unpack_char(buff);
  packet.delta_z = cmc_buffer_unpack_char(buff);
  packet.on_ground = cmc_buffer_unpack_bool(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_look_packet_t
unpack_S2C_play_entity_look_packet(cmc_buffer *buff) {
  S2C_play_entity_look_packet_t packet;
  packet.entity_id = cmc_buffer_unpack_varint(buff);
  packet.yaw = cmc_buffer_unpack_byte(buff);
  packet.pitch = cmc_buffer_unpack_byte(buff);
  packet.on_ground = cmc_buffer_unpack_bool(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_look_and_relative_move_packet_t
unpack_S2C_play_entity_look_and_relative_move_packet(cmc_buffer *buff) {
  S2C_play_entity_look_and_relative_move_packet_t packet;
  packet.entity_id = cmc_buffer_unpack_varint(buff);
  packet.delta_x = cmc_buffer_unpack_char(buff);
  packet.delta_y = cmc_buffer_unpack_char(buff);
  packet.delta_z = cmc_buffer_unpack_char(buff);
  packet.yaw = cmc_buffer_unpack_byte(buff);
  packet.pitch = cmc_buffer_unpack_byte(buff);
  packet.on_ground = cmc_buffer_unpack_bool(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_teleport_packet_t
unpack_S2C_play_entity_teleport_packet(cmc_buffer *buff) {
  S2C_play_entity_teleport_packet_t packet;
  packet.entity_id = cmc_buffer_unpack_varint(buff);
  packet.x = cmc_buffer_unpack_int(buff);
  packet.y = cmc_buffer_unpack_int(buff);
  packet.z = cmc_buffer_unpack_int(buff);
  packet.yaw = cmc_buffer_unpack_byte(buff);
  packet.pitch = cmc_buffer_unpack_byte(buff);
  packet.on_ground = cmc_buffer_unpack_bool(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_head_look_packet_t
unpack_S2C_play_entity_head_look_packet(cmc_buffer *buff) {
  S2C_play_entity_head_look_packet_t packet;
  packet.entity_id = cmc_buffer_unpack_varint(buff);
  packet.head_yaw = cmc_buffer_unpack_byte(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_status_packet_t
unpack_S2C_play_entity_status_packet(cmc_buffer *buff) {
  S2C_play_entity_status_packet_t packet;
  packet.entity_id = cmc_buffer_unpack_int(buff);
  packet.entity_status = cmc_buffer_unpack_char(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_attach_entity_packet_t
unpack_S2C_play_attach_entity_packet(cmc_buffer *buff) {
  S2C_play_attach_entity_packet_t packet;
  packet.entity_id = cmc_buffer_unpack_int(buff);
  packet.vehicle_id = cmc_buffer_unpack_int(buff);
  packet.leash = cmc_buffer_unpack_bool(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_metadata_packet_t
unpack_S2C_play_entity_metadata_packet(cmc_buffer *buff) {
  S2C_play_entity_metadata_packet_t packet;
  packet.entity_id = cmc_buffer_unpack_varint(buff);
  packet.meta_data = cmc_buffer_unpack_entity_metadata(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_entity_effect_packet_t
unpack_S2C_play_entity_effect_packet(cmc_buffer *buff) {
  S2C_play_entity_effect_packet_t packet;
  packet.entity_id = cmc_buffer_unpack_varint(buff);
  packet.effect_id = cmc_buffer_unpack_char(buff);
  packet.amplifier = cmc_buffer_unpack_char(buff);
  packet.duration = cmc_buffer_unpack_varint(buff);
  packet.hide_particles = cmc_buffer_unpack_bool(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_remove_entity_effect_packet_t
unpack_S2C_play_remove_entity_effect_packet(cmc_buffer *buff) {
  S2C_play_remove_entity_effect_packet_t packet;
  packet.entity_id = cmc_buffer_unpack_varint(buff);
  packet.effect_id = cmc_buffer_unpack_char(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_set_experience_packet_t
unpack_S2C_play_set_experience_packet(cmc_buffer *buff) {
  S2C_play_set_experience_packet_t packet;
  packet.experience_bar = cmc_buffer_unpack_float(buff);
  packet.level = cmc_buffer_unpack_varint(buff);
  packet.total_experience = cmc_buffer_unpack_varint(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_chunk_data_packet_t
unpack_S2C_play_chunk_data_packet(cmc_buffer *buff) {
  S2C_play_chunk_data_packet_t packet;
  packet.chunk_x = cmc_buffer_unpack_int(buff);
  packet.chunk_z = cmc_buffer_unpack_int(buff);
  packet.ground_up_continuous = cmc_buffer_unpack_bool(buff);
  packet.primary_bitmask = cmc_buffer_unpack_ushort(buff);
  packet.chunk = cmc_buffer_unpack_byte_array(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_block_change_packet_t
unpack_S2C_play_block_change_packet(cmc_buffer *buff) {
  S2C_play_block_change_packet_t packet;
  packet.location = cmc_buffer_unpack_position(buff);
  packet.block_id = cmc_buffer_unpack_varint(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_block_action_packet_t
unpack_S2C_play_block_action_packet(cmc_buffer *buff) {
  S2C_play_block_action_packet_t packet;
  packet.location = cmc_buffer_unpack_position(buff);
  packet.block_data_1 = cmc_buffer_unpack_byte(buff);
  packet.block_data_2 = cmc_buffer_unpack_byte(buff);
  packet.block_type = cmc_buffer_unpack_varint(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_block_break_animation_packet_t
unpack_S2C_play_block_break_animation_packet(cmc_buffer *buff) {
  S2C_play_block_break_animation_packet_t packet;
  packet.entity_id = cmc_buffer_unpack_varint(buff);
  packet.location = cmc_buffer_unpack_position(buff);
  packet.destroy_stage = cmc_buffer_unpack_char(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_effect_packet_t unpack_S2C_play_effect_packet(cmc_buffer *buff) {
  S2C_play_effect_packet_t packet;
  packet.effect_id = cmc_buffer_unpack_int(buff);
  packet.location = cmc_buffer_unpack_position(buff);
  packet.data = cmc_buffer_unpack_int(buff);
  packet.d = cmc_buffer_unpack_bool(buff);
  packet.particle_id = cmc_buffer_unpack_int(buff);
  packet.long_distances = cmc_buffer_unpack_bool(buff);
  packet.x = cmc_buffer_unpack_float(buff);
  packet.y = cmc_buffer_unpack_float(buff);
  packet.z = cmc_buffer_unpack_float(buff);
  packet.x_offset = cmc_buffer_unpack_float(buff);
  packet.y_offset = cmc_buffer_unpack_float(buff);
  packet.z_offset = cmc_buffer_unpack_float(buff);
  packet.particle_data = cmc_buffer_unpack_float(buff);
  packet.particle_count = cmc_buffer_unpack_int(buff);
  packet.sable_relative_volume = cmc_buffer_unpack_int(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_sound_effect_packet_t
unpack_S2C_play_sound_effect_packet(cmc_buffer *buff) {
  S2C_play_sound_effect_packet_t packet;
  packet.sound_name = cmc_buffer_unpack_string(buff);
  packet.x = cmc_buffer_unpack_int(buff);
  packet.y = cmc_buffer_unpack_int(buff);
  packet.z = cmc_buffer_unpack_int(buff);
  packet.volume = cmc_buffer_unpack_float(buff);
  packet.pitch = cmc_buffer_unpack_byte(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_change_game_state_packet_t
unpack_S2C_play_change_game_state_packet(cmc_buffer *buff) {
  S2C_play_change_game_state_packet_t packet;
  packet.reason = cmc_buffer_unpack_byte(buff);
  packet.value = cmc_buffer_unpack_float(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_player_abilities_packet_t
unpack_S2C_play_player_abilities_packet(cmc_buffer *buff) {
  S2C_play_player_abilities_packet_t packet;
  packet.flags = cmc_buffer_unpack_char(buff);
  packet.flying_speed = cmc_buffer_unpack_float(buff);
  packet.fov_modifier = cmc_buffer_unpack_float(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_plugin_message_packet_t
unpack_S2C_play_plugin_message_packet(cmc_buffer *buff) {
  S2C_play_plugin_message_packet_t packet;
  packet.channel = cmc_buffer_unpack_string(buff);
  packet.data = cmc_buffer_unpack_byte_array(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_disconnect_packet_t
unpack_S2C_play_disconnect_packet(cmc_buffer *buff) {
  S2C_play_disconnect_packet_t packet;
  packet.reason = cmc_buffer_unpack_string(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

S2C_play_change_difficulty_packet_t
unpack_S2C_play_change_difficulty_packet(cmc_buffer *buff) {
  S2C_play_change_difficulty_packet_t packet;
  packet.difficulty = cmc_buffer_unpack_byte(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

C2S_play_keep_alive_packet_t
unpack_C2S_play_keep_alive_packet(cmc_buffer *buff) {
  C2S_play_keep_alive_packet_t packet;
  packet.keep_alive_id = cmc_buffer_unpack_varint(buff);
  UNPACK_ERR_HANDELER;
  return packet;
}

// CGSE: unpack_methods_c

// CGSS: free_methods_c
void free_C2S_handshake_handshake_packet(
    C2S_handshake_handshake_packet_t packet) {
  free_string(packet.server_addr);
}
void free_S2C_status_response_packet(S2C_status_response_packet_t packet) {
  free_string(packet.response);
}

void free_S2C_login_disconnect_packet(S2C_login_disconnect_packet_t packet) {
  free_string(packet.reason);
}
void free_S2C_login_encryption_request_packet(
    S2C_login_encryption_request_packet_t packet) {
  free_string(packet.server_id);
  free_byte_array(packet.public_key);
  free_byte_array(packet.verify_token);
}
void free_S2C_login_success_packet(S2C_login_success_packet_t packet) {
  free_string(packet.uuid);
  free_string(packet.name);
}

void free_C2S_login_start_packet(C2S_login_start_packet_t packet) {
  free_string(packet.name);
}
void free_C2S_login_encryption_response_packet(
    C2S_login_encryption_response_packet_t packet) {
  free_byte_array(packet.shared_secret);
  free_byte_array(packet.verify_token);
}

void free_S2C_play_join_game_packet(S2C_play_join_game_packet_t packet) {
  free_string(packet.level_type);
}
void free_S2C_play_chat_message_packet(S2C_play_chat_message_packet_t packet) {
  free_string(packet.message);
}

void free_S2C_play_entity_equipment_packet(
    S2C_play_entity_equipment_packet_t packet) {
  free_slot(packet.item);
}

void free_S2C_play_respawn_packet(S2C_play_respawn_packet_t packet) {
  free_string(packet.level_type);
}

void free_S2C_play_spawn_player_packet(S2C_play_spawn_player_packet_t packet) {
  free_entity_metadata(packet.meta_data);
}

void free_S2C_play_spawn_mob_packet(S2C_play_spawn_mob_packet_t packet) {
  free_entity_metadata(packet.meta_data);
}
void free_S2C_play_spawn_painting_packet(
    S2C_play_spawn_painting_packet_t packet) {
  free_string(packet.title);
}

void free_S2C_play_entity_metadata_packet(
    S2C_play_entity_metadata_packet_t packet) {
  free_entity_metadata(packet.meta_data);
}

void free_S2C_play_chunk_data_packet(S2C_play_chunk_data_packet_t packet) {
  free_byte_array(packet.chunk);
}

void free_S2C_play_sound_effect_packet(S2C_play_sound_effect_packet_t packet) {
  free_string(packet.sound_name);
}

void free_S2C_play_plugin_message_packet(
    S2C_play_plugin_message_packet_t packet) {
  free_string(packet.channel);
  free_byte_array(packet.data);
}
void free_S2C_play_disconnect_packet(S2C_play_disconnect_packet_t packet) {
  free_string(packet.reason);
}

// CGSE: free_methods_c