#include "buffer.h"
#include "err.h"
#include "heap_utils.h"
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

struct cmc_conn *cmc_conn_init() {
  struct cmc_conn *conn = MALLOC(sizeof(struct cmc_conn));

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(25565); // Replace 8080 with your server's port
  inet_pton(AF_INET, "127.0.0.1", &(server_addr.sin_addr));

  conn->addr = server_addr;
  conn->name = "Bot";
  conn->state = CONN_STATE_OFFLINE;
  conn->compression_threshold = -1;
  conn->sockfd = -1;
  conn->shared_secret = NULL;
  memset(&conn->on_packet, 0, sizeof(conn->on_packet));
  return conn;
}

void cmc_conn_free(struct cmc_conn *conn) {
  assert(conn->state == CONN_STATE_OFFLINE);
  if (conn->shared_secret)
    FREE(conn->shared_secret);
  FREE(conn);
}

void cmc_conn_close(struct cmc_conn *conn) {
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

cmc_buffer *cmc_conn_recive_packet(struct cmc_conn *conn) {
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

  cmc_buffer *buff = cmc_buffer_init_w_size(packet_len);
#define ERR_ACTION goto on_err1;
  ERR_IF(recv_all(conn->sockfd, buff->data, packet_len) == -1, ERR_RECV);

  if (conn->compression_threshold < 0)
    return buff;

  int decompressed_length = cmc_buffer_unpack_varint(buff);
  if (decompressed_length <= 0)
    return buff;

  unsigned char *decompressed_data = MALLOC(decompressed_length);
#define ERR_ACTION goto on_err2;

  z_stream strm;
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;

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
  for (int i = 0; i < len; i++) {
    printf("\\x%02X", bytes[i]);
  }
  printf("'\n");
}

void cmc_conn_send_packet(struct cmc_conn *conn, cmc_buffer *buff) {
#define ERR_ACTION goto on_error;
  cmc_buffer *compressed_buffer = cmc_buffer_init();
  if (conn->compression_threshold >= 0) {
    if (buff->length >= conn->compression_threshold) {
      cmc_buffer_pack_varint(compressed_buffer, buff->length);
      uLong compressedSize = compressBound(buff->length);
      Bytef *compressedData = (Bytef *)malloc(compressedSize);
      ERR_IF(compress(compressedData, &compressedSize, buff->data,
                      buff->length) != Z_OK,
             ERR_ZLIB_COMPRESS);
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
#define ERR_ACTION ;
  ERR_IF_NOT_ZERO(send_all(conn->sockfd, compressed_buffer->data,
                           compressed_buffer->length),
                  ERR_SENDING);
  cmc_buffer_free(compressed_buffer);
  return;

on_error:
  cmc_buffer_free(compressed_buffer);
  return;
}

void cmc_conn_loop(struct cmc_conn *conn) {
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
    cmc_buffer *packet = ERR_ABLE(cmc_conn_recive_packet(conn));

#define ERR_ACTION goto err_free_packet;
    int packet_id = ERR_ABLE(cmc_buffer_unpack_varint(packet));
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
        // CGSS: loop_handler
      case packetid_S2C_play_keep_alive: {
        if (!conn->on_packet.keep_alive)
          goto unhandeled_packet;
        S2C_play_keep_alive_packet_t data =
            ERR_ABLE(unpack_S2C_play_keep_alive_packet(packet));
        conn->on_packet.keep_alive(data, conn);

        break;
      }
      case packetid_S2C_play_join_game: {
        if (!conn->on_packet.join_game)
          goto unhandeled_packet;
        S2C_play_join_game_packet_t data =
            ERR_ABLE(unpack_S2C_play_join_game_packet(packet));
        conn->on_packet.join_game(data, conn);
        free_S2C_play_join_game_packet(data);
        break;
      }
      case packetid_S2C_play_chat_message: {
        if (!conn->on_packet.chat_message)
          goto unhandeled_packet;
        S2C_play_chat_message_packet_t data =
            ERR_ABLE(unpack_S2C_play_chat_message_packet(packet));
        conn->on_packet.chat_message(data, conn);
        free_S2C_play_chat_message_packet(data);
        break;
      }
      case packetid_S2C_play_time_update: {
        if (!conn->on_packet.time_update)
          goto unhandeled_packet;
        S2C_play_time_update_packet_t data =
            ERR_ABLE(unpack_S2C_play_time_update_packet(packet));
        conn->on_packet.time_update(data, conn);

        break;
      }
      case packetid_S2C_play_entity_equipment: {
        if (!conn->on_packet.entity_equipment)
          goto unhandeled_packet;
        S2C_play_entity_equipment_packet_t data =
            ERR_ABLE(unpack_S2C_play_entity_equipment_packet(packet));
        conn->on_packet.entity_equipment(data, conn);
        free_S2C_play_entity_equipment_packet(data);
        break;
      }
      case packetid_S2C_play_spawn_position: {
        if (!conn->on_packet.spawn_position)
          goto unhandeled_packet;
        S2C_play_spawn_position_packet_t data =
            ERR_ABLE(unpack_S2C_play_spawn_position_packet(packet));
        conn->on_packet.spawn_position(data, conn);

        break;
      }
      case packetid_S2C_play_update_health: {
        if (!conn->on_packet.update_health)
          goto unhandeled_packet;
        S2C_play_update_health_packet_t data =
            ERR_ABLE(unpack_S2C_play_update_health_packet(packet));
        conn->on_packet.update_health(data, conn);

        break;
      }
      case packetid_S2C_play_respawn: {
        if (!conn->on_packet.respawn)
          goto unhandeled_packet;
        S2C_play_respawn_packet_t data =
            ERR_ABLE(unpack_S2C_play_respawn_packet(packet));
        conn->on_packet.respawn(data, conn);
        free_S2C_play_respawn_packet(data);
        break;
      }
      case packetid_S2C_play_player_look_and_position: {
        if (!conn->on_packet.player_look_and_position)
          goto unhandeled_packet;
        S2C_play_player_look_and_position_packet_t data =
            ERR_ABLE(unpack_S2C_play_player_look_and_position_packet(packet));
        conn->on_packet.player_look_and_position(data, conn);

        break;
      }
      case packetid_S2C_play_held_item_change: {
        if (!conn->on_packet.held_item_change)
          goto unhandeled_packet;
        S2C_play_held_item_change_packet_t data =
            ERR_ABLE(unpack_S2C_play_held_item_change_packet(packet));
        conn->on_packet.held_item_change(data, conn);

        break;
      }
      case packetid_S2C_play_use_bed: {
        if (!conn->on_packet.use_bed)
          goto unhandeled_packet;
        S2C_play_use_bed_packet_t data =
            ERR_ABLE(unpack_S2C_play_use_bed_packet(packet));
        conn->on_packet.use_bed(data, conn);

        break;
      }
      case packetid_S2C_play_animation: {
        if (!conn->on_packet.animation)
          goto unhandeled_packet;
        S2C_play_animation_packet_t data =
            ERR_ABLE(unpack_S2C_play_animation_packet(packet));
        conn->on_packet.animation(data, conn);

        break;
      }
      case packetid_S2C_play_spawn_player: {
        if (!conn->on_packet.spawn_player)
          goto unhandeled_packet;
        S2C_play_spawn_player_packet_t data =
            ERR_ABLE(unpack_S2C_play_spawn_player_packet(packet));
        conn->on_packet.spawn_player(data, conn);
        free_S2C_play_spawn_player_packet(data);
        break;
      }
      case packetid_S2C_play_collect_item: {
        if (!conn->on_packet.collect_item)
          goto unhandeled_packet;
        S2C_play_collect_item_packet_t data =
            ERR_ABLE(unpack_S2C_play_collect_item_packet(packet));
        conn->on_packet.collect_item(data, conn);

        break;
      }
      case packetid_S2C_play_spawn_mob: {
        if (!conn->on_packet.spawn_mob)
          goto unhandeled_packet;
        S2C_play_spawn_mob_packet_t data =
            ERR_ABLE(unpack_S2C_play_spawn_mob_packet(packet));
        conn->on_packet.spawn_mob(data, conn);
        free_S2C_play_spawn_mob_packet(data);
        break;
      }
      case packetid_S2C_play_spawn_painting: {
        if (!conn->on_packet.spawn_painting)
          goto unhandeled_packet;
        S2C_play_spawn_painting_packet_t data =
            ERR_ABLE(unpack_S2C_play_spawn_painting_packet(packet));
        conn->on_packet.spawn_painting(data, conn);
        free_S2C_play_spawn_painting_packet(data);
        break;
      }
      case packetid_S2C_play_spawn_experience_orb: {
        if (!conn->on_packet.spawn_experience_orb)
          goto unhandeled_packet;
        S2C_play_spawn_experience_orb_packet_t data =
            ERR_ABLE(unpack_S2C_play_spawn_experience_orb_packet(packet));
        conn->on_packet.spawn_experience_orb(data, conn);

        break;
      }
      case packetid_S2C_play_entity_velocity: {
        if (!conn->on_packet.entity_velocity)
          goto unhandeled_packet;
        S2C_play_entity_velocity_packet_t data =
            ERR_ABLE(unpack_S2C_play_entity_velocity_packet(packet));
        conn->on_packet.entity_velocity(data, conn);

        break;
      }
      case packetid_S2C_play_entity: {
        if (!conn->on_packet.entity)
          goto unhandeled_packet;
        S2C_play_entity_packet_t data =
            ERR_ABLE(unpack_S2C_play_entity_packet(packet));
        conn->on_packet.entity(data, conn);

        break;
      }
      case packetid_S2C_play_entity_relative_move: {
        if (!conn->on_packet.entity_relative_move)
          goto unhandeled_packet;
        S2C_play_entity_relative_move_packet_t data =
            ERR_ABLE(unpack_S2C_play_entity_relative_move_packet(packet));
        conn->on_packet.entity_relative_move(data, conn);

        break;
      }
      case packetid_S2C_play_entity_look: {
        if (!conn->on_packet.entity_look)
          goto unhandeled_packet;
        S2C_play_entity_look_packet_t data =
            ERR_ABLE(unpack_S2C_play_entity_look_packet(packet));
        conn->on_packet.entity_look(data, conn);

        break;
      }
      case packetid_S2C_play_entity_look_and_relative_move: {
        if (!conn->on_packet.entity_look_and_relative_move)
          goto unhandeled_packet;
        S2C_play_entity_look_and_relative_move_packet_t data = ERR_ABLE(
            unpack_S2C_play_entity_look_and_relative_move_packet(packet));
        conn->on_packet.entity_look_and_relative_move(data, conn);

        break;
      }
      case packetid_S2C_play_entity_teleport: {
        if (!conn->on_packet.entity_teleport)
          goto unhandeled_packet;
        S2C_play_entity_teleport_packet_t data =
            ERR_ABLE(unpack_S2C_play_entity_teleport_packet(packet));
        conn->on_packet.entity_teleport(data, conn);

        break;
      }
      case packetid_S2C_play_entity_head_look: {
        if (!conn->on_packet.entity_head_look)
          goto unhandeled_packet;
        S2C_play_entity_head_look_packet_t data =
            ERR_ABLE(unpack_S2C_play_entity_head_look_packet(packet));
        conn->on_packet.entity_head_look(data, conn);

        break;
      }
      case packetid_S2C_play_entity_status: {
        if (!conn->on_packet.entity_status)
          goto unhandeled_packet;
        S2C_play_entity_status_packet_t data =
            ERR_ABLE(unpack_S2C_play_entity_status_packet(packet));
        conn->on_packet.entity_status(data, conn);

        break;
      }
      case packetid_S2C_play_attach_entity: {
        if (!conn->on_packet.attach_entity)
          goto unhandeled_packet;
        S2C_play_attach_entity_packet_t data =
            ERR_ABLE(unpack_S2C_play_attach_entity_packet(packet));
        conn->on_packet.attach_entity(data, conn);

        break;
      }
      case packetid_S2C_play_entity_metadata: {
        if (!conn->on_packet.entity_metadata)
          goto unhandeled_packet;
        S2C_play_entity_metadata_packet_t data =
            ERR_ABLE(unpack_S2C_play_entity_metadata_packet(packet));
        conn->on_packet.entity_metadata(data, conn);
        free_S2C_play_entity_metadata_packet(data);
        break;
      }
      case packetid_S2C_play_entity_effect: {
        if (!conn->on_packet.entity_effect)
          goto unhandeled_packet;
        S2C_play_entity_effect_packet_t data =
            ERR_ABLE(unpack_S2C_play_entity_effect_packet(packet));
        conn->on_packet.entity_effect(data, conn);

        break;
      }
      case packetid_S2C_play_remove_entity_effect: {
        if (!conn->on_packet.remove_entity_effect)
          goto unhandeled_packet;
        S2C_play_remove_entity_effect_packet_t data =
            ERR_ABLE(unpack_S2C_play_remove_entity_effect_packet(packet));
        conn->on_packet.remove_entity_effect(data, conn);

        break;
      }
      case packetid_S2C_play_set_experience: {
        if (!conn->on_packet.set_experience)
          goto unhandeled_packet;
        S2C_play_set_experience_packet_t data =
            ERR_ABLE(unpack_S2C_play_set_experience_packet(packet));
        conn->on_packet.set_experience(data, conn);

        break;
      }
      case packetid_S2C_play_chunk_data: {
        if (!conn->on_packet.chunk_data)
          goto unhandeled_packet;
        S2C_play_chunk_data_packet_t data =
            ERR_ABLE(unpack_S2C_play_chunk_data_packet(packet));
        conn->on_packet.chunk_data(data, conn);
        free_S2C_play_chunk_data_packet(data);
        break;
      }
      case packetid_S2C_play_block_change: {
        if (!conn->on_packet.block_change)
          goto unhandeled_packet;
        S2C_play_block_change_packet_t data =
            ERR_ABLE(unpack_S2C_play_block_change_packet(packet));
        conn->on_packet.block_change(data, conn);

        break;
      }
      case packetid_S2C_play_block_action: {
        if (!conn->on_packet.block_action)
          goto unhandeled_packet;
        S2C_play_block_action_packet_t data =
            ERR_ABLE(unpack_S2C_play_block_action_packet(packet));
        conn->on_packet.block_action(data, conn);

        break;
      }
      case packetid_S2C_play_block_break_animation: {
        if (!conn->on_packet.block_break_animation)
          goto unhandeled_packet;
        S2C_play_block_break_animation_packet_t data =
            ERR_ABLE(unpack_S2C_play_block_break_animation_packet(packet));
        conn->on_packet.block_break_animation(data, conn);

        break;
      }
      case packetid_S2C_play_effect: {
        if (!conn->on_packet.effect)
          goto unhandeled_packet;
        S2C_play_effect_packet_t data =
            ERR_ABLE(unpack_S2C_play_effect_packet(packet));
        conn->on_packet.effect(data, conn);

        break;
      }
      case packetid_S2C_play_sound_effect: {
        if (!conn->on_packet.sound_effect)
          goto unhandeled_packet;
        S2C_play_sound_effect_packet_t data =
            ERR_ABLE(unpack_S2C_play_sound_effect_packet(packet));
        conn->on_packet.sound_effect(data, conn);
        free_S2C_play_sound_effect_packet(data);
        break;
      }
      case packetid_S2C_play_change_game_state: {
        if (!conn->on_packet.change_game_state)
          goto unhandeled_packet;
        S2C_play_change_game_state_packet_t data =
            ERR_ABLE(unpack_S2C_play_change_game_state_packet(packet));
        conn->on_packet.change_game_state(data, conn);

        break;
      }
      case packetid_S2C_play_player_abilities: {
        if (!conn->on_packet.player_abilities)
          goto unhandeled_packet;
        S2C_play_player_abilities_packet_t data =
            ERR_ABLE(unpack_S2C_play_player_abilities_packet(packet));
        conn->on_packet.player_abilities(data, conn);

        break;
      }
      case packetid_S2C_play_plugin_message: {
        if (!conn->on_packet.plugin_message)
          goto unhandeled_packet;
        S2C_play_plugin_message_packet_t data =
            ERR_ABLE(unpack_S2C_play_plugin_message_packet(packet));
        conn->on_packet.plugin_message(data, conn);
        free_S2C_play_plugin_message_packet(data);
        break;
      }
      case packetid_S2C_play_disconnect: {
        if (!conn->on_packet.disconnect)
          goto unhandeled_packet;
        S2C_play_disconnect_packet_t data =
            ERR_ABLE(unpack_S2C_play_disconnect_packet(packet));
        conn->on_packet.disconnect(data, conn);
        free_S2C_play_disconnect_packet(data);
        break;
      }
      case packetid_S2C_play_change_difficulty: {
        if (!conn->on_packet.change_difficulty)
          goto unhandeled_packet;
        S2C_play_change_difficulty_packet_t data =
            ERR_ABLE(unpack_S2C_play_change_difficulty_packet(packet));
        conn->on_packet.change_difficulty(data, conn);

        break;
      }
        // CGSE: loop_handler
      unhandeled_packet:
        if (conn->on_unhandeld_packet)
          conn->on_unhandeld_packet(packet, packet_id, conn);
        break;
      }
      cmc_buffer_free(packet);
      break;
    err_free_packet:
      cmc_buffer_free(packet);
      goto err_close_conn;
    default:
      break;
    }
  }
#define ERR_ACTION return;
err_close_conn:
  cmc_conn_close(conn);
}