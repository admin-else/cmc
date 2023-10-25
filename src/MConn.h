#pragma once

#include "MCbuffer.h"
#include "MCtypes.h"
#include <stddef.h>
#include <stdint.h>

typedef struct{
  int sockfd;
  char *addr;
  uint16_t port;
  uint8_t state; // SEE MCONN_STATE_ macros
  varint_t compression_threshold;
  byte_t *shared_secret;
} MConn;

typedef enum { CONN_STATE_OFFLINE, CONN_STATE_STATUS, CONN_STATE_LOGIN, CONN_STATE_PLAY, CONN_STATE_HANDSHAKING } MConn_state;

MConn *MConn_init(char *ip, uint16_t port, char **errmsg);

MCbuffer *MConn_recive_packet(MConn *conn, char **errmsg);

void MConn_send_buffer(MConn *conn, MCbuffer *buff, char **errmsg);

void MConn_send_and_free_buffer(MConn *conn, MCbuffer *buff, char **errmsg);

void MConn_send_packet(MConn *conn, MCbuffer *buff, char **errmsg);

MCbuffer *MConn_recive_packet(MConn *conn, char **errmsg);

void MConn_free(MConn *conn, char **errmsg);

void MConn_close(MConn *conn, char **errmsg);

MConn *MConn_connect(MConn *conn, char **errmsg);