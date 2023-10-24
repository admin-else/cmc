//This code is generated.
#pragma once

#include "MCbuffer.h"
#include "MConn.h"
#include "MCtypes.h"
#include <stdbool.h>
#include <jansson.h>

#define PACKETID_C2S_HANDSHAKE 0x00
#define PACKETID_S2C_STATUS_RESPONSE 0x00
#define PACKETID_C2S_STATUS_REQUEST 0x00
#define PACKETID_S2C_LOGIN_DISCONNECT 0x00
#define PACKETID_S2C_LOGIN_ENCRYPTION_REQUEST 0x01
#define PACKETID_S2C_LOGIN_SUCCESS 0x02
#define PACKETID_S2C_LOGIN_SET_COMPRESSION 0x03
#define PACKETID_C2S_LOGIN_START 0x00
#define PACKETID_C2S_LOGIN_ENCRYPTION_RESPONSE 0x01
#define PACKETID_S2C_PLAY_KEEP_ALIVE 0x00
#define PACKETID_S2C_PLAY_JOIN_GAME 0x02
#define PACKETID_C2S_PLAY_KEEP_ALIVE 0x00

typedef struct {
  varint_t protocole_version;
  char * server_addr;
  unsigned short server_port;
  varint_t next_state;
} C2S_handshake_packet_t;

void send_packet_C2S_handshake(MConn *conn, varint_t protocole_version, char * server_addr, unsigned short server_port, varint_t next_state, char **errmsg);

C2S_handshake_packet_t unpack_C2S_handshake_packet(MCbuffer *buff, char **errmsg);

typedef struct {
  json_t * response;
} S2C_status_response_packet_t;

void send_packet_S2C_status_response(MConn *conn, json_t * response, char **errmsg);

S2C_status_response_packet_t unpack_S2C_status_response_packet(MCbuffer *buff, char **errmsg);

void send_packet_C2S_status_request(MConn *conn, char **errmsg);

typedef struct {
  json_t * reason;
} S2C_login_disconnect_packet_t;

void send_packet_S2C_login_disconnect(MConn *conn, json_t * reason, char **errmsg);

S2C_login_disconnect_packet_t unpack_S2C_login_disconnect_packet(MCbuffer *buff, char **errmsg);

typedef struct {
  char * server_id;
  MCbuffer * public_key;
  MCbuffer * verify_token;
} S2C_login_encryption_request_packet_t;

void send_packet_S2C_login_encryption_request(MConn *conn, char * server_id, MCbuffer * public_key, MCbuffer * verify_token, char **errmsg);

S2C_login_encryption_request_packet_t unpack_S2C_login_encryption_request_packet(MCbuffer *buff, char **errmsg);

typedef struct {
  char * uuid;
  char * name;
} S2C_login_success_packet_t;

void send_packet_S2C_login_success(MConn *conn, char * uuid, char * name, char **errmsg);

S2C_login_success_packet_t unpack_S2C_login_success_packet(MCbuffer *buff, char **errmsg);

typedef struct {
  varint_t threshold;
} S2C_login_set_compression_packet_t;

void send_packet_S2C_login_set_compression(MConn *conn, varint_t threshold, char **errmsg);

S2C_login_set_compression_packet_t unpack_S2C_login_set_compression_packet(MCbuffer *buff, char **errmsg);

typedef struct {
  char * name;
} C2S_login_start_packet_t;

void send_packet_C2S_login_start(MConn *conn, char * name, char **errmsg);

C2S_login_start_packet_t unpack_C2S_login_start_packet(MCbuffer *buff, char **errmsg);

typedef struct {
  MCbuffer * shared_secret;
  MCbuffer * verify_token;
} C2S_login_encryption_response_packet_t;

void send_packet_C2S_login_encryption_response(MConn *conn, MCbuffer * shared_secret, MCbuffer * verify_token, char **errmsg);

C2S_login_encryption_response_packet_t unpack_C2S_login_encryption_response_packet(MCbuffer *buff, char **errmsg);

typedef struct {
  varint_t keep_alive_id;
} S2C_play_keep_alive_packet_t;

void send_packet_S2C_play_keep_alive(MConn *conn, varint_t keep_alive_id, char **errmsg);

S2C_play_keep_alive_packet_t unpack_S2C_play_keep_alive_packet(MCbuffer *buff, char **errmsg);

typedef struct {
  int entity_id;
  byte_t gamemode;
  char dimension;
  byte_t difficulty;
  byte_t max_players;
  char * level_type;
  bool reduced_debug_info;
} S2C_play_join_game_packet_t;

void send_packet_S2C_play_join_game(MConn *conn, int entity_id, byte_t gamemode, char dimension, byte_t difficulty, byte_t max_players, char * level_type, bool reduced_debug_info, char **errmsg);

S2C_play_join_game_packet_t unpack_S2C_play_join_game_packet(MCbuffer *buff, char **errmsg);

typedef struct {
  varint_t keep_alive_id;
} C2S_play_keep_alive_packet_t;

void send_packet_C2S_play_keep_alive(MConn *conn, varint_t keep_alive_id, char **errmsg);

C2S_play_keep_alive_packet_t unpack_C2S_play_keep_alive_packet(MCbuffer *buff, char **errmsg);

