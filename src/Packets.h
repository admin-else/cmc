//This code is generated.

#pragma once
#include "MCbuffer.h"
#include "MConn.h"
#include "MCtypes.h"
#include <stdbool.h>
#include <jansson.h>

typedef struct {
  varint_t protocole_version;
  char * server_addr;
  unsigned short port;
  varint_t next_state;
} handshake_packet_t;

void send_packet_handshake(MConn *conn, varint_t protocole_version, char * server_addr, unsigned short port, varint_t next_state, char **errmsg);

handshake_packet_t unpack_handshake_packet(MCbuffer *buff, char **errmsg);

void send_packet_status_request(MConn *conn, char **errmsg);

typedef struct {
  json_t * response;
} status_response_packet_t;

void send_packet_status_response(MConn *conn, json_t * response, char **errmsg);

status_response_packet_t unpack_status_response_packet(MCbuffer *buff, char **errmsg);