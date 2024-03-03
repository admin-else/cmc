#pragma once

#include "buffer.h"
#include "packet_types.h"
#include <netinet/in.h>
#include <stddef.h>
#include <stdint.h>

typedef enum {
  CMC_CONN_STATE_OFFLINE,
  CMC_CONN_STATE_STATUS,
  CMC_CONN_STATE_LOGIN,
  CMC_CONN_STATE_PLAY,
  CMC_CONN_STATE_HANDSHAKE
} cmc_conn_state;

typedef enum { CMC_DIRECTION_S2C, CMC_DIRECTION_C2S } packet_direction;

struct cmc_conn;

struct cmc_custom_heap_functions {
  void *(*malloc)(size_t n);
  void *(*remalloc)(void *ptr, size_t n);
  void (*free)(void *ptr);
};

struct cmc_conn {
  int sockfd;
  struct sockaddr_in addr;
  cmc_conn_state state; // SEE cmc_conn_STATE_ macros
  ssize_t compression_threshold;
  char *name;
  int protocol_version;
};

typedef struct cmc_conn cmc_conn;

struct cmc_conn cmc_conn_init(int protocol_version);

cmc_buffer *cmc_conn_recive_packet(struct cmc_conn *conn);

void cmc_conn_send_buffer(struct cmc_conn *conn, cmc_buffer *buff);

void cmc_conn_send_and_free_buffer(struct cmc_conn *conn, cmc_buffer *buff);

void cmc_conn_send_packet(struct cmc_conn *conn, cmc_buffer *buff);

cmc_buffer *cmc_conn_recive_packet(struct cmc_conn *conn);

void cmc_conn_close(struct cmc_conn *conn);

void cmc_conn_login(struct cmc_conn *conn);