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
  ERR_ABLE(send_packet_C2S_handshake(conn, 47, "cmc", 25565, CONN_STATE_LOGIN));
  ERR_ABLE(send_packet_C2S_login_start(conn, conn->name));
  conn->state = CONN_STATE_LOGIN;
  while (conn->state != CONN_STATE_OFFLINE) {
    MCbuffer *packet = ERR_ABLE(MConn_recive_packet(conn));
#define ERR_ACTION goto err_free_packet;
    int packet_id = ERR_ABLE(MCbuffer_unpack_varint(packet));
    printf("packet id %i\n", packet_id);
    switch (conn->state) {
    case CONN_STATE_LOGIN: {
      switch (packet_id) {
      case PACKETID_S2C_LOGIN_DISCONNECT:
        ERR(ERR_KICKED_WHILE_LOGIN);
      case PACKETID_S2C_LOGIN_ENCRYPTION_REQUEST:
        ERR(ERR_SERVER_ONLINE_MODE);
        break;
      case PACKETID_S2C_LOGIN_SET_COMPRESSION: {
        S2C_login_set_compression_packet_t compression_packet =
            unpack_S2C_login_set_compression_packet(packet);
        conn->compression_threshold = compression_packet.threshold;
        break;
      }
      case PACKETID_S2C_LOGIN_SUCCESS:
        conn->state = CONN_STATE_PLAY;
        break;
      default:
        ERR(ERR_INVALID_PACKET_ID_WHILE_LOGGING_IN);
        break;
      }
    }
    case CONN_STATE_PLAY:
      switch (packet_id) {
        // CGSS: loop_handler
      case PACKETID_S2C_PLAY_KEEP_ALIVE: {
        S2C_play_keep_alive_packet_t data =
            unpack_S2C_play_keep_alive_packet(packet);
        ERR_CHECK;
        conn->on_packet.keep_alive(data);
      }
      case PACKETID_S2C_PLAY_JOIN_GAME: {
        S2C_play_join_game_packet_t data =
            unpack_S2C_play_join_game_packet(packet);
        ERR_CHECK;
        conn->on_packet.join_game(data);
      }
      case PACKETID_S2C_PLAY_CHAT_MESSAGE: {
        S2C_play_chat_message_packet_t data =
            unpack_S2C_play_chat_message_packet(packet);
        ERR_CHECK;
        conn->on_packet.chat_message(data);
      }
      case PACKETID_S2C_PLAY_TIME_UPDATE: {
        S2C_play_time_update_packet_t data =
            unpack_S2C_play_time_update_packet(packet);
        ERR_CHECK;
        conn->on_packet.time_update(data);
      }
      case PACKETID_S2C_PLAY_ENTITY_EQUIPMENT: {
        S2C_play_entity_equipment_packet_t data =
            unpack_S2C_play_entity_equipment_packet(packet);
        ERR_CHECK;
        conn->on_packet.entity_equipment(data);
      }
      case PACKETID_S2C_PLAY_SPAWN_POSITION: {
        S2C_play_spawn_position_packet_t data =
            unpack_S2C_play_spawn_position_packet(packet);
        ERR_CHECK;
        conn->on_packet.spawn_position(data);
      }
      case PACKETID_S2C_PLAY_UPDATE_HEALTH: {
        S2C_play_update_health_packet_t data =
            unpack_S2C_play_update_health_packet(packet);
        ERR_CHECK;
        conn->on_packet.update_health(data);
      }
      case PACKETID_S2C_PLAY_RESPAWN: {
        S2C_play_respawn_packet_t data = unpack_S2C_play_respawn_packet(packet);
        ERR_CHECK;
        conn->on_packet.respawn(data);
      }
      case PACKETID_S2C_PLAY_PLAYER_LOOK_AND_POSITION: {
        S2C_play_player_look_and_position_packet_t data =
            unpack_S2C_play_player_look_and_position_packet(packet);
        ERR_CHECK;
        conn->on_packet.player_look_and_position(data);
      }
      case PACKETID_S2C_PLAY_HELD_ITEM_CHANGE: {
        S2C_play_held_item_change_packet_t data =
            unpack_S2C_play_held_item_change_packet(packet);
        ERR_CHECK;
        conn->on_packet.held_item_change(data);
      }
      case PACKETID_S2C_PLAY_USE_BED: {
        S2C_play_use_bed_packet_t data = unpack_S2C_play_use_bed_packet(packet);
        ERR_CHECK;
        conn->on_packet.use_bed(data);
      }
      case PACKETID_S2C_PLAY_ANIMATION: {
        S2C_play_animation_packet_t data =
            unpack_S2C_play_animation_packet(packet);
        ERR_CHECK;
        conn->on_packet.animation(data);
      }
      case PACKETID_S2C_PLAY_SPAWN_PLAYER: {
        S2C_play_spawn_player_packet_t data =
            unpack_S2C_play_spawn_player_packet(packet);
        ERR_CHECK;
        conn->on_packet.spawn_player(data);
      }
      case PACKETID_S2C_PLAY_COLLECT_ITEM: {
        S2C_play_collect_item_packet_t data =
            unpack_S2C_play_collect_item_packet(packet);
        ERR_CHECK;
        conn->on_packet.collect_item(data);
      }
      case PACKETID_S2C_PLAY_SPAWN_MOB: {
        S2C_play_spawn_mob_packet_t data =
            unpack_S2C_play_spawn_mob_packet(packet);
        ERR_CHECK;
        conn->on_packet.spawn_mob(data);
      }
      case PACKETID_S2C_PLAY_SPAWN_PAINTING: {
        S2C_play_spawn_painting_packet_t data =
            unpack_S2C_play_spawn_painting_packet(packet);
        ERR_CHECK;
        conn->on_packet.spawn_painting(data);
      }
      case PACKETID_S2C_PLAY_SPAWN_EXPERIENCE_ORB: {
        S2C_play_spawn_experience_orb_packet_t data =
            unpack_S2C_play_spawn_experience_orb_packet(packet);
        ERR_CHECK;
        conn->on_packet.spawn_experience_orb(data);
      }
      case PACKETID_S2C_PLAY_ENTITY_VELOCITY: {
        S2C_play_entity_velocity_packet_t data =
            unpack_S2C_play_entity_velocity_packet(packet);
        ERR_CHECK;
        conn->on_packet.entity_velocity(data);
      }
      case PACKETID_S2C_PLAY_ENTITY: {
        S2C_play_entity_packet_t data = unpack_S2C_play_entity_packet(packet);
        ERR_CHECK;
        conn->on_packet.entity(data);
      }
      case PACKETID_S2C_PLAY_ENTITY_RELATIVE_MOVE: {
        S2C_play_entity_relative_move_packet_t data =
            unpack_S2C_play_entity_relative_move_packet(packet);
        ERR_CHECK;
        conn->on_packet.entity_relative_move(data);
      }
      case PACKETID_S2C_PLAY_ENTITY_LOOK: {
        S2C_play_entity_look_packet_t data =
            unpack_S2C_play_entity_look_packet(packet);
        ERR_CHECK;
        conn->on_packet.entity_look(data);
      }
      case PACKETID_S2C_PLAY_ENTITY_LOOK_AND_RELATIVE_MOVE: {
        S2C_play_entity_look_and_relative_move_packet_t data =
            unpack_S2C_play_entity_look_and_relative_move_packet(packet);
        ERR_CHECK;
        conn->on_packet.entity_look_and_relative_move(data);
      }
      case PACKETID_S2C_PLAY_ENTITY_TELEPORT: {
        S2C_play_entity_teleport_packet_t data =
            unpack_S2C_play_entity_teleport_packet(packet);
        ERR_CHECK;
        conn->on_packet.entity_teleport(data);
      }
      case PACKETID_S2C_PLAY_ENTITY_HEAD_LOOK: {
        S2C_play_entity_head_look_packet_t data =
            unpack_S2C_play_entity_head_look_packet(packet);
        ERR_CHECK;
        conn->on_packet.entity_head_look(data);
      }
      case PACKETID_S2C_PLAY_ENTITY_STATUS: {
        S2C_play_entity_status_packet_t data =
            unpack_S2C_play_entity_status_packet(packet);
        ERR_CHECK;
        conn->on_packet.entity_status(data);
      }
      case PACKETID_S2C_PLAY_ATTACH_ENTITY: {
        S2C_play_attach_entity_packet_t data =
            unpack_S2C_play_attach_entity_packet(packet);
        ERR_CHECK;
        conn->on_packet.attach_entity(data);
      }
      case PACKETID_S2C_PLAY_ENTITY_METADATA: {
        S2C_play_entity_metadata_packet_t data =
            unpack_S2C_play_entity_metadata_packet(packet);
        ERR_CHECK;
        conn->on_packet.entity_metadata(data);
      }
      case PACKETID_S2C_PLAY_ENTITY_EFFECT: {
        S2C_play_entity_effect_packet_t data =
            unpack_S2C_play_entity_effect_packet(packet);
        ERR_CHECK;
        conn->on_packet.entity_effect(data);
      }
      case PACKETID_S2C_PLAY_REMOVE_ENTITY_EFFECT: {
        S2C_play_remove_entity_effect_packet_t data =
            unpack_S2C_play_remove_entity_effect_packet(packet);
        ERR_CHECK;
        conn->on_packet.remove_entity_effect(data);
      }
      case PACKETID_S2C_PLAY_SET_EXPERIENCE: {
        S2C_play_set_experience_packet_t data =
            unpack_S2C_play_set_experience_packet(packet);
        ERR_CHECK;
        conn->on_packet.set_experience(data);
      }
      case PACKETID_S2C_PLAY_CHUNK_DATA: {
        S2C_play_chunk_data_packet_t data =
            unpack_S2C_play_chunk_data_packet(packet);
        ERR_CHECK;
        conn->on_packet.chunk_data(data);
      }
      case PACKETID_S2C_PLAY_BLOCK_CHANGE: {
        S2C_play_block_change_packet_t data =
            unpack_S2C_play_block_change_packet(packet);
        ERR_CHECK;
        conn->on_packet.block_change(data);
      }
      case PACKETID_S2C_PLAY_BLOCK_ACTION: {
        S2C_play_block_action_packet_t data =
            unpack_S2C_play_block_action_packet(packet);
        ERR_CHECK;
        conn->on_packet.block_action(data);
      }
      case PACKETID_S2C_PLAY_BLOCK_BREAK_ANIMATION: {
        S2C_play_block_break_animation_packet_t data =
            unpack_S2C_play_block_break_animation_packet(packet);
        ERR_CHECK;
        conn->on_packet.block_break_animation(data);
      }
      case PACKETID_S2C_PLAY_EFFECT: {
        S2C_play_effect_packet_t data = unpack_S2C_play_effect_packet(packet);
        ERR_CHECK;
        conn->on_packet.effect(data);
      }
      case PACKETID_S2C_PLAY_SOUND_EFFECT: {
        S2C_play_sound_effect_packet_t data =
            unpack_S2C_play_sound_effect_packet(packet);
        ERR_CHECK;
        conn->on_packet.sound_effect(data);
      }
      case PACKETID_S2C_PLAY_CHANGE_GAME_STATE: {
        S2C_play_change_game_state_packet_t data =
            unpack_S2C_play_change_game_state_packet(packet);
        ERR_CHECK;
        conn->on_packet.change_game_state(data);
      }
      case PACKETID_S2C_PLAY_PLAYER_ABILITIES: {
        S2C_play_player_abilities_packet_t data =
            unpack_S2C_play_player_abilities_packet(packet);
        ERR_CHECK;
        conn->on_packet.player_abilities(data);
      }
      case PACKETID_S2C_PLAY_PLUGIN_MESSAGE: {
        S2C_play_plugin_message_packet_t data =
            unpack_S2C_play_plugin_message_packet(packet);
        ERR_CHECK;
        conn->on_packet.plugin_message(data);
      }
      case PACKETID_S2C_PLAY_DISCONNECT: {
        S2C_play_disconnect_packet_t data =
            unpack_S2C_play_disconnect_packet(packet);
        ERR_CHECK;
        conn->on_packet.disconnect(data);
      }
      case PACKETID_S2C_PLAY_CHANGE_DIFFICULTY: {
        S2C_play_change_difficulty_packet_t data =
            unpack_S2C_play_change_difficulty_packet(packet);
        ERR_CHECK;
        conn->on_packet.change_difficulty(data);
      }
        // CGSE: loop_handler
      }
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