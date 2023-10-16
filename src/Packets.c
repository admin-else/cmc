//This code is generated.

#include "Packets.h"
#include "MCbuffer.h"
#include "MConn.h"
#include "MCtypes.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap-utils.h"

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
    FREE(buff);                                                                \
    *errmsg = strdup(error_message);                                           \
    return;                                                                    \
  }

#define UNPACK_ERR_HANDELER(unpack_name)                                       \
  if (*errmsg != NULL) {                                                       \
    char error_message[256];                                                   \
    sprintf(error_message, "error while unpacking " #unpack_name ": %s",       \
            *errmsg);                                                          \
    *errmsg = strdup(error_message);                                           \
    return packet;                                                             \
  }

void send_packet_keep_alive(MConn *conn, varint_t keep_alive_id, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_varint(buff, keep_alive_id, errmsg);
  PACK_ERR_HANDELER(keep_alive);
  MConn_send_and_free_buffer(conn, buff, errmsg);
}

keep_alive_packet_t unpack_keep_alive_packet(MCbuffer *buff, char **errmsg) {
  keep_alive_packet_t packet;
  packet.keep_alive_id=MCbuffer_unpack_varint(buff,errmsg);
  UNPACK_ERR_HANDELER(keep_alive);
  return packet;
}

void send_packet_join_game(MConn *conn, int entity_id, byte_t gamemode, char dimension, byte_t difficulty, byte_t max_players, char * level_type, bool reduced_debug_info, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_int(buff, entity_id, errmsg);
  MCbuffer_pack_byte(buff, gamemode, errmsg);
  MCbuffer_pack_char(buff, dimension, errmsg);
  MCbuffer_pack_byte(buff, difficulty, errmsg);
  MCbuffer_pack_byte(buff, max_players, errmsg);
  MCbuffer_pack_string(buff, level_type, errmsg);
  MCbuffer_pack_bool(buff, reduced_debug_info, errmsg);
  PACK_ERR_HANDELER(join_game);
  MConn_send_and_free_buffer(conn, buff, errmsg);
}

join_game_packet_t unpack_join_game_packet(MCbuffer *buff, char **errmsg) {
  join_game_packet_t packet;
  packet.entity_id=MCbuffer_unpack_int(buff,errmsg);
  packet.gamemode=MCbuffer_unpack_byte(buff,errmsg);
  packet.dimension=MCbuffer_unpack_char(buff,errmsg);
  packet.difficulty=MCbuffer_unpack_byte(buff,errmsg);
  packet.max_players=MCbuffer_unpack_byte(buff,errmsg);
  packet.level_type=MCbuffer_unpack_string(buff,errmsg);
  packet.reduced_debug_info=MCbuffer_unpack_bool(buff,errmsg);
  UNPACK_ERR_HANDELER(join_game);
  return packet;
}

void send_packet_chat_message(MConn *conn, char * message, char position, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_string(buff, message, errmsg);
  MCbuffer_pack_char(buff, position, errmsg);
  PACK_ERR_HANDELER(chat_message);
  MConn_send_and_free_buffer(conn, buff, errmsg);
}

chat_message_packet_t unpack_chat_message_packet(MCbuffer *buff, char **errmsg) {
  chat_message_packet_t packet;
  packet.message=MCbuffer_unpack_string(buff,errmsg);
  packet.position=MCbuffer_unpack_char(buff,errmsg);
  UNPACK_ERR_HANDELER(chat_message);
  return packet;
}

void send_packet_time_update(MConn *conn, long world_age, long time_of_day, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_long(buff, world_age, errmsg);
  MCbuffer_pack_long(buff, time_of_day, errmsg);
  PACK_ERR_HANDELER(time_update);
  MConn_send_and_free_buffer(conn, buff, errmsg);
}

time_update_packet_t unpack_time_update_packet(MCbuffer *buff, char **errmsg) {
  time_update_packet_t packet;
  packet.world_age=MCbuffer_unpack_long(buff,errmsg);
  packet.time_of_day=MCbuffer_unpack_long(buff,errmsg);
  UNPACK_ERR_HANDELER(time_update);
  return packet;
}

