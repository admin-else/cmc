#pragma once

#include <cmc/buff.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdint.h>

typedef enum {
  CMC_CONN_STATE_OFFLINE,
  CMC_CONN_STATE_STATUS = 1, // this is importent for the next state packet
  CMC_CONN_STATE_LOGIN = 2,
  CMC_CONN_STATE_PLAY,
  CMC_CONN_STATE_HANDSHAKE,
  CMC_CONN_STATE_CONFIG
} cmc_conn_state;

typedef enum { CMC_DIRECTION_S2C, CMC_DIRECTION_C2S } packet_direction;

typedef struct {
  int sockfd;
  struct sockaddr_in addr;
  cmc_conn_state state;
  ssize_t compression_threshold;
  char *name;
  int protocol_version;
  cmc_err_extra err;
} cmc_conn;

cmc_conn cmc_conn_init(int protocol_version);

cmc_err cmc_conn_connect(cmc_conn *conn, struct sockaddr *addr,
                         socklen_t addr_len);

cmc_buff *cmc_conn_recive_packet(cmc_conn *conn);

void cmc_conn_send_buffer(cmc_conn *conn, cmc_buff *buff);

void cmc_conn_send_and_free_buffer(cmc_conn *conn, cmc_buff *buff);

void cmc_conn_send_packet(cmc_conn *conn, cmc_buff *buff);

cmc_err cmc_conn_close(cmc_conn *conn);