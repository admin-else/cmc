#ifndef MCONN_H
#define MCONN_H

#include "MCbuffer.h"
#include <stddef.h>
#include <stdint.h>

typedef struct{
  int sockfd;
  char *addr;
  uint16_t port;
  uint8_t state; // SEE MCONN_STATE_ macros
} MConn;

#define MCONN_STATE_HANDSHAKING 0
#define MCONN_STATE_STATUS 1
#define MCONN_STATE_LOGIN 2
#define MCONN_STATE_PLAY 3
#define MCONN_STATE_OFFLINE 4

typedef enum { HANDSHAKING, STATUS, LOGIN, PLAY, OFFLINE } MConn_state;

void MConn_send_buffer(MConn *conn, MCbuffer *buff, char **errmsg);
MConn *MConn_init(char *ip, uint16_t port, char **errmsg);

#endif