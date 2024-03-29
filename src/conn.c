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

struct cmc_conn cmc_conn_init(int protocol_version) {
  struct cmc_conn conn;
  struct sockaddr_in localhost;
  localhost.sin_family = AF_INET;
  localhost.sin_port = htons(25565);
  inet_pton(AF_INET, "127.0.0.1", &(localhost.sin_addr));

  conn.addr = localhost;
  conn.name = "Bot";
  conn.state = CMC_CONN_STATE_OFFLINE;
  conn.compression_threshold = -1;
  conn.sockfd = -1;
  conn.protocol_version = protocol_version;
  return conn;
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

  cmc_buffer *buff = cmc_buffer_init(conn->protocol_version);
  cmc_buffer_pack(buff, NULL, packet_len);
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

  cmc_buffer *decompressed_buff = cmc_buffer_init(conn->protocol_version);
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
  cmc_buffer *compressed_buffer = cmc_buffer_init(conn->protocol_version);
  if (conn->compression_threshold >= 0) {
    if (buff->length >= (size_t)conn->compression_threshold) {
      cmc_buffer_pack_varint(compressed_buffer, buff->length);
      uLong compressedSize = compressBound(buff->length);
      Bytef *compressedData = (Bytef *)MALLOC(compressedSize);
      ERR_IF(compress(compressedData, &compressedSize, buff->data,
                      buff->length) != Z_OK,
             ERR_ZLIB_COMPRESS, goto on_error;);
    } else {
      cmc_buffer_pack_varint(compressed_buffer, 0);
      cmc_buffer_pack(compressed_buffer, buff->data, buff->length);
    }
  } else {
    cmc_buffer_pack(compressed_buffer, buff->data, buff->length);
  }

  cmc_buffer *tmp_buff = cmc_buffer_init(conn->protocol_version);
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

void cmc_conn_login(struct cmc_conn *conn) {
  conn->sockfd = socket(AF_INET, SOCK_STREAM, 0);
  ERR_IF_NOT(conn->sockfd, ERR_SOCKET, return;);

  ERR_IF_LESS_THAN_ZERO(
      connect(conn->sockfd, (struct sockaddr *)&conn->addr, sizeof(conn->addr)),
      ERR_CONNETING, return;);
  conn->state = CMC_CONN_STATE_HANDSHAKE;
  C2S_handshake_handshake_packet handshake = {
      .server_addr = "cmc",
      .next_state = CMC_CONN_STATE_LOGIN,
      .protocole_version = conn->protocol_version,
      .server_port = 25565};
  C2S_login_start_packet login_start = {
      .name = conn->name,
      .uuid = (cmc_uuid){.upper = 826775, .lower = 489269249}};
  ERR_ABLE(cmc_send_C2S_handshake_handshake_packet(conn, &handshake),
           goto close_conn;);
  ERR_ABLE(cmc_send_C2S_login_start_packet(conn, &login_start),
           goto close_conn;);
  conn->state = CMC_CONN_STATE_LOGIN;
  while (conn->state == CMC_CONN_STATE_LOGIN) {
    cmc_buffer *raw_packet = ERR_ABLE(cmc_conn_recive_packet(conn), break;);
    int packet_id = ERR_ABLE(cmc_buffer_unpack_varint(raw_packet), break;);
    int pnid = cmc_packet_id_to_packet_name_id(
        packet_id, conn->state, CMC_DIRECTION_S2C, conn->protocol_version);
    switch (pnid) {
    case CMC_S2C_LOGIN_SET_COMPRESSION_NAME_ID: {
      S2C_login_set_compression_packet compression =
          unpack_S2C_login_set_compression_packet(raw_packet);
      conn->compression_threshold = compression.threshold;
      cmc_free_S2C_login_set_compression_packet(&compression);
      break;
    }
    case CMC_S2C_LOGIN_SUCCESS_NAME_ID:
      conn->state = CMC_CONN_STATE_PLAY;
      break;
    case CMC_S2C_LOGIN_DISCONNECT_NAME_ID: {
      S2C_login_disconnect_packet disconnect =
          unpack_S2C_login_disconnect_packet(raw_packet);
      if (!cmc_err) {
        printf("kicked while logging in: %s\n", disconnect.reason);
        cmc_free_S2C_login_disconnect_packet(&disconnect);
      }
      ERR(ERR_KICKED_WHILE_LOGIN, conn->state = CMC_CONN_STATE_OFFLINE;);
      break;
    }
    case CMC_S2C_LOGIN_ENCRYPTION_REQUEST_NAME_ID:
      ERR(ERR_SERVER_ONLINE_MODE, conn->state = CMC_CONN_STATE_OFFLINE;);
      break;
    case CMC_UNKOWN_NAME_ID:
      ERR(ERR_UNKOWN_PACKET, conn->state = CMC_CONN_STATE_OFFLINE;);
      break;
    default:
      ERR(ERR_UNEXPECTED_PACKET, conn->state = CMC_CONN_STATE_OFFLINE;);
    }
    cmc_buffer_free(raw_packet);
  }
  if (conn->state != CMC_CONN_STATE_PLAY) {
  close_conn:
    cmc_conn_close(conn);
  }
}