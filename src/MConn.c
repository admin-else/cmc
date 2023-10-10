#include "MConn.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "MCbuffer.h"

MConn *MConn_init(char *ip, uint16_t port, char **errmsg) {
  MConn *conn = malloc(sizeof(MConn));
  conn->state = MCONN_STATE_OFFLINE;
  conn->addr = ip;
  conn->port = port;

  if ((conn->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    free(conn);
    *errmsg = "Socket creation error";
    return NULL;
  }

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(port);

  if (inet_pton(AF_INET, ip, &address.sin_addr) <= 0) {
    free(conn);
    *errmsg = "Invalid address.";
    return NULL;
  }

  if (connect(conn->sockfd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    free(conn);
    *errmsg = "Connection Failed";
    return NULL;
  }

  conn->state = MCONN_STATE_HANDSHAKING;

  return conn;
}

int send_all(int socket, const void *buffer, size_t length) {
  const char *ptr = (const char *)buffer;
  size_t total_sent = 0;

  while (total_sent < length) {
    ssize_t sent = send(socket, ptr + total_sent, length - total_sent, 0);

    if (sent == -1)
      return -1;

    total_sent += sent;
  }

  return 0; // Success
}

void MConn_send_buffer(MConn *conn, MCbuffer *buff, char **errmsg) {
  if (send_all(conn->sockfd, buff->data, buff->length) == -1) *errmsg = "encounter error while sending...";
}