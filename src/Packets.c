#include "Packets.h"
#include "MCbuffer.h"
#include "MConn.h"
#include "MCtypes.h"
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

#define PACK_ERR_HANDELER(pack_name)                                           \
  if (*errmsg != NULL) {                                                       \
    char error_message[256];                                                   \
    sprintf(error_message, "error while packing " #pack_name ": %s", *errmsg); \
    free(buff);                                                                \
    *errmsg = strdup(error_message);                                           \
    return;                                                                    \
  }

#define UNPACK_ERR_HANDELER(unpack_name)                                       \
  if (buff->position != buff->length)                                          \
    *errmsg = "unpack function didnt do the entire buffer";                    \
  if (*errmsg != NULL) {                                                       \
    char error_message[256];                                                   \
    sprintf(error_message, "error while unpacking " #unpack_name ": %s",       \
            *errmsg);                                                          \
    *errmsg = strdup(error_message);                                           \
    return packet;                                                             \
  }

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
                               "C2S_HANDSHAKE");
    PACKET_DATA_TO_STRING_UTIL(0x00, CONN_STATE_STATUS, DIRECTION_S2C,
                               "S2C_STATUS_RESPONSE");
    PACKET_DATA_TO_STRING_UTIL(0x01, CONN_STATE_STATUS, DIRECTION_S2C,
                               "S2C_STATUS_PONG");
    PACKET_DATA_TO_STRING_UTIL(0x00, CONN_STATE_STATUS, DIRECTION_C2S,
                               "C2S_STATUS_REQUEST");
    PACKET_DATA_TO_STRING_UTIL(0x01, CONN_STATE_STATUS, DIRECTION_C2S,
                               "C2S_STATUS_PING");
    PACKET_DATA_TO_STRING_UTIL(0x00, CONN_STATE_LOGIN, DIRECTION_S2C,
                               "S2C_LOGIN_DISCONNECT");
    PACKET_DATA_TO_STRING_UTIL(0x01, CONN_STATE_LOGIN, DIRECTION_S2C,
                               "S2C_LOGIN_ENCRYPTION_REQUEST");
    PACKET_DATA_TO_STRING_UTIL(0x02, CONN_STATE_LOGIN, DIRECTION_S2C,
                               "S2C_LOGIN_SUCCESS");
    PACKET_DATA_TO_STRING_UTIL(0x03, CONN_STATE_LOGIN, DIRECTION_S2C,
                               "S2C_LOGIN_SET_COMPRESSION");
    PACKET_DATA_TO_STRING_UTIL(0x00, CONN_STATE_LOGIN, DIRECTION_C2S,
                               "C2S_LOGIN_START");
    PACKET_DATA_TO_STRING_UTIL(0x01, CONN_STATE_LOGIN, DIRECTION_C2S,
                               "C2S_LOGIN_ENCRYPTION_RESPONSE");
    PACKET_DATA_TO_STRING_UTIL(0x00, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_KEEP_ALIVE");
    PACKET_DATA_TO_STRING_UTIL(0x01, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_JOIN_GAME");
    PACKET_DATA_TO_STRING_UTIL(0x02, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_CHAT_MESSAGE");
    PACKET_DATA_TO_STRING_UTIL(0x03, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_TIME_UPDATE");
    PACKET_DATA_TO_STRING_UTIL(0x04, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_ENTITY_EQUIPMENT");
    PACKET_DATA_TO_STRING_UTIL(0x05, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_SPAWN_POSITION");
    PACKET_DATA_TO_STRING_UTIL(0x06, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_UPDATE_HEALTH");
    PACKET_DATA_TO_STRING_UTIL(0x07, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_RESPAWN");
    PACKET_DATA_TO_STRING_UTIL(0x08, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_PLAYER_LOOK_AND_POSITION");
    PACKET_DATA_TO_STRING_UTIL(0x09, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_HELD_ITEM_CHANGE");
    PACKET_DATA_TO_STRING_UTIL(0x0A, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_USE_BED");
    PACKET_DATA_TO_STRING_UTIL(0x0B, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_ANIMATION");
    PACKET_DATA_TO_STRING_UTIL(0x0C, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_SPAWN_PLAYER");
    PACKET_DATA_TO_STRING_UTIL(0x0D, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_COLLECT_ITEM");
    PACKET_DATA_TO_STRING_UTIL(0x0F, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_SPAWN_MOB");
    PACKET_DATA_TO_STRING_UTIL(0x10, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_SPAWN_PAINTING");
    PACKET_DATA_TO_STRING_UTIL(0x11, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_SPAWN_EXPERIENCE_ORB");
    PACKET_DATA_TO_STRING_UTIL(0x12, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_ENTITY_VELOCITY");
    PACKET_DATA_TO_STRING_UTIL(0x14, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_ENTITY");
    PACKET_DATA_TO_STRING_UTIL(0x15, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_ENTITY_RELATIVE_MOVE");
    PACKET_DATA_TO_STRING_UTIL(0x16, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_ENTITY_LOOK");
    PACKET_DATA_TO_STRING_UTIL(0x17, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_ENTITY_LOOK_AND_RELATIVE_MOVE");
    PACKET_DATA_TO_STRING_UTIL(0x18, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_ENTITY_TELEPORT");
    PACKET_DATA_TO_STRING_UTIL(0x19, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_ENTITY_HEAD_LOOK");
    PACKET_DATA_TO_STRING_UTIL(0x1A, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_ENTITY_STATUS");
    PACKET_DATA_TO_STRING_UTIL(0x1B, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_ATTACH_ENTITY");
    PACKET_DATA_TO_STRING_UTIL(0x1C, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_ENTITY_METADATA");
    PACKET_DATA_TO_STRING_UTIL(0x1D, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_ENTITY_EFFECT");
    PACKET_DATA_TO_STRING_UTIL(0x1E, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_REMOVE_ENTITY_EFFECT");
    PACKET_DATA_TO_STRING_UTIL(0x1F, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_SET_EXPERIENCE");
    PACKET_DATA_TO_STRING_UTIL(0x21, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_CHUNK_DATA");
    PACKET_DATA_TO_STRING_UTIL(0x23, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_BLOCK_CHANGE");
    PACKET_DATA_TO_STRING_UTIL(0x24, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_BLOCK_ACTION");
    PACKET_DATA_TO_STRING_UTIL(0x25, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_BLOCK_BREAK_ANIMATION");
    PACKET_DATA_TO_STRING_UTIL(0x28, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_EFFECT");
    PACKET_DATA_TO_STRING_UTIL(0x29, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_SOUND_EFFECT");
    PACKET_DATA_TO_STRING_UTIL(0x2B, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_CHANGE_GAME_STATE");
    PACKET_DATA_TO_STRING_UTIL(0x39, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_PLAYER_ABILITIES");
    PACKET_DATA_TO_STRING_UTIL(0x3F, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_PLUGIN_MESSAGE");
    PACKET_DATA_TO_STRING_UTIL(0x40, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_DISCONNECT");
    PACKET_DATA_TO_STRING_UTIL(0x41, CONN_STATE_PLAY, DIRECTION_S2C,
                               "S2C_PLAY_CHANGE_DIFFICULTY");
    PACKET_DATA_TO_STRING_UTIL(0x00, CONN_STATE_PLAY, DIRECTION_C2S,
                               "C2S_PLAY_KEEP_ALIVE");
    // CGSE: packet_data_to_string
  default:
    return "PACKET_UNKNOWN";
  }

#undef PACKET_DATA_TO_STRING_UTIL
}

// CGSS: send_methods
void send_packet_C2S_handshake(MConn *conn, varint_t protocole_version,
                               char *server_addr, unsigned short server_port,
                               varint_t next_state, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_C2S_HANDSHAKE, errmsg);
  MCbuffer_pack_varint(buff, protocole_version, errmsg);
  MCbuffer_pack_string(buff, server_addr, errmsg);
  MCbuffer_pack_ushort(buff, server_port, errmsg);
  MCbuffer_pack_varint(buff, next_state, errmsg);
  PACK_ERR_HANDELER(C2S_handshake);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_status_response(MConn *conn, json_t *response,
                                     char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_STATUS_RESPONSE, errmsg);
  MCbuffer_pack_json(buff, response, errmsg);
  PACK_ERR_HANDELER(S2C_status_response);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_status_pong(MConn *conn, long payload, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_STATUS_PONG, errmsg);
  MCbuffer_pack_long(buff, payload, errmsg);
  PACK_ERR_HANDELER(S2C_status_pong);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_C2S_status_request(MConn *conn, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_C2S_STATUS_REQUEST, errmsg);
  PACK_ERR_HANDELER(C2S_status_request);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_C2S_status_ping(MConn *conn, long payload, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_C2S_STATUS_PING, errmsg);
  MCbuffer_pack_long(buff, payload, errmsg);
  PACK_ERR_HANDELER(C2S_status_ping);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_login_disconnect(MConn *conn, json_t *reason,
                                      char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_LOGIN_DISCONNECT, errmsg);
  MCbuffer_pack_json(buff, reason, errmsg);
  PACK_ERR_HANDELER(S2C_login_disconnect);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_login_encryption_request(MConn *conn, char *server_id,
                                              MCbuffer *public_key,
                                              MCbuffer *verify_token,
                                              char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_LOGIN_ENCRYPTION_REQUEST, errmsg);
  MCbuffer_pack_string(buff, server_id, errmsg);
  MCbuffer_pack_byte_array(buff, public_key, errmsg);
  MCbuffer_pack_byte_array(buff, verify_token, errmsg);
  PACK_ERR_HANDELER(S2C_login_encryption_request);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_login_success(MConn *conn, char *uuid, char *name,
                                   char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_LOGIN_SUCCESS, errmsg);
  MCbuffer_pack_string(buff, uuid, errmsg);
  MCbuffer_pack_string(buff, name, errmsg);
  PACK_ERR_HANDELER(S2C_login_success);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_login_set_compression(MConn *conn, varint_t threshold,
                                           char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_LOGIN_SET_COMPRESSION, errmsg);
  MCbuffer_pack_varint(buff, threshold, errmsg);
  PACK_ERR_HANDELER(S2C_login_set_compression);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_C2S_login_start(MConn *conn, char *name, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_C2S_LOGIN_START, errmsg);
  MCbuffer_pack_string(buff, name, errmsg);
  PACK_ERR_HANDELER(C2S_login_start);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_C2S_login_encryption_response(MConn *conn,
                                               MCbuffer *shared_secret,
                                               MCbuffer *verify_token,
                                               char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_C2S_LOGIN_ENCRYPTION_RESPONSE, errmsg);
  MCbuffer_pack_byte_array(buff, shared_secret, errmsg);
  MCbuffer_pack_byte_array(buff, verify_token, errmsg);
  PACK_ERR_HANDELER(C2S_login_encryption_response);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_keep_alive(MConn *conn, varint_t keep_alive_id,
                                     char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_KEEP_ALIVE, errmsg);
  MCbuffer_pack_varint(buff, keep_alive_id, errmsg);
  PACK_ERR_HANDELER(S2C_play_keep_alive);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_join_game(MConn *conn, int entity_id, byte_t gamemode,
                                    char dimension, byte_t difficulty,
                                    byte_t max_players, char *level_type,
                                    bool reduced_debug_info, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_JOIN_GAME, errmsg);
  MCbuffer_pack_int(buff, entity_id, errmsg);
  MCbuffer_pack_byte(buff, gamemode, errmsg);
  MCbuffer_pack_char(buff, dimension, errmsg);
  MCbuffer_pack_byte(buff, difficulty, errmsg);
  MCbuffer_pack_byte(buff, max_players, errmsg);
  MCbuffer_pack_string(buff, level_type, errmsg);
  MCbuffer_pack_bool(buff, reduced_debug_info, errmsg);
  PACK_ERR_HANDELER(S2C_play_join_game);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_chat_message(MConn *conn, json_t *message,
                                       char position, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_CHAT_MESSAGE, errmsg);
  MCbuffer_pack_json(buff, message, errmsg);
  MCbuffer_pack_char(buff, position, errmsg);
  PACK_ERR_HANDELER(S2C_play_chat_message);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_time_update(MConn *conn, long world_age,
                                      long time_of_day, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_TIME_UPDATE, errmsg);
  MCbuffer_pack_long(buff, world_age, errmsg);
  MCbuffer_pack_long(buff, time_of_day, errmsg);
  PACK_ERR_HANDELER(S2C_play_time_update);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_entity_equipment(MConn *conn, varint_t entity_id,
                                           short slot, slot_t *item,
                                           char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_ENTITY_EQUIPMENT, errmsg);
  MCbuffer_pack_varint(buff, entity_id, errmsg);
  MCbuffer_pack_short(buff, slot, errmsg);
  MCbuffer_pack_slot(buff, item, errmsg);
  PACK_ERR_HANDELER(S2C_play_entity_equipment);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_spawn_position(MConn *conn, block_pos_t location,
                                         char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_SPAWN_POSITION, errmsg);
  MCbuffer_pack_position(buff, location, errmsg);
  PACK_ERR_HANDELER(S2C_play_spawn_position);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_update_health(MConn *conn, float health,
                                        varint_t food, float food_saturation,
                                        char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_UPDATE_HEALTH, errmsg);
  MCbuffer_pack_float(buff, health, errmsg);
  MCbuffer_pack_varint(buff, food, errmsg);
  MCbuffer_pack_float(buff, food_saturation, errmsg);
  PACK_ERR_HANDELER(S2C_play_update_health);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_respawn(MConn *conn, int dimesion, byte_t difficulty,
                                  byte_t gamemode, char *level_type,
                                  char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_RESPAWN, errmsg);
  MCbuffer_pack_int(buff, dimesion, errmsg);
  MCbuffer_pack_byte(buff, difficulty, errmsg);
  MCbuffer_pack_byte(buff, gamemode, errmsg);
  MCbuffer_pack_string(buff, level_type, errmsg);
  PACK_ERR_HANDELER(S2C_play_respawn);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_player_look_and_position(MConn *conn, double x,
                                                   double y, double z,
                                                   float yaw, float pitch,
                                                   byte_t flags,
                                                   char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_PLAYER_LOOK_AND_POSITION,
                       errmsg);
  MCbuffer_pack_double(buff, x, errmsg);
  MCbuffer_pack_double(buff, y, errmsg);
  MCbuffer_pack_double(buff, z, errmsg);
  MCbuffer_pack_float(buff, yaw, errmsg);
  MCbuffer_pack_float(buff, pitch, errmsg);
  MCbuffer_pack_byte(buff, flags, errmsg);
  PACK_ERR_HANDELER(S2C_play_player_look_and_position);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_held_item_change(MConn *conn, char slot,
                                           char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_HELD_ITEM_CHANGE, errmsg);
  MCbuffer_pack_char(buff, slot, errmsg);
  PACK_ERR_HANDELER(S2C_play_held_item_change);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_use_bed(MConn *conn, varint_t entity_id,
                                  block_pos_t location, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_USE_BED, errmsg);
  MCbuffer_pack_varint(buff, entity_id, errmsg);
  MCbuffer_pack_position(buff, location, errmsg);
  PACK_ERR_HANDELER(S2C_play_use_bed);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_animation(MConn *conn, varint_t entity_id,
                                    byte_t animation, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_ANIMATION, errmsg);
  MCbuffer_pack_varint(buff, entity_id, errmsg);
  MCbuffer_pack_byte(buff, animation, errmsg);
  PACK_ERR_HANDELER(S2C_play_animation);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_spawn_player(MConn *conn, varint_t entity_id,
                                       unsigned long long uuid, int x, int y,
                                       int z, byte_t yaw, byte_t pitch,
                                       short current_item,
                                       entity_metadata_t meta_data,
                                       char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_SPAWN_PLAYER, errmsg);
  MCbuffer_pack_varint(buff, entity_id, errmsg);
  MCbuffer_pack_ullong(buff, uuid, errmsg);
  MCbuffer_pack_int(buff, x, errmsg);
  MCbuffer_pack_int(buff, y, errmsg);
  MCbuffer_pack_int(buff, z, errmsg);
  MCbuffer_pack_byte(buff, yaw, errmsg);
  MCbuffer_pack_byte(buff, pitch, errmsg);
  MCbuffer_pack_short(buff, current_item, errmsg);
  MCbuffer_pack_entity_metadata(buff, meta_data, errmsg);
  PACK_ERR_HANDELER(S2C_play_spawn_player);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_collect_item(MConn *conn,
                                       varint_t collected_entity_id,
                                       varint_t collector_entity_id,
                                       char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_COLLECT_ITEM, errmsg);
  MCbuffer_pack_varint(buff, collected_entity_id, errmsg);
  MCbuffer_pack_varint(buff, collector_entity_id, errmsg);
  PACK_ERR_HANDELER(S2C_play_collect_item);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_spawn_mob(MConn *conn, varint_t entity_id,
                                    byte_t type, int x, int y, int z,
                                    byte_t yaw, byte_t pitch, byte_t head_pitch,
                                    short x_vel, short y_vel, short z_vel,
                                    entity_metadata_t meta_data,
                                    char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_SPAWN_MOB, errmsg);
  MCbuffer_pack_varint(buff, entity_id, errmsg);
  MCbuffer_pack_byte(buff, type, errmsg);
  MCbuffer_pack_int(buff, x, errmsg);
  MCbuffer_pack_int(buff, y, errmsg);
  MCbuffer_pack_int(buff, z, errmsg);
  MCbuffer_pack_byte(buff, yaw, errmsg);
  MCbuffer_pack_byte(buff, pitch, errmsg);
  MCbuffer_pack_byte(buff, head_pitch, errmsg);
  MCbuffer_pack_short(buff, x_vel, errmsg);
  MCbuffer_pack_short(buff, y_vel, errmsg);
  MCbuffer_pack_short(buff, z_vel, errmsg);
  MCbuffer_pack_entity_metadata(buff, meta_data, errmsg);
  PACK_ERR_HANDELER(S2C_play_spawn_mob);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_spawn_painting(MConn *conn, varint_t entity_id,
                                         char *title, block_pos_t location,
                                         byte_t direction, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_SPAWN_PAINTING, errmsg);
  MCbuffer_pack_varint(buff, entity_id, errmsg);
  MCbuffer_pack_string(buff, title, errmsg);
  MCbuffer_pack_position(buff, location, errmsg);
  MCbuffer_pack_byte(buff, direction, errmsg);
  PACK_ERR_HANDELER(S2C_play_spawn_painting);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_spawn_experience_orb(MConn *conn, varint_t entity_id,
                                               int x, int y, int z, short count,
                                               char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_SPAWN_EXPERIENCE_ORB, errmsg);
  MCbuffer_pack_varint(buff, entity_id, errmsg);
  MCbuffer_pack_int(buff, x, errmsg);
  MCbuffer_pack_int(buff, y, errmsg);
  MCbuffer_pack_int(buff, z, errmsg);
  MCbuffer_pack_short(buff, count, errmsg);
  PACK_ERR_HANDELER(S2C_play_spawn_experience_orb);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_entity_velocity(MConn *conn, varint_t entity_id,
                                          short x_vel, short y_vel, short z_vel,
                                          char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_ENTITY_VELOCITY, errmsg);
  MCbuffer_pack_varint(buff, entity_id, errmsg);
  MCbuffer_pack_short(buff, x_vel, errmsg);
  MCbuffer_pack_short(buff, y_vel, errmsg);
  MCbuffer_pack_short(buff, z_vel, errmsg);
  PACK_ERR_HANDELER(S2C_play_entity_velocity);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_entity(MConn *conn, varint_t entity_id,
                                 char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_ENTITY, errmsg);
  MCbuffer_pack_varint(buff, entity_id, errmsg);
  PACK_ERR_HANDELER(S2C_play_entity);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_entity_relative_move(MConn *conn, varint_t entity_id,
                                               char delta_x, char delta_y,
                                               char delta_z, bool on_ground,
                                               char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_ENTITY_RELATIVE_MOVE, errmsg);
  MCbuffer_pack_varint(buff, entity_id, errmsg);
  MCbuffer_pack_char(buff, delta_x, errmsg);
  MCbuffer_pack_char(buff, delta_y, errmsg);
  MCbuffer_pack_char(buff, delta_z, errmsg);
  MCbuffer_pack_bool(buff, on_ground, errmsg);
  PACK_ERR_HANDELER(S2C_play_entity_relative_move);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_entity_look(MConn *conn, varint_t entity_id,
                                      byte_t yaw, byte_t pitch, bool on_ground,
                                      char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_ENTITY_LOOK, errmsg);
  MCbuffer_pack_varint(buff, entity_id, errmsg);
  MCbuffer_pack_byte(buff, yaw, errmsg);
  MCbuffer_pack_byte(buff, pitch, errmsg);
  MCbuffer_pack_bool(buff, on_ground, errmsg);
  PACK_ERR_HANDELER(S2C_play_entity_look);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_entity_look_and_relative_move(
    MConn *conn, varint_t entity_id, char delta_x, char delta_y, char delta_z,
    byte_t yaw, byte_t pitch, bool on_ground, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_ENTITY_LOOK_AND_RELATIVE_MOVE,
                       errmsg);
  MCbuffer_pack_varint(buff, entity_id, errmsg);
  MCbuffer_pack_char(buff, delta_x, errmsg);
  MCbuffer_pack_char(buff, delta_y, errmsg);
  MCbuffer_pack_char(buff, delta_z, errmsg);
  MCbuffer_pack_byte(buff, yaw, errmsg);
  MCbuffer_pack_byte(buff, pitch, errmsg);
  MCbuffer_pack_bool(buff, on_ground, errmsg);
  PACK_ERR_HANDELER(S2C_play_entity_look_and_relative_move);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_entity_teleport(MConn *conn, varint_t entity_id,
                                          int x, int y, int z, byte_t yaw,
                                          byte_t pitch, bool on_ground,
                                          char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_ENTITY_TELEPORT, errmsg);
  MCbuffer_pack_varint(buff, entity_id, errmsg);
  MCbuffer_pack_int(buff, x, errmsg);
  MCbuffer_pack_int(buff, y, errmsg);
  MCbuffer_pack_int(buff, z, errmsg);
  MCbuffer_pack_byte(buff, yaw, errmsg);
  MCbuffer_pack_byte(buff, pitch, errmsg);
  MCbuffer_pack_bool(buff, on_ground, errmsg);
  PACK_ERR_HANDELER(S2C_play_entity_teleport);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_entity_head_look(MConn *conn, varint_t entity_id,
                                           byte_t head_yaw, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_ENTITY_HEAD_LOOK, errmsg);
  MCbuffer_pack_varint(buff, entity_id, errmsg);
  MCbuffer_pack_byte(buff, head_yaw, errmsg);
  PACK_ERR_HANDELER(S2C_play_entity_head_look);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_entity_status(MConn *conn, int entity_id,
                                        char entity_status, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_ENTITY_STATUS, errmsg);
  MCbuffer_pack_int(buff, entity_id, errmsg);
  MCbuffer_pack_char(buff, entity_status, errmsg);
  PACK_ERR_HANDELER(S2C_play_entity_status);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_attach_entity(MConn *conn, int entity_id,
                                        int vehicle_id, bool leash,
                                        char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_ATTACH_ENTITY, errmsg);
  MCbuffer_pack_int(buff, entity_id, errmsg);
  MCbuffer_pack_int(buff, vehicle_id, errmsg);
  MCbuffer_pack_bool(buff, leash, errmsg);
  PACK_ERR_HANDELER(S2C_play_attach_entity);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_entity_metadata(MConn *conn, varint_t entity_id,
                                          entity_metadata_t meta_data,
                                          char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_ENTITY_METADATA, errmsg);
  MCbuffer_pack_varint(buff, entity_id, errmsg);
  MCbuffer_pack_entity_metadata(buff, meta_data, errmsg);
  PACK_ERR_HANDELER(S2C_play_entity_metadata);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_entity_effect(MConn *conn, varint_t entity_id,
                                        char effect_id, char amplifier,
                                        varint_t duration, bool hide_particles,
                                        char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_ENTITY_EFFECT, errmsg);
  MCbuffer_pack_varint(buff, entity_id, errmsg);
  MCbuffer_pack_char(buff, effect_id, errmsg);
  MCbuffer_pack_char(buff, amplifier, errmsg);
  MCbuffer_pack_varint(buff, duration, errmsg);
  MCbuffer_pack_bool(buff, hide_particles, errmsg);
  PACK_ERR_HANDELER(S2C_play_entity_effect);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_remove_entity_effect(MConn *conn, varint_t entity_id,
                                               char effect_id, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_REMOVE_ENTITY_EFFECT, errmsg);
  MCbuffer_pack_varint(buff, entity_id, errmsg);
  MCbuffer_pack_char(buff, effect_id, errmsg);
  PACK_ERR_HANDELER(S2C_play_remove_entity_effect);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_set_experience(MConn *conn, float experience_bar,
                                         varint_t level,
                                         varint_t total_experience,
                                         char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_SET_EXPERIENCE, errmsg);
  MCbuffer_pack_float(buff, experience_bar, errmsg);
  MCbuffer_pack_varint(buff, level, errmsg);
  MCbuffer_pack_varint(buff, total_experience, errmsg);
  PACK_ERR_HANDELER(S2C_play_set_experience);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_chunk_data(MConn *conn, int chunk_x, int chunk_z,
                                     bool ground_up_continuous,
                                     unsigned short primary_bitmask,
                                     MCbuffer *chunk, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_CHUNK_DATA, errmsg);
  MCbuffer_pack_int(buff, chunk_x, errmsg);
  MCbuffer_pack_int(buff, chunk_z, errmsg);
  MCbuffer_pack_bool(buff, ground_up_continuous, errmsg);
  MCbuffer_pack_ushort(buff, primary_bitmask, errmsg);
  MCbuffer_pack_byte_array(buff, chunk, errmsg);
  PACK_ERR_HANDELER(S2C_play_chunk_data);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_block_change(MConn *conn, block_pos_t location,
                                       varint_t block_id, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_BLOCK_CHANGE, errmsg);
  MCbuffer_pack_position(buff, location, errmsg);
  MCbuffer_pack_varint(buff, block_id, errmsg);
  PACK_ERR_HANDELER(S2C_play_block_change);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_block_action(MConn *conn, block_pos_t location,
                                       byte_t block_data_1, byte_t block_data_2,
                                       varint_t block_type, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_BLOCK_ACTION, errmsg);
  MCbuffer_pack_position(buff, location, errmsg);
  MCbuffer_pack_byte(buff, block_data_1, errmsg);
  MCbuffer_pack_byte(buff, block_data_2, errmsg);
  MCbuffer_pack_varint(buff, block_type, errmsg);
  PACK_ERR_HANDELER(S2C_play_block_action);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_block_break_animation(MConn *conn, varint_t entity_id,
                                                block_pos_t location,
                                                char destroy_stage,
                                                char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_BLOCK_BREAK_ANIMATION, errmsg);
  MCbuffer_pack_varint(buff, entity_id, errmsg);
  MCbuffer_pack_position(buff, location, errmsg);
  MCbuffer_pack_char(buff, destroy_stage, errmsg);
  PACK_ERR_HANDELER(S2C_play_block_break_animation);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_effect(MConn *conn, int effect_id,
                                 block_pos_t location, int data, bool d,
                                 int particle_id, bool long_distances, float x,
                                 float y, float z, float x_offset,
                                 float y_offset, float z_offset,
                                 float particle_data, int particle_count,
                                 int sable_relative_volume, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_EFFECT, errmsg);
  MCbuffer_pack_int(buff, effect_id, errmsg);
  MCbuffer_pack_position(buff, location, errmsg);
  MCbuffer_pack_int(buff, data, errmsg);
  MCbuffer_pack_bool(buff, d, errmsg);
  MCbuffer_pack_int(buff, particle_id, errmsg);
  MCbuffer_pack_bool(buff, long_distances, errmsg);
  MCbuffer_pack_float(buff, x, errmsg);
  MCbuffer_pack_float(buff, y, errmsg);
  MCbuffer_pack_float(buff, z, errmsg);
  MCbuffer_pack_float(buff, x_offset, errmsg);
  MCbuffer_pack_float(buff, y_offset, errmsg);
  MCbuffer_pack_float(buff, z_offset, errmsg);
  MCbuffer_pack_float(buff, particle_data, errmsg);
  MCbuffer_pack_int(buff, particle_count, errmsg);
  MCbuffer_pack_int(buff, sable_relative_volume, errmsg);
  PACK_ERR_HANDELER(S2C_play_effect);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_sound_effect(MConn *conn, char *sound_name, int x,
                                       int y, int z, float volume, byte_t pitch,
                                       char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_SOUND_EFFECT, errmsg);
  MCbuffer_pack_string(buff, sound_name, errmsg);
  MCbuffer_pack_int(buff, x, errmsg);
  MCbuffer_pack_int(buff, y, errmsg);
  MCbuffer_pack_int(buff, z, errmsg);
  MCbuffer_pack_float(buff, volume, errmsg);
  MCbuffer_pack_byte(buff, pitch, errmsg);
  PACK_ERR_HANDELER(S2C_play_sound_effect);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_change_game_state(MConn *conn, byte_t reason,
                                            float value, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_CHANGE_GAME_STATE, errmsg);
  MCbuffer_pack_byte(buff, reason, errmsg);
  MCbuffer_pack_float(buff, value, errmsg);
  PACK_ERR_HANDELER(S2C_play_change_game_state);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_player_abilities(MConn *conn, char flags,
                                           float flying_speed,
                                           float fov_modifier, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_PLAYER_ABILITIES, errmsg);
  MCbuffer_pack_char(buff, flags, errmsg);
  MCbuffer_pack_float(buff, flying_speed, errmsg);
  MCbuffer_pack_float(buff, fov_modifier, errmsg);
  PACK_ERR_HANDELER(S2C_play_player_abilities);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_plugin_message(MConn *conn, char *channel,
                                         MCbuffer *data, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_PLUGIN_MESSAGE, errmsg);
  MCbuffer_pack_string(buff, channel, errmsg);
  MCbuffer_pack_byte_array(buff, data, errmsg);
  PACK_ERR_HANDELER(S2C_play_plugin_message);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_disconnect(MConn *conn, char *reason, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_DISCONNECT, errmsg);
  MCbuffer_pack_string(buff, reason, errmsg);
  PACK_ERR_HANDELER(S2C_play_disconnect);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_S2C_play_change_difficulty(MConn *conn, byte_t difficulty,
                                            char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_S2C_PLAY_CHANGE_DIFFICULTY, errmsg);
  MCbuffer_pack_byte(buff, difficulty, errmsg);
  PACK_ERR_HANDELER(S2C_play_change_difficulty);
  MConn_send_packet(conn, buff, errmsg);
}

void send_packet_C2S_play_keep_alive(MConn *conn, varint_t keep_alive_id,
                                     char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, PACKETID_C2S_PLAY_KEEP_ALIVE, errmsg);
  MCbuffer_pack_varint(buff, keep_alive_id, errmsg);
  PACK_ERR_HANDELER(C2S_play_keep_alive);
  MConn_send_packet(conn, buff, errmsg);
}

// CGSE: send_methods

// CGSS: unpack_methods
C2S_handshake_packet_t unpack_C2S_handshake_packet(MCbuffer *buff,
                                                   char **errmsg) {
  C2S_handshake_packet_t packet;
  packet.protocole_version = MCbuffer_unpack_varint(buff, errmsg);
  packet.server_addr = MCbuffer_unpack_string(buff, errmsg);
  packet.server_port = MCbuffer_unpack_ushort(buff, errmsg);
  packet.next_state = MCbuffer_unpack_varint(buff, errmsg);
  UNPACK_ERR_HANDELER(C2S_handshake);
  return packet;
}

S2C_status_response_packet_t unpack_S2C_status_response_packet(MCbuffer *buff,
                                                               char **errmsg) {
  S2C_status_response_packet_t packet;
  packet.response = MCbuffer_unpack_json(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_status_response);
  return packet;
}

S2C_status_pong_packet_t unpack_S2C_status_pong_packet(MCbuffer *buff,
                                                       char **errmsg) {
  S2C_status_pong_packet_t packet;
  packet.payload = MCbuffer_unpack_long(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_status_pong);
  return packet;
}

C2S_status_ping_packet_t unpack_C2S_status_ping_packet(MCbuffer *buff,
                                                       char **errmsg) {
  C2S_status_ping_packet_t packet;
  packet.payload = MCbuffer_unpack_long(buff, errmsg);
  UNPACK_ERR_HANDELER(C2S_status_ping);
  return packet;
}

S2C_login_disconnect_packet_t
unpack_S2C_login_disconnect_packet(MCbuffer *buff, char **errmsg) {
  S2C_login_disconnect_packet_t packet;
  packet.reason = MCbuffer_unpack_json(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_login_disconnect);
  return packet;
}

S2C_login_encryption_request_packet_t
unpack_S2C_login_encryption_request_packet(MCbuffer *buff, char **errmsg) {
  S2C_login_encryption_request_packet_t packet;
  packet.server_id = MCbuffer_unpack_string(buff, errmsg);
  packet.public_key = MCbuffer_unpack_byte_array(buff, errmsg);
  packet.verify_token = MCbuffer_unpack_byte_array(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_login_encryption_request);
  return packet;
}

S2C_login_success_packet_t unpack_S2C_login_success_packet(MCbuffer *buff,
                                                           char **errmsg) {
  S2C_login_success_packet_t packet;
  packet.uuid = MCbuffer_unpack_string(buff, errmsg);
  packet.name = MCbuffer_unpack_string(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_login_success);
  return packet;
}

S2C_login_set_compression_packet_t
unpack_S2C_login_set_compression_packet(MCbuffer *buff, char **errmsg) {
  S2C_login_set_compression_packet_t packet;
  packet.threshold = MCbuffer_unpack_varint(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_login_set_compression);
  return packet;
}

C2S_login_start_packet_t unpack_C2S_login_start_packet(MCbuffer *buff,
                                                       char **errmsg) {
  C2S_login_start_packet_t packet;
  packet.name = MCbuffer_unpack_string(buff, errmsg);
  UNPACK_ERR_HANDELER(C2S_login_start);
  return packet;
}

C2S_login_encryption_response_packet_t
unpack_C2S_login_encryption_response_packet(MCbuffer *buff, char **errmsg) {
  C2S_login_encryption_response_packet_t packet;
  packet.shared_secret = MCbuffer_unpack_byte_array(buff, errmsg);
  packet.verify_token = MCbuffer_unpack_byte_array(buff, errmsg);
  UNPACK_ERR_HANDELER(C2S_login_encryption_response);
  return packet;
}

S2C_play_keep_alive_packet_t unpack_S2C_play_keep_alive_packet(MCbuffer *buff,
                                                               char **errmsg) {
  S2C_play_keep_alive_packet_t packet;
  packet.keep_alive_id = MCbuffer_unpack_varint(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_keep_alive);
  return packet;
}

S2C_play_join_game_packet_t unpack_S2C_play_join_game_packet(MCbuffer *buff,
                                                             char **errmsg) {
  S2C_play_join_game_packet_t packet;
  packet.entity_id = MCbuffer_unpack_int(buff, errmsg);
  packet.gamemode = MCbuffer_unpack_byte(buff, errmsg);
  packet.dimension = MCbuffer_unpack_char(buff, errmsg);
  packet.difficulty = MCbuffer_unpack_byte(buff, errmsg);
  packet.max_players = MCbuffer_unpack_byte(buff, errmsg);
  packet.level_type = MCbuffer_unpack_string(buff, errmsg);
  packet.reduced_debug_info = MCbuffer_unpack_bool(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_join_game);
  return packet;
}

S2C_play_chat_message_packet_t
unpack_S2C_play_chat_message_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_chat_message_packet_t packet;
  packet.message = MCbuffer_unpack_json(buff, errmsg);
  packet.position = MCbuffer_unpack_char(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_chat_message);
  return packet;
}

S2C_play_time_update_packet_t
unpack_S2C_play_time_update_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_time_update_packet_t packet;
  packet.world_age = MCbuffer_unpack_long(buff, errmsg);
  packet.time_of_day = MCbuffer_unpack_long(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_time_update);
  return packet;
}

S2C_play_entity_equipment_packet_t
unpack_S2C_play_entity_equipment_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_entity_equipment_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff, errmsg);
  packet.slot = MCbuffer_unpack_short(buff, errmsg);
  packet.item = MCbuffer_unpack_slot(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_entity_equipment);
  return packet;
}

S2C_play_spawn_position_packet_t
unpack_S2C_play_spawn_position_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_spawn_position_packet_t packet;
  packet.location = MCbuffer_unpack_position(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_spawn_position);
  return packet;
}

S2C_play_update_health_packet_t
unpack_S2C_play_update_health_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_update_health_packet_t packet;
  packet.health = MCbuffer_unpack_float(buff, errmsg);
  packet.food = MCbuffer_unpack_varint(buff, errmsg);
  packet.food_saturation = MCbuffer_unpack_float(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_update_health);
  return packet;
}

S2C_play_respawn_packet_t unpack_S2C_play_respawn_packet(MCbuffer *buff,
                                                         char **errmsg) {
  S2C_play_respawn_packet_t packet;
  packet.dimesion = MCbuffer_unpack_int(buff, errmsg);
  packet.difficulty = MCbuffer_unpack_byte(buff, errmsg);
  packet.gamemode = MCbuffer_unpack_byte(buff, errmsg);
  packet.level_type = MCbuffer_unpack_string(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_respawn);
  return packet;
}

S2C_play_player_look_and_position_packet_t
unpack_S2C_play_player_look_and_position_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_player_look_and_position_packet_t packet;
  packet.x = MCbuffer_unpack_double(buff, errmsg);
  packet.y = MCbuffer_unpack_double(buff, errmsg);
  packet.z = MCbuffer_unpack_double(buff, errmsg);
  packet.yaw = MCbuffer_unpack_float(buff, errmsg);
  packet.pitch = MCbuffer_unpack_float(buff, errmsg);
  packet.flags = MCbuffer_unpack_byte(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_player_look_and_position);
  return packet;
}

S2C_play_held_item_change_packet_t
unpack_S2C_play_held_item_change_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_held_item_change_packet_t packet;
  packet.slot = MCbuffer_unpack_char(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_held_item_change);
  return packet;
}

S2C_play_use_bed_packet_t unpack_S2C_play_use_bed_packet(MCbuffer *buff,
                                                         char **errmsg) {
  S2C_play_use_bed_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff, errmsg);
  packet.location = MCbuffer_unpack_position(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_use_bed);
  return packet;
}

S2C_play_animation_packet_t unpack_S2C_play_animation_packet(MCbuffer *buff,
                                                             char **errmsg) {
  S2C_play_animation_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff, errmsg);
  packet.animation = MCbuffer_unpack_byte(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_animation);
  return packet;
}

S2C_play_spawn_player_packet_t
unpack_S2C_play_spawn_player_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_spawn_player_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff, errmsg);
  packet.uuid = MCbuffer_unpack_ullong(buff, errmsg);
  packet.x = MCbuffer_unpack_int(buff, errmsg);
  packet.y = MCbuffer_unpack_int(buff, errmsg);
  packet.z = MCbuffer_unpack_int(buff, errmsg);
  packet.yaw = MCbuffer_unpack_byte(buff, errmsg);
  packet.pitch = MCbuffer_unpack_byte(buff, errmsg);
  packet.current_item = MCbuffer_unpack_short(buff, errmsg);
  packet.meta_data = MCbuffer_unpack_entity_metadata(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_spawn_player);
  return packet;
}

S2C_play_collect_item_packet_t
unpack_S2C_play_collect_item_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_collect_item_packet_t packet;
  packet.collected_entity_id = MCbuffer_unpack_varint(buff, errmsg);
  packet.collector_entity_id = MCbuffer_unpack_varint(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_collect_item);
  return packet;
}

S2C_play_spawn_mob_packet_t unpack_S2C_play_spawn_mob_packet(MCbuffer *buff,
                                                             char **errmsg) {
  S2C_play_spawn_mob_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff, errmsg);
  packet.type = MCbuffer_unpack_byte(buff, errmsg);
  packet.x = MCbuffer_unpack_int(buff, errmsg);
  packet.y = MCbuffer_unpack_int(buff, errmsg);
  packet.z = MCbuffer_unpack_int(buff, errmsg);
  packet.yaw = MCbuffer_unpack_byte(buff, errmsg);
  packet.pitch = MCbuffer_unpack_byte(buff, errmsg);
  packet.head_pitch = MCbuffer_unpack_byte(buff, errmsg);
  packet.x_vel = MCbuffer_unpack_short(buff, errmsg);
  packet.y_vel = MCbuffer_unpack_short(buff, errmsg);
  packet.z_vel = MCbuffer_unpack_short(buff, errmsg);
  packet.meta_data = MCbuffer_unpack_entity_metadata(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_spawn_mob);
  return packet;
}

S2C_play_spawn_painting_packet_t
unpack_S2C_play_spawn_painting_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_spawn_painting_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff, errmsg);
  packet.title = MCbuffer_unpack_string(buff, errmsg);
  packet.location = MCbuffer_unpack_position(buff, errmsg);
  packet.direction = MCbuffer_unpack_byte(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_spawn_painting);
  return packet;
}

S2C_play_spawn_experience_orb_packet_t
unpack_S2C_play_spawn_experience_orb_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_spawn_experience_orb_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff, errmsg);
  packet.x = MCbuffer_unpack_int(buff, errmsg);
  packet.y = MCbuffer_unpack_int(buff, errmsg);
  packet.z = MCbuffer_unpack_int(buff, errmsg);
  packet.count = MCbuffer_unpack_short(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_spawn_experience_orb);
  return packet;
}

S2C_play_entity_velocity_packet_t
unpack_S2C_play_entity_velocity_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_entity_velocity_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff, errmsg);
  packet.x_vel = MCbuffer_unpack_short(buff, errmsg);
  packet.y_vel = MCbuffer_unpack_short(buff, errmsg);
  packet.z_vel = MCbuffer_unpack_short(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_entity_velocity);
  return packet;
}

S2C_play_entity_packet_t unpack_S2C_play_entity_packet(MCbuffer *buff,
                                                       char **errmsg) {
  S2C_play_entity_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_entity);
  return packet;
}

S2C_play_entity_relative_move_packet_t
unpack_S2C_play_entity_relative_move_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_entity_relative_move_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff, errmsg);
  packet.delta_x = MCbuffer_unpack_char(buff, errmsg);
  packet.delta_y = MCbuffer_unpack_char(buff, errmsg);
  packet.delta_z = MCbuffer_unpack_char(buff, errmsg);
  packet.on_ground = MCbuffer_unpack_bool(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_entity_relative_move);
  return packet;
}

S2C_play_entity_look_packet_t
unpack_S2C_play_entity_look_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_entity_look_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff, errmsg);
  packet.yaw = MCbuffer_unpack_byte(buff, errmsg);
  packet.pitch = MCbuffer_unpack_byte(buff, errmsg);
  packet.on_ground = MCbuffer_unpack_bool(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_entity_look);
  return packet;
}

S2C_play_entity_look_and_relative_move_packet_t
unpack_S2C_play_entity_look_and_relative_move_packet(MCbuffer *buff,
                                                     char **errmsg) {
  S2C_play_entity_look_and_relative_move_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff, errmsg);
  packet.delta_x = MCbuffer_unpack_char(buff, errmsg);
  packet.delta_y = MCbuffer_unpack_char(buff, errmsg);
  packet.delta_z = MCbuffer_unpack_char(buff, errmsg);
  packet.yaw = MCbuffer_unpack_byte(buff, errmsg);
  packet.pitch = MCbuffer_unpack_byte(buff, errmsg);
  packet.on_ground = MCbuffer_unpack_bool(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_entity_look_and_relative_move);
  return packet;
}

S2C_play_entity_teleport_packet_t
unpack_S2C_play_entity_teleport_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_entity_teleport_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff, errmsg);
  packet.x = MCbuffer_unpack_int(buff, errmsg);
  packet.y = MCbuffer_unpack_int(buff, errmsg);
  packet.z = MCbuffer_unpack_int(buff, errmsg);
  packet.yaw = MCbuffer_unpack_byte(buff, errmsg);
  packet.pitch = MCbuffer_unpack_byte(buff, errmsg);
  packet.on_ground = MCbuffer_unpack_bool(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_entity_teleport);
  return packet;
}

S2C_play_entity_head_look_packet_t
unpack_S2C_play_entity_head_look_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_entity_head_look_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff, errmsg);
  packet.head_yaw = MCbuffer_unpack_byte(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_entity_head_look);
  return packet;
}

S2C_play_entity_status_packet_t
unpack_S2C_play_entity_status_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_entity_status_packet_t packet;
  packet.entity_id = MCbuffer_unpack_int(buff, errmsg);
  packet.entity_status = MCbuffer_unpack_char(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_entity_status);
  return packet;
}

S2C_play_attach_entity_packet_t
unpack_S2C_play_attach_entity_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_attach_entity_packet_t packet;
  packet.entity_id = MCbuffer_unpack_int(buff, errmsg);
  packet.vehicle_id = MCbuffer_unpack_int(buff, errmsg);
  packet.leash = MCbuffer_unpack_bool(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_attach_entity);
  return packet;
}

S2C_play_entity_metadata_packet_t
unpack_S2C_play_entity_metadata_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_entity_metadata_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff, errmsg);
  packet.meta_data = MCbuffer_unpack_entity_metadata(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_entity_metadata);
  return packet;
}

S2C_play_entity_effect_packet_t
unpack_S2C_play_entity_effect_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_entity_effect_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff, errmsg);
  packet.effect_id = MCbuffer_unpack_char(buff, errmsg);
  packet.amplifier = MCbuffer_unpack_char(buff, errmsg);
  packet.duration = MCbuffer_unpack_varint(buff, errmsg);
  packet.hide_particles = MCbuffer_unpack_bool(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_entity_effect);
  return packet;
}

