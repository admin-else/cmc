#pragma once

#include "MCbuffer.h"
#include "MCtypes.h"
#include <stddef.h>
#include <stdint.h>

typedef struct{
  int sockfd;
  char *addr;
  uint16_t port;
  MConn_state state; // SEE MCONN_STATE_ macros
  varint_t compression_threshold;
  byte_t *shared_secret;
  struct {
    void (*chat_message)(int);
  } on_packet;
  struct {
    int eid;
    float health;
  } player;
} MConn;

MConn *MConn_init(char *ip, uint16_t port, char **errmsg);

MCbuffer *MConn_recive_packet(MConn *conn, char **errmsg);

void MConn_send_buffer(MConn *conn, MCbuffer *buff, char **errmsg);

void MConn_send_and_free_buffer(MConn *conn, MCbuffer *buff, char **errmsg);

void MConn_send_packet(MConn *conn, MCbuffer *buff, char **errmsg);

MCbuffer *MConn_recive_packet(MConn *conn, char **errmsg);

void MConn_free(MConn *conn, char **errmsg);

void MConn_close(MConn *conn, char **errmsg);

MConn *MConn_connect(MConn *conn, char **errmsg);