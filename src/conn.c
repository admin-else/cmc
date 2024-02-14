#include "buffer.h"
#include "err.h"
#include "heap_utils.h"
#include "packet_types.h"
#include "packets.h"
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <zlib.h>

struct cmc_conn *cmc_conn_init() {
  struct cmc_conn *conn =
      malloc(sizeof(*conn)); // TODO: when you guve a libary a custom allocator
                             // it supposed to always use it i will have to make
                             // it be that way but right now this will stay

  struct sockaddr_in localhost;
  localhost.sin_family = AF_INET;
  localhost.sin_port = htons(25565);
  inet_pton(AF_INET, "127.0.0.1", &(localhost.sin_addr));

  conn->custom_heap.free = free;
  conn->custom_heap.malloc = malloc;
  conn->custom_heap.realloc = realloc;

  conn->addr = localhost;
  conn->name = "Bot";
  conn->state = CMC_CONN_STATE_OFFLINE;
  conn->compression_threshold = -1;
  conn->sockfd = -1;
  conn->shared_secret = NULL;
  conn->on_packet = NULL;
  return conn;
}

void cmc_conn_free(struct cmc_conn *conn) {
  assert(conn->state == CMC_CONN_STATE_OFFLINE);
  if (conn->shared_secret)
    FREE(conn->shared_secret);
  FREE(conn);
}

void cmc_conn_close(struct cmc_conn *conn) {
  if (conn->state == CMC_CONN_STATE_OFFLINE)
    return;
  ERR_IF_NOT_ZERO(close(conn->sockfd), ERR_CLOSING, return;);
  conn->state = CMC_CONN_STATE_OFFLINE;
  conn->sockfd = -1;
}

int send_all(int socket, const void *buffer, size_t length) {
  size_t total_sent = 0;

  while (total_sent < length) {
    ssize_t sent = send(socket, buffer + total_sent, length - total_sent, 0);

    if (sent == -1)
      return -1;

    total_sent += sent;
  }

  return 0; // Success
}

int recv_all(int socket, void *buffer, size_t length) {
  size_t total_received = 0;

  while (total_received < length) {
    ssize_t bytes_received = // sorry janfel ): dereferencing ‘void *’ pointer
        recv(socket, buffer + total_received, length - total_received, 0);

    if (bytes_received <= 0)
      return -1;
    total_received += bytes_received;
  }

  return 0;
}

cmc_buffer *cmc_conn_recive_packet(struct cmc_conn *conn) {
  int32_t packet_len = 0;
  for (int i = 0; i < 5; i++) {
    uint8_t b;
    ERR_IF(recv(conn->sockfd, &b, 1, 0) != 1, ERR_RECV, return NULL;);
    packet_len |= (b & 0x7F) << (7 * i);
    if (!(b & 0x80))
      break;
  }

  ERR_IF_LESS_OR_EQ_TO_ZERO(packet_len, ERR_INVALID_PACKET_LEN, return NULL;);

  cmc_buffer *buff = cmc_buffer_init_w_size(packet_len);
  ERR_IF(recv_all(conn->sockfd, buff->data, packet_len) == -1, ERR_RECV,
         goto on_err1;);

  if (conn->compression_threshold == -1)
    return buff;

  int decompressed_length_signed = cmc_buffer_unpack_varint(buff);
  if (decompressed_length_signed <= 0)
    return buff;

  size_t decompressed_length = decompressed_length_signed;

  unsigned char *decompressed_data = MALLOC(decompressed_length);

  z_stream strm;
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;

  strm.avail_in = buff->length - buff->position;
  strm.next_in = (Bytef *)buff->data + buff->position;
  strm.avail_out = decompressed_length;
  strm.next_out = (Bytef *)decompressed_data;

  ERR_IF(inflateInit(&strm) != Z_OK, ERR_ZLIB_INIT, goto on_err2;);

  ERR_IF(inflate(&strm, Z_FINISH) != Z_STREAM_END, ERR_ZLIB_INFLATE,
         goto on_err3;);

  size_t real_decompressed_length = strm.total_out;

  inflateEnd(&strm);
  ERR_IF(real_decompressed_length != decompressed_length, ERR_SENDER_LYING,
         goto on_err2;);

  cmc_buffer_free(buff);

  cmc_buffer *decompressed_buff = cmc_buffer_init();
  decompressed_buff->data = decompressed_data;
  decompressed_buff->capacity = decompressed_length;
  decompressed_buff->length = decompressed_length;

  return decompressed_buff;

  // TODO: Encryption
on_err3:
  inflateEnd(&strm);
on_err2:
  FREE(decompressed_data);
on_err1:
  cmc_buffer_free(buff);
  return NULL;
}