S2C_play_remove_entity_effect_packet_t
unpack_S2C_play_remove_entity_effect_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_remove_entity_effect_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff, errmsg);
  packet.effect_id = MCbuffer_unpack_char(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_remove_entity_effect);
  return packet;
}

S2C_play_set_experience_packet_t
unpack_S2C_play_set_experience_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_set_experience_packet_t packet;
  packet.experience_bar = MCbuffer_unpack_float(buff, errmsg);
  packet.level = MCbuffer_unpack_varint(buff, errmsg);
  packet.total_experience = MCbuffer_unpack_varint(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_set_experience);
  return packet;
}

S2C_play_chunk_data_packet_t unpack_S2C_play_chunk_data_packet(MCbuffer *buff,
                                                               char **errmsg) {
  S2C_play_chunk_data_packet_t packet;
  packet.chunk_x = MCbuffer_unpack_int(buff, errmsg);
  packet.chunk_z = MCbuffer_unpack_int(buff, errmsg);
  packet.ground_up_continuous = MCbuffer_unpack_bool(buff, errmsg);
  packet.primary_bitmask = MCbuffer_unpack_ushort(buff, errmsg);
  packet.chunk = MCbuffer_unpack_byte_array(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_chunk_data);
  return packet;
}

S2C_play_block_change_packet_t
unpack_S2C_play_block_change_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_block_change_packet_t packet;
  packet.location = MCbuffer_unpack_position(buff, errmsg);
  packet.block_id = MCbuffer_unpack_varint(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_block_change);
  return packet;
}

