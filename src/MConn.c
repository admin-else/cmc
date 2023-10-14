#include "MConn.h"
#include "MCbuffer.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>

MConn *MConn_init(char *ip, uint16_t port, char **errmsg) {
  MConn *conn = malloc(sizeof(MConn));
  conn->state = OFFLINE;
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

  conn->state = HANDSHAKING;

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

int recv_all(int socket, void *buffer, int length) {
  int total_received = 0;

  while (total_received < length) {
    int bytes_received =
        recv(socket, buffer + total_received, length - total_received, 0);

    if (bytes_received < 0) {
      return -1;
    } else if (bytes_received == 0) {
      break;
    } else {
      total_received += bytes_received;
    }
  }

  return total_received;
}

void MConn_send_buffer(MConn *conn, MCbuffer *buff, char **errmsg) {
  if (send_all(conn->sockfd, buff->data, buff->length) == -1)
    *errmsg = "encounter error while sending...";
}

void MConn_send_and_free_buffer(MConn *conn, MCbuffer *buff, char **errmsg) {
  MConn_send_buffer(conn, buff, errmsg);
  MCbuffer_free(buff);
}

MCbuffer *MConn_recive_packet(MConn *conn, char **errmsg) {
  MCbuffer *buff = MCbuffer_init();
  uint32_t packet_len_unsiged = 0;
  for (int i = 0; i < 5; i++) {
    uint8_t b;
    if (recv(conn->sockfd, &b, 1, 0) == -1) {
      *errmsg = "coudlnt recive byte...";
      return buff;
    }

    packet_len_unsiged |= (b & 0x7F) << (7 * i);
    if (!(b & 0x80))
      break;
  }

  int32_t packet_len = (int32_t)packet_len_unsiged;
  if (0 > packet_len) {
    *errmsg = "Packet len less than zero???";
    return buff;
  }

  buff->data = malloc(packet_len);
  recv_all(conn->sockfd, buff->data, packet_len);
  buff->capacity = packet_len;
  buff->length = packet_len;
  return buff;
}