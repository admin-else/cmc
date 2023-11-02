#include "MConn.h"
#include "MCbuffer.h"
#include "MCtypes.h"
#include "heap-utils.h"
#include <arpa/inet.h>
#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <zlib.h>

MConn *MConn_init(char *ip, uint16_t port, char **errmsg) {
  MConn *conn = MALLOC(sizeof(MConn));
  conn->state = CONN_STATE_OFFLINE;
  conn->addr = ip;
  conn->port = port;
  conn->compression_threshold = -1;
  conn->sockfd = -1;
  return conn;
}

MConn *MConn_connect(MConn *conn, char **errmsg) {
  if ((conn->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    FREE(conn);
    *errmsg = "Socket creation error";
    return NULL;
  }

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(conn->port);

  if (inet_pton(AF_INET, conn->addr, &address.sin_addr) <= 0) {
    FREE(conn);
    *errmsg = "Invalid address.";
    return NULL;
  }

  if (connect(conn->sockfd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    FREE(conn);
    *errmsg = "Connection Failed";
    return NULL;
  }

  conn->state = CONN_STATE_HANDSHAKE;

  return conn;
}

void MConn_free(MConn *conn, char **errmsg) {
  assert(conn->state == CONN_STATE_OFFLINE);
  FREE(conn->shared_secret);
  FREE(conn);
}

void MConn_close(MConn *conn, char **errmsg) {
  if (conn->state == CONN_STATE_OFFLINE)
    return;
  if (close(conn->sockfd) != 0)
    *errmsg = "unable to close connection";
  if (*errmsg != NULL)
    return;
  conn->state  = CONN_STATE_OFFLINE;
  conn->sockfd = -1;
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
  ssize_t total_received = 0;

  while (total_received < length) {
    ssize_t bytes_received =
        recv(socket, buffer + total_received, length - total_received, 0);

    if (bytes_received <= 0)
      return -1;
    total_received += bytes_received;
  }

  return 0;
}

MCbuffer *MConn_recive_packet(MConn *conn, char **errmsg) {
  uint32_t packet_len_unsiged = 0;
  for (int i = 0; i < 5; i++) {
    uint8_t b;
    if (recv(conn->sockfd, &b, 1, 0) != 1) {
      *errmsg = "coudlnt recive byte...";
      return NULL;
    }

    packet_len_unsiged |= (b & 0x7F) << (7 * i);
    if (!(b & 0x80))
      break;
  }

  int32_t packet_len = (int32_t)packet_len_unsiged;
  if (0 >= packet_len) {
    *errmsg = "Packet len less or equal to zero???";
    return NULL;
  }

  MCbuffer *buff = MCbuffer_init_w_size(packet_len);
  if (recv_all(conn->sockfd, buff->data, packet_len) == -1) {
    MConn_close(conn, errmsg);
    MConn_free(conn, errmsg);
    MCbuffer_free(buff);
    *errmsg = "recving data failed";
    return NULL;
  }
  buff->capacity = packet_len;
  buff->length = packet_len;
  buff->position = 0;

  if (conn->compression_threshold < 0)
    return buff;

  int decompressed_length = MCbuffer_unpack_varint(buff, errmsg);
  if (decompressed_length <= 0)
    return buff;
  
  byte_t *decompressed_data = MALLOC(decompressed_length);
  size_t real_decompressed_length;
  if (uncompress(decompressed_data, &real_decompressed_length, buff->data,
                 packet_len) != Z_OK) {
    *errmsg = "zlib decompression failed!";
    FREE(decompressed_data);
    MCbuffer_free(buff);
    return NULL;
  }

  if (real_decompressed_length != decompressed_length) {
    *errmsg = "The sender lied about the decompresed lenght of a packet";
    FREE(decompressed_data);
    MCbuffer_free(buff);
    return NULL;
  }

  MCbuffer_free(buff);

  MCbuffer *decompressed_buff = MCbuffer_init();
  decompressed_buff->data = decompressed_data;
  decompressed_buff->capacity = decompressed_length;
  decompressed_buff->length = decompressed_length;

  return decompressed_buff;

  // TODO: Encryption
}

inline void print_bytes_py(unsigned char *bytes, size_t len) {
  printf("b'");
  for (int i = 0; i < len; i++) {
    printf("\\x%02X", bytes[i]);
  }
  printf("'\n");
}

void MConn_send_packet(MConn *conn, MCbuffer *buff, char **errmsg) {
  /*
    if compression_threshold >= 0:
        if len(data) >= compression_threshold:
            data = cls.pack_varint(len(data)) + zlib.compress(data)
        else:
            data = cls.pack_varint(0) + data
    return cls.pack_varint(len(data), max_bits=32) + data
  */

  MCbuffer *compressed_buffer = MCbuffer_init();
  if (conn->compression_threshold >= 0) {
    if (buff->length >= conn->compression_threshold) {
      MCbuffer_pack_varint(compressed_buffer, buff->length, errmsg);
      uLong compressedSize = compressBound(buff->length);
      Bytef *compressedData = (Bytef *)malloc(compressedSize);
      if (compress(compressedData, &compressedSize, buff->data, buff->length) !=
          Z_OK) {
        *errmsg = "zlib failed while unpacking";
        FREE(compressedData);
        MCbuffer_free(compressed_buffer);
        MCbuffer_free(buff);
        return;
      }
    } else {
      MCbuffer_pack_varint(compressed_buffer, 0, errmsg);
      MCbuffer_pack(compressed_buffer, buff->data, buff->length, errmsg);
    }
    MCbuffer_free(buff);
  } else {
    MCbuffer_pack(compressed_buffer, buff->data, buff->length, errmsg);
    MCbuffer_free(buff);
  }

  MCbuffer *tmp_buff = MCbuffer_init();
  MCbuffer_pack_varint(tmp_buff, compressed_buffer->length, errmsg);

  compressed_buffer = MCbuffer_combine(tmp_buff, compressed_buffer, errmsg);

  if (send_all(conn->sockfd, compressed_buffer->data,
               compressed_buffer->length) != 0)
    *errmsg = "error cant send packet for some reason.";
  MCbuffer_free(compressed_buffer);
  return;
}