inline void print_bytes_py(unsigned char *bytes, size_t len) {
  printf("b'");
  for (size_t i = 0; i < len; i++) {
    printf("\\x%02X", bytes[i]);
  }
  printf("'\n");
}

void cmc_conn_send_packet(struct cmc_conn *conn, cmc_buffer *buff) {
  cmc_buffer *compressed_buffer = cmc_buffer_init();
  if (conn->compression_threshold >= 0) {
    if (buff->length >= (size_t)conn->compression_threshold) {
      cmc_buffer_pack_varint(compressed_buffer, buff->length);
      uLong compressedSize = compressBound(buff->length);
      Bytef *compressedData = (Bytef *)malloc(compressedSize);
      ERR_IF(compress(compressedData, &compressedSize, buff->data,
                      buff->length) != Z_OK,
             ERR_ZLIB_COMPRESS, goto on_error;);
    } else {
      cmc_buffer_pack_varint(compressed_buffer, 0);
      cmc_buffer_pack(compressed_buffer, buff->data, buff->length);
    }
    cmc_buffer_free(buff);
  } else {
    cmc_buffer_pack(compressed_buffer, buff->data, buff->length);
    cmc_buffer_free(buff);
  }

  cmc_buffer *tmp_buff = cmc_buffer_init();
  cmc_buffer_pack_varint(tmp_buff, compressed_buffer->length);

  compressed_buffer = cmc_buffer_combine(tmp_buff, compressed_buffer);
  ERR_IF_NOT_ZERO(send_all(conn->sockfd, compressed_buffer->data,
                           compressed_buffer->length),
                  ERR_SENDING, );
  cmc_buffer_free(compressed_buffer);
  return;

on_error:
  cmc_buffer_free(compressed_buffer);
  return;
}

void cmc_conn_loop(struct cmc_conn *conn) {
  conn->sockfd = socket(AF_INET, SOCK_STREAM, 0);
  ERR_IF_NOT(conn->sockfd, ERR_SOCKET, return;);

  ERR_IF_LESS_THAN_ZERO(
      connect(conn->sockfd, (struct sockaddr *)&conn->addr, sizeof(conn->addr)),
      ERR_CONNETING, goto err_close_conn;);
  ERR_ABLE(send_packet_C2S_handshake_handshake(conn, 47, "cmc", 25565,
                                               CMC_CONN_STATE_LOGIN),
           goto err_close_conn;);
  ERR_ABLE(send_packet_C2S_login_start(conn, conn->name), goto err_close_conn;);
  conn->state = CMC_CONN_STATE_LOGIN;
  while (conn->state != CMC_CONN_STATE_OFFLINE) {
    cmc_buffer *packet =
        ERR_ABLE(cmc_conn_recive_packet(conn), goto err_close_conn;);

    int packet_id =
        ERR_ABLE(cmc_buffer_unpack_varint(packet), goto err_free_packet;);
    switch (conn->state) {
    case CMC_CONN_STATE_LOGIN: {
      switch (packet_id) {
      case CMC_PACKETID_S2C_LOGIN_DISCONNECT:
        ERR(ERR_KICKED_WHILE_LOGIN, goto err_free_packet;);
        break;
      case CMC_PACKETID_S2C_LOGIN_ENCRYPTION_REQUEST:
        ERR(ERR_SERVER_ONLINE_MODE, goto err_free_packet;);
        break;
      case CMC_PACKETID_S2C_LOGIN_SET_COMPRESSION: {
        S2C_login_set_compression_packet compression_data =
            unpack_S2C_login_set_compression_packet(packet);
        conn->compression_threshold = compression_data.threshold;
        break;
      }
      case CMC_PACKETID_S2C_LOGIN_SUCCESS:
        conn->state = CMC_CONN_STATE_PLAY;
        break;
      default:
        ERR(ERR_INVALID_PACKET_ID_WHILE_LOGGING_IN, goto err_free_packet;);
        break;
      }
      break;
    }
    case CMC_CONN_STATE_PLAY:
      if (conn->on_packet)
        conn->on_packet(packet, packet_id, conn);
      cmc_buffer_free(packet);
      break;
    err_free_packet:
      cmc_buffer_free(packet);
      goto err_close_conn;
    default:
      break;
    }
  }
err_close_conn:
  cmc_conn_close(conn);
}