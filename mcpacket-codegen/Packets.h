//This code is generated.

#ifndef PACKET_H
#define PACKET_H

#include "MCbuffer.h"
#include "MConn.h"
#include "MCtypes.h"
#include <stdbool.h>

typedef struct {
  varint_t keep_alive_id;
} keep_alive_packet_t;

void send_packet_keep_alive(MConn *conn, varint_t keep_alive_id, char **errmsg);

keep_alive_packet_t unpack_keep_alive_packet(MCbuffer *buff, char **errmsg);

typedef struct {
  int entity_id;
  byte_t gamemode;
  char dimension;
  byte_t difficulty;
  byte_t max_players;
  char * level_type;
  bool reduced_debug_info;
} join_game_packet_t;

void send_packet_join_game(MConn *conn, int entity_id, byte_t gamemode, char dimension, byte_t difficulty, byte_t max_players, char * level_type, bool reduced_debug_info, char **errmsg);

join_game_packet_t unpack_join_game_packet(MCbuffer *buff, char **errmsg);

typedef struct {
  char * message;
  char position;
} chat_message_packet_t;

void send_packet_chat_message(MConn *conn, char * message, char position, char **errmsg);

chat_message_packet_t unpack_chat_message_packet(MCbuffer *buff, char **errmsg);

typedef struct {
  long world_age;
  long time_of_day;
} time_update_packet_t;

void send_packet_time_update(MConn *conn, long world_age, long time_of_day, char **errmsg);

time_update_packet_t unpack_time_update_packet(MCbuffer *buff, char **errmsg);

#endif