S2C_play_block_action_packet_t
unpack_S2C_play_block_action_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_block_action_packet_t packet;
  packet.location = MCbuffer_unpack_position(buff, errmsg);
  packet.block_data_1 = MCbuffer_unpack_byte(buff, errmsg);
  packet.block_data_2 = MCbuffer_unpack_byte(buff, errmsg);
  packet.block_type = MCbuffer_unpack_varint(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_block_action);
  return packet;
}

S2C_play_block_break_animation_packet_t
unpack_S2C_play_block_break_animation_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_block_break_animation_packet_t packet;
  packet.entity_id = MCbuffer_unpack_varint(buff, errmsg);
  packet.location = MCbuffer_unpack_position(buff, errmsg);
  packet.destroy_stage = MCbuffer_unpack_char(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_block_break_animation);
  return packet;
}

S2C_play_effect_packet_t unpack_S2C_play_effect_packet(MCbuffer *buff,
                                                       char **errmsg) {
  S2C_play_effect_packet_t packet;
  packet.effect_id = MCbuffer_unpack_int(buff, errmsg);
  packet.location = MCbuffer_unpack_position(buff, errmsg);
  packet.data = MCbuffer_unpack_int(buff, errmsg);
  packet.d = MCbuffer_unpack_bool(buff, errmsg);
  packet.particle_id = MCbuffer_unpack_int(buff, errmsg);
  packet.long_distances = MCbuffer_unpack_bool(buff, errmsg);
  packet.x = MCbuffer_unpack_float(buff, errmsg);
  packet.y = MCbuffer_unpack_float(buff, errmsg);
  packet.z = MCbuffer_unpack_float(buff, errmsg);
  packet.x_offset = MCbuffer_unpack_float(buff, errmsg);
  packet.y_offset = MCbuffer_unpack_float(buff, errmsg);
  packet.z_offset = MCbuffer_unpack_float(buff, errmsg);
  packet.particle_data = MCbuffer_unpack_float(buff, errmsg);
  packet.particle_count = MCbuffer_unpack_int(buff, errmsg);
  packet.sable_relative_volume = MCbuffer_unpack_int(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_effect);
  return packet;
}

