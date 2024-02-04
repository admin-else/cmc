#include "err.h"
#include "heap_utils.h"
#include "mcbuffer.h"
#include "mctypes.h"
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

MConn *MConn_init() {
  MConn *conn = MALLOC(sizeof(MConn));

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(25565); // Replace 8080 with your server's port
  inet_pton(AF_INET, "127.0.0.1", &(server_addr.sin_addr));

  conn->addr = server_addr;
  conn->name = "Bot";
  conn->state = CONN_STATE_OFFLINE;
  conn->compression_threshold = -1;
  conn->sockfd = -1;
  memset(&conn->on_packet, 0, sizeof(conn->on_packet));
  return conn;
}

void MConn_free(MConn *conn) {
  assert(conn->state == CONN_STATE_OFFLINE);
  FREE(conn->shared_secret);
  FREE(conn);
}

void MConn_close(MConn *conn) {
#define ERR_ACTION return;
  if (conn->state == CONN_STATE_OFFLINE)
    return;
  ERR_IF_NOT_ZERO(close(conn->sockfd), ERR_CLOSING);
  conn->state = CONN_STATE_OFFLINE;
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

MCbuffer *MConn_recive_packet(MConn *conn) {
#define ERR_ACTION return NULL;
  int32_t packet_len = 0;
  for (int i = 0; i < 5; i++) {
    uint8_t b;
    ERR_IF(recv(conn->sockfd, &b, 1, 0) != 1, ERR_RECV);
    packet_len |= (b & 0x7F) << (7 * i);
    if (!(b & 0x80))
      break;
  }

  ERR_IF_LESS_OR_EQ_TO_ZERO(packet_len, ERR_INVALID_PACKET_LEN);

  MCbuffer *buff = MCbuffer_init_w_size(packet_len);
#define ERR_ACTION goto on_err1;
  ERR_IF(recv_all(conn->sockfd, buff->data, packet_len) == -1, ERR_RECV);

  if (conn->compression_threshold < 0)
    return buff;

  int decompressed_length = MCbuffer_unpack_varint(buff);
  if (decompressed_length <= 0)
    return buff;

  unsigned char *decompressed_data = MALLOC(decompressed_length);
#define ERR_ACTION goto on_err2;

  z_stream strm;
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;
  int ret;

  strm.avail_in = buff->length - buff->position;
  strm.next_in = (Bytef *)buff->data + buff->position;
  strm.avail_out = decompressed_length;
  strm.next_out = (Bytef *)decompressed_data;

  ERR_IF(inflateInit(&strm) != Z_OK, ERR_ZLIB_INIT);
#define ERR_ACTION goto on_err3;

  ERR_IF(inflate(&strm, Z_FINISH) != Z_STREAM_END, ERR_ZLIB_INIT);

  size_t real_decompressed_length = strm.total_out;

  inflateEnd(&strm);
#define ERR_ACTION goto on_err2;
  ERR_IF(real_decompressed_length != decompressed_length, ERR_SENDER_LYING);

  MCbuffer_free(buff);

  MCbuffer *decompressed_buff = MCbuffer_init();
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
  MCbuffer_free(buff);
  return NULL;
}

inline void print_bytes_py(unsigned char *bytes, size_t len) {
  printf("b'");
  for (int i = 0; i < len; i++) {
    printf("\\x%02X", bytes[i]);
  }
  printf("'\n");
}

void MConn_send_packet(MConn *conn, MCbuffer *buff) {
#define ERR_ACTION goto on_error;
  MCbuffer *compressed_buffer = MCbuffer_init();
  if (conn->compression_threshold >= 0) {
    if (buff->length >= conn->compression_threshold) {
      MCbuffer_pack_varint(compressed_buffer, buff->length);
      uLong compressedSize = compressBound(buff->length);
      Bytef *compressedData = (Bytef *)malloc(compressedSize);
      ERR_IF(compress(compressedData, &compressedSize, buff->data,
                      buff->length) != Z_OK,
             ERR_ZLIB_COMPRESS);
    } else {
      MCbuffer_pack_varint(compressed_buffer, 0);
      MCbuffer_pack(compressed_buffer, buff->data, buff->length);
    }
    MCbuffer_free(buff);
  } else {
    MCbuffer_pack(compressed_buffer, buff->data, buff->length);
    MCbuffer_free(buff);
  }

  MCbuffer *tmp_buff = MCbuffer_init();
  MCbuffer_pack_varint(tmp_buff, compressed_buffer->length);

  compressed_buffer = MCbuffer_combine(tmp_buff, compressed_buffer);
#define ERR_ACTION ;
  ERR_IF_NOT_ZERO(send_all(conn->sockfd, compressed_buffer->data,
                           compressed_buffer->length),
                  ERR_SENDING);
  MCbuffer_free(compressed_buffer);
  return;

on_error:
  MCbuffer_free(compressed_buffer);
  return;
}

void MConn_loop(MConn *conn) {
#define ERR_ACTION return;
  conn->sockfd = socket(AF_INET, SOCK_STREAM, 0);
  ERR_IF_NOT(conn->sockfd, ERR_SOCKET);
#define ERR_ACTION goto err_close_conn;

  ERR_IF_LESS_THAN_ZERO(
      connect(conn->sockfd, (struct sockaddr *)&conn->addr, sizeof(conn->addr)),
      ERR_CONNETING);
  ERR_ABLE(send_packet_C2S_handshake_handshake(conn, 47, "cmc", 25565,
                                               CONN_STATE_LOGIN));
  ERR_ABLE(send_packet_C2S_login_start(conn, conn->name));
  conn->state = CONN_STATE_LOGIN;
  while (conn->state != CONN_STATE_OFFLINE) {
    MCbuffer *packet = ERR_ABLE(MConn_recive_packet(conn));
#define ERR_ACTION goto err_free_packet;
    int packet_id = ERR_ABLE(MCbuffer_unpack_varint(packet));
    switch (conn->state) {
    case CONN_STATE_LOGIN: {
      switch (packet_id) {
      case packetid_S2C_login_disconnect:
        ERR(ERR_KICKED_WHILE_LOGIN);
        break;
      case packetid_S2C_login_encryption_request:
        ERR(ERR_SERVER_ONLINE_MODE);
        break;
      case packetid_S2C_login_set_compression: {
        S2C_login_set_compression_packet_t compression_packet =
            unpack_S2C_login_set_compression_packet(packet);
        conn->compression_threshold = compression_packet.threshold;
        break;
      }
      case packetid_S2C_login_success:
        conn->state = CONN_STATE_PLAY;
        break;
      default:
        ERR(ERR_INVALID_PACKET_ID_WHILE_LOGGING_IN);
        break;
      }
      break;
    }
    case CONN_STATE_PLAY:
      switch (packet_id) {
#define PACKER_HANDLER_HELPER(packet_name)                                     \
  case packetid_S2C_play_##packet_name: {                                      \
    if (!conn->on_packet.packet_name)                                          \
      break;                                                                   \
    S2C_play_##packet_name##_packet_t data =                                   \
        unpack_S2C_play_##packet_name##_packet(packet);                        \
    if (cmc_err.type) {                                                        \
      printf("ERR_CHECKED %s:%d\n", __FILE__, __LINE__);                       \
      goto err_free_packet;                                                    \
    }                                                                          \
    conn->on_packet.packet_name(data);                                         \
    break;                                                                     \
  }

        // CGSS: loop_handler
        PACKER_HANDLER_HELPER(keep_alive);
        PACKER_HANDLER_HELPER(join_game);
        PACKER_HANDLER_HELPER(chat_message);
        PACKER_HANDLER_HELPER(time_update);
        PACKER_HANDLER_HELPER(entity_equipment);
        PACKER_HANDLER_HELPER(spawn_position);
        PACKER_HANDLER_HELPER(update_health);
        PACKER_HANDLER_HELPER(respawn);
        PACKER_HANDLER_HELPER(player_look_and_position);
        PACKER_HANDLER_HELPER(held_item_change);
        PACKER_HANDLER_HELPER(use_bed);
        PACKER_HANDLER_HELPER(animation);
        PACKER_HANDLER_HELPER(spawn_player);
        PACKER_HANDLER_HELPER(collect_item);
        PACKER_HANDLER_HELPER(spawn_mob);
        PACKER_HANDLER_HELPER(spawn_painting);
        PACKER_HANDLER_HELPER(spawn_experience_orb);
        PACKER_HANDLER_HELPER(entity_velocity);
        PACKER_HANDLER_HELPER(entity);
        PACKER_HANDLER_HELPER(entity_relative_move);
        PACKER_HANDLER_HELPER(entity_look);
        PACKER_HANDLER_HELPER(entity_look_and_relative_move);
        PACKER_HANDLER_HELPER(entity_teleport);
        PACKER_HANDLER_HELPER(entity_head_look);
        PACKER_HANDLER_HELPER(entity_status);
        PACKER_HANDLER_HELPER(attach_entity);
        PACKER_HANDLER_HELPER(entity_metadata);
        PACKER_HANDLER_HELPER(entity_effect);
        PACKER_HANDLER_HELPER(remove_entity_effect);
        PACKER_HANDLER_HELPER(set_experience);
        PACKER_HANDLER_HELPER(chunk_data);
        PACKER_HANDLER_HELPER(block_change);
        PACKER_HANDLER_HELPER(block_action);
        PACKER_HANDLER_HELPER(block_break_animation);
        PACKER_HANDLER_HELPER(effect);
        PACKER_HANDLER_HELPER(sound_effect);
        PACKER_HANDLER_HELPER(change_game_state);
        PACKER_HANDLER_HELPER(player_abilities);
        PACKER_HANDLER_HELPER(plugin_message);
        PACKER_HANDLER_HELPER(disconnect);
        PACKER_HANDLER_HELPER(change_difficulty);
        // CGSE: loop_handler
      }
      break;
    err_free_packet:
      MCbuffer_free(packet);
      goto err_close_conn;
    default:
      break;
    }
  }
#define ERR_ACTION return;
err_close_conn:
  MConn_close(conn);
}