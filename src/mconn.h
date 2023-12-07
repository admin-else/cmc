#pragma once

#include "mcbuffer.h"
#include "mctypes.h"
#include "packet_types.h"
#include <stddef.h>
#include <stdint.h>

typedef struct {
  int sockfd;
  char *addr;
  uint16_t port;
  MConn_state state; // SEE MCONN_STATE_ macros
  varint_t compression_threshold;
  byte_t *shared_secret;
  struct {
    // CGSS: on_packet_fuction_pointers
    void (*keep_alive)(const S2C_play_keep_alive_packet_t packet);
    // CGSE: on_packet_fuction_pointers
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