S2C_play_sound_effect_packet_t
unpack_S2C_play_sound_effect_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_sound_effect_packet_t packet;
  packet.sound_name = MCbuffer_unpack_string(buff, errmsg);
  packet.x = MCbuffer_unpack_int(buff, errmsg);
  packet.y = MCbuffer_unpack_int(buff, errmsg);
  packet.z = MCbuffer_unpack_int(buff, errmsg);
  packet.volume = MCbuffer_unpack_float(buff, errmsg);
  packet.pitch = MCbuffer_unpack_byte(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_sound_effect);
  return packet;
}

S2C_play_change_game_state_packet_t
unpack_S2C_play_change_game_state_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_change_game_state_packet_t packet;
  packet.reason = MCbuffer_unpack_byte(buff, errmsg);
  packet.value = MCbuffer_unpack_float(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_change_game_state);
  return packet;
}

S2C_play_player_abilities_packet_t
unpack_S2C_play_player_abilities_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_player_abilities_packet_t packet;
  packet.flags = MCbuffer_unpack_char(buff, errmsg);
  packet.flying_speed = MCbuffer_unpack_float(buff, errmsg);
  packet.fov_modifier = MCbuffer_unpack_float(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_player_abilities);
  return packet;
}

S2C_play_plugin_message_packet_t
unpack_S2C_play_plugin_message_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_plugin_message_packet_t packet;
  packet.channel = MCbuffer_unpack_string(buff, errmsg);
  packet.data = MCbuffer_unpack_byte_array(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_plugin_message);
  return packet;
}

S2C_play_disconnect_packet_t unpack_S2C_play_disconnect_packet(MCbuffer *buff,
                                                               char **errmsg) {
  S2C_play_disconnect_packet_t packet;
  packet.reason = MCbuffer_unpack_string(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_disconnect);
  return packet;
}

S2C_play_change_difficulty_packet_t
unpack_S2C_play_change_difficulty_packet(MCbuffer *buff, char **errmsg) {
  S2C_play_change_difficulty_packet_t packet;
  packet.difficulty = MCbuffer_unpack_byte(buff, errmsg);
  UNPACK_ERR_HANDELER(S2C_play_change_difficulty);
  return packet;
}

C2S_play_keep_alive_packet_t unpack_C2S_play_keep_alive_packet(MCbuffer *buff,
                                                               char **errmsg) {
  C2S_play_keep_alive_packet_t packet;
  packet.keep_alive_id = MCbuffer_unpack_varint(buff, errmsg);
  UNPACK_ERR_HANDELER(C2S_play_keep_alive);
  return packet;
}

// CGSE: unpack_methods