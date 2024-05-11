#include <arpa/inet.h>
#include <cmc/buff.h>
#include <cmc/err.h>
#include <cmc/heap_utils.h>
#include <cmc/conn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <zlib.h>
#include "err.h"

cmc_conn cmc_conn_init(int protocol_version) {
  return (cmc_conn){.state = CMC_CONN_STATE_OFFLINE,
                    .compression_threshold = -1,
                    .sockfd = -1,
                    .protocol_version = protocol_version};
}

cmc_err cmc_conn_close(cmc_conn *conn) {
  if (conn->state == CMC_CONN_STATE_OFFLINE)
    return CMC_ERR_NO;
  CMC_ERRRC_IF(close(conn->sockfd), CMC_ERR_CLOSING);
  conn->state = CMC_CONN_STATE_OFFLINE;
  conn->sockfd = -1;
  return CMC_ERR_NO;
}

static int send_all(int socket, const void *buff, size_t length) {
  size_t total_sent = 0;

  while (total_sent < length) {
    ssize_t sent = send(socket, buff + total_sent, length - total_sent, 0);

    if (sent == -1)
      return -1;

    total_sent += sent;
  }

  return 0; // Success
}

static int recv_all(int socket, void *buff, size_t length) {
  size_t total_received = 0;

  while (total_received < length) {
    ssize_t bytes_received = // sorry janfel ): dereferencing ‘void *’ pointer
        recv(socket, buff + total_received, length - total_received, 0);

    if (bytes_received <= 0)
      return -1;
    total_received += bytes_received;
  }

  return 0;
}

cmc_buff *cmc_conn_recive_packet(cmc_conn *conn) {
  int32_t packet_len = 0;
  for (int i = 0; i < 5; i++) {
    uint8_t b;
    CMC_ERRC_IF(recv(conn->sockfd, &b, 1, 0) != 1, CMC_ERR_RECV, return NULL;);
    packet_len |= (b & 0x7F) << (7 * i);
    if (!(b & 0x80))
      break;
  }

  CMC_ERRC_IF(packet_len <= 0, CMC_ERR_INVALID_PACKET_LEN,
                            return NULL;);

  cmc_buff *buff = cmc_buff_init(conn->protocol_version);
  cmc_buff_pack(buff, NULL, packet_len);
  CMC_ERRC_IF(recv_all(conn->sockfd, buff->data, packet_len) == -1, CMC_ERR_RECV,
         goto on_err1;);

  if (conn->compression_threshold == -1)
    return buff;

  int decompressed_length_signed = cmc_buff_unpack_varint(buff);
  if (decompressed_length_signed <= 0)
    return buff;

  size_t decompressed_length = decompressed_length_signed;

  unsigned char *decompressed_data = CMC_ERRC_ABLE(cmc_malloc(decompressed_length, &conn->err), goto on_err1);

  z_stream strm;
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;

  strm.avail_in = buff->length - buff->position;
  strm.next_in = (Bytef *)buff->data + buff->position;
  strm.avail_out = decompressed_length;
  strm.next_out = (Bytef *)decompressed_data;

  CMC_ERRC_IF(inflateInit(&strm) != Z_OK, CMC_ERR_ZLIB_INIT, goto on_err2;);

  CMC_ERRC_IF(inflate(&strm, Z_FINISH) != Z_STREAM_END, CMC_ERR_ZLIB_INFLATE,
         goto on_err3;);

  size_t real_decompressed_length = strm.total_out;

  inflateEnd(&strm);
  CMC_ERRC_IF(real_decompressed_length != decompressed_length, CMC_ERR_SENDER_LYING,
         goto on_err2;);

  cmc_buff_free(buff);

  cmc_buff *decompressed_buff = cmc_buff_init(conn->protocol_version);
  decompressed_buff->data = decompressed_data;
  decompressed_buff->capacity = decompressed_length;
  decompressed_buff->length = decompressed_length;

  return decompressed_buff;

  // TODO: Encryption
on_err3:
  inflateEnd(&strm);
on_err2:
  free(decompressed_data);
on_err1:
  cmc_buff_free(buff);
  return NULL;
}

void cmc_conn_send_packet(cmc_conn *conn, cmc_buff *buff) {
  cmc_buff *compressed_buff = cmc_buff_init(conn->protocol_version);
  if (conn->compression_threshold >= 0) {
    if (buff->length >= (size_t)conn->compression_threshold) {
      cmc_buff_pack_varint(compressed_buff, buff->length);
      uLong compressedSize = compressBound(buff->length);
      Bytef *compressedData = CMC_ERRC_ABLE(cmc_malloc(compressedSize, &buff->err), goto on_error);
      CMC_ERRC_IF(compress(compressedData, &compressedSize, buff->data,
                      buff->length) != Z_OK,
             CMC_ERR_ZLIB_COMPRESS, goto on_error;);
    } else {
      cmc_buff_pack_varint(compressed_buff, 0);
      cmc_buff_pack(compressed_buff, buff->data, buff->length);
    }
  } else {
    cmc_buff_pack(compressed_buff, buff->data, buff->length);
  }

  cmc_buff *packet_size_buff = cmc_buff_init(conn->protocol_version);
  cmc_buff_pack_varint(packet_size_buff, compressed_buff->length);

  compressed_buff = cmc_buff_combine(packet_size_buff, compressed_buff);
  CMC_ERRC_IF(send_all(conn->sockfd, compressed_buff->data,
                           compressed_buff->length) != 0,
                  CMC_ERR_SENDING, );
  cmc_buff_free(compressed_buff);
  return;

on_error:
  cmc_buff_free(compressed_buff);
  return;
}