#include "mconn.h"
#include "err.h"
#include "heap_utils.h"
#include "mcbuffer.h"
#include "mctypes.h"
#include "packet_types.h"
#include "packets.h"
#include <arpa/inet.h>
#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <zlib.h>

MConn *MConn_init() {
  MConn *conn = MALLOC(sizeof(MConn));
  conn->state = CONN_STATE_OFFLINE;
  struct sockaddr_in localhost = (struct sockaddr_in){.sin_family = AF_INET, .sin_port = htons(25565), .sin_addr = inet_addr("127.0.0.1")};
  memcpy(&conn->addr, &localhost, sizeof(localhost));
  conn->compression_threshold = -1;
  conn->sockfd = -1;
  memset(&conn->on_packet, 0, sizeof(conn->on_packet));
  return conn;
}

void MConn_login_and_packet_loop(MConn *conn, char **errmsg) {
  if ((conn->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    *errmsg = "Socket creation error";
    return;
  }

  if (connect(conn->sockfd, (struct sockaddr *)&conn->addr,
              sizeof(conn->addr)) < 0) {
    *errmsg = "Connection Failed";
    return;
  }

  conn->state = CONN_STATE_HANDSHAKE;
  // maybe soon changed to act more like a vanninla client
  send_packet_C2S_handshake(conn, 47, "cmc", 25565, CONN_STATE_LOGIN, errmsg);
  send_packet_C2S_login_start(conn, "cmc", errmsg);
  conn->state = CONN_STATE_LOGIN;

  while (conn->state != CONN_STATE_OFFLINE) {
    MCbuffer *buff = MConn_recive_packet(conn, errmsg);
    varint_t packet_id = MCbuffer_unpack_varint(buff, errmsg);
    switch (conn->state) {
    case CONN_STATE_LOGIN:
      switch (packet_id) {
      case PACKETID_S2C_LOGIN_DISCONNECT:
        conn->state = CONN_STATE_OFFLINE;
        break;
      case PACKETID_S2C_LOGIN_ENCRYPTION_REQUEST:
        // Online mode is not implemented ):
        conn->state = CONN_STATE_OFFLINE;
        break;
      case PACKETID_S2C_LOGIN_SET_COMPRESSION:
        conn->compression_threshold =
            unpack_S2C_login_set_compression_packet(buff, errmsg).threshold;
        break;
      case PACKETID_S2C_LOGIN_SUCCESS:
        // TODO: implement player entity data
        conn->state = CONN_STATE_PLAY;
        break;
      }
      break;
    case CONN_STATE_PLAY:
      break;
    default: // HANDSHAKE OFFLINE AND STATUS
      break;
    }
    MCbuffer_free(buff);
  }

  return;
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

  unsigned char *decompressed_data = MALLOC(decompressed_length);

  z_stream strm;
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;
  int ret;

  strm.avail_in = buff->length - buff->position;
  strm.next_in = (Bytef *)buff->data + buff->position;
  strm.avail_out = decompressed_length;
  strm.next_out = (Bytef *)decompressed_data;

  ret = inflateInit(&strm);
  if (ret != Z_OK) {
    sprintf(*errmsg, "Decompression initialization error: %s\n", strm.msg);
    MCbuffer_free(buff);
    return NULL;
  }

  ret = inflate(&strm, Z_FINISH);
  if (ret != Z_STREAM_END) {
    sprintf(*errmsg, "Decompression error: %s\n", strm.msg);
    MCbuffer_free(buff);
    inflateEnd(&strm);
    return NULL;
  }

  size_t real_decompressed_length = strm.total_out;

  inflateEnd(&strm);

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

void MConn_loop(MConn *conn, char **errmsg) {
  conn->sockfd = socket(AF_INET, SOCK_STREAM, 0);
  ERR_IF_NOT(conn->sockfd, "failed to create socket");
  ERR_IF_NOT(connect(conn->sockfd, &conn->addr, sizeof(conn->addr)),
             "unable to connect to server");
  ERR_ABLE(send_packet_C2S_handshake(conn, 47, "cmc", 25565, CONN_STATE_LOGIN,
                                     errmsg));
  ERR_ABLE(send_packet_C2S_login_start(conn, conn->name, errmsg));
  conn->state = CONN_STATE_LOGIN;
  while (conn->state != CONN_STATE_OFFLINE) {
    MCbuffer *packet = ERR_ABLE(MConn_recive_packet(conn, errmsg));
    int packet_id = ERR_ABLE(MCbuffer_unpack_varint(packet, errmsg));
    switch (conn->state) {
    case CONN_STATE_LOGIN: {
      switch (packet_id) {
      case PACKETID_S2C_LOGIN_DISCONNECT:
        conn->state = CONN_STATE_OFFLINE;
        *errmsg = "kicked while logging in";
        break;
      case PACKETID_S2C_LOGIN_ENCRYPTION_REQUEST:
        conn->state = CONN_STATE_OFFLINE;
        *errmsg = "server requested online mode this programm does not support "
                  "online mode";
        break;
      case PACKETID_S2C_LOGIN_SET_COMPRESSION: {
        S2C_login_set_compression_packet_t compression_packet =
            unpack_S2C_login_set_compression_packet(packet, errmsg);
        conn->compression_threshold = compression_packet.threshold;
        break;
      }
      case PACKETID_S2C_LOGIN_SUCCESS:
        conn->state = CONN_STATE_PLAY;
        break;
      default:
        conn->state = CONN_STATE_OFFLINE;
        *errmsg = "unregognized packet id in login state.";
        break;
      }
    }
    case CONN_STATE_PLAY:
      switch (packet_id) {
        // CGSS: loop_handler
      case PACKETID_S2C_PLAY_KEEP_ALIVE: {
        S2C_play_keep_alive_packet_t data =
            unpack_S2C_play_keep_alive_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.keep_alive(data);
      }
      case PACKETID_S2C_PLAY_JOIN_GAME: {
        S2C_play_join_game_packet_t data =
            unpack_S2C_play_join_game_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.join_game(data);
      }
      case PACKETID_S2C_PLAY_CHAT_MESSAGE: {
        S2C_play_chat_message_packet_t data =
            unpack_S2C_play_chat_message_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.chat_message(data);
      }
      case PACKETID_S2C_PLAY_TIME_UPDATE: {
        S2C_play_time_update_packet_t data =
            unpack_S2C_play_time_update_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.time_update(data);
      }
      case PACKETID_S2C_PLAY_ENTITY_EQUIPMENT: {
        S2C_play_entity_equipment_packet_t data =
            unpack_S2C_play_entity_equipment_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.entity_equipment(data);
      }
      case PACKETID_S2C_PLAY_SPAWN_POSITION: {
        S2C_play_spawn_position_packet_t data =
            unpack_S2C_play_spawn_position_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.spawn_position(data);
      }
      case PACKETID_S2C_PLAY_UPDATE_HEALTH: {
        S2C_play_update_health_packet_t data =
            unpack_S2C_play_update_health_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.update_health(data);
      }
      case PACKETID_S2C_PLAY_RESPAWN: {
        S2C_play_respawn_packet_t data =
            unpack_S2C_play_respawn_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.respawn(data);
      }
      case PACKETID_S2C_PLAY_PLAYER_LOOK_AND_POSITION: {
        S2C_play_player_look_and_position_packet_t data =
            unpack_S2C_play_player_look_and_position_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.player_look_and_position(data);
      }
      case PACKETID_S2C_PLAY_HELD_ITEM_CHANGE: {
        S2C_play_held_item_change_packet_t data =
            unpack_S2C_play_held_item_change_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.held_item_change(data);
      }
      case PACKETID_S2C_PLAY_USE_BED: {
        S2C_play_use_bed_packet_t data =
            unpack_S2C_play_use_bed_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.use_bed(data);
      }
      case PACKETID_S2C_PLAY_ANIMATION: {
        S2C_play_animation_packet_t data =
            unpack_S2C_play_animation_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.animation(data);
      }
      case PACKETID_S2C_PLAY_SPAWN_PLAYER: {
        S2C_play_spawn_player_packet_t data =
            unpack_S2C_play_spawn_player_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.spawn_player(data);
      }
      case PACKETID_S2C_PLAY_COLLECT_ITEM: {
        S2C_play_collect_item_packet_t data =
            unpack_S2C_play_collect_item_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.collect_item(data);
      }
      case PACKETID_S2C_PLAY_SPAWN_MOB: {
        S2C_play_spawn_mob_packet_t data =
            unpack_S2C_play_spawn_mob_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.spawn_mob(data);
      }
      case PACKETID_S2C_PLAY_SPAWN_PAINTING: {
        S2C_play_spawn_painting_packet_t data =
            unpack_S2C_play_spawn_painting_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.spawn_painting(data);
      }
      case PACKETID_S2C_PLAY_SPAWN_EXPERIENCE_ORB: {
        S2C_play_spawn_experience_orb_packet_t data =
            unpack_S2C_play_spawn_experience_orb_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.spawn_experience_orb(data);
      }
      case PACKETID_S2C_PLAY_ENTITY_VELOCITY: {
        S2C_play_entity_velocity_packet_t data =
            unpack_S2C_play_entity_velocity_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.entity_velocity(data);
      }
      case PACKETID_S2C_PLAY_ENTITY: {
        S2C_play_entity_packet_t data =
            unpack_S2C_play_entity_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.entity(data);
      }
      case PACKETID_S2C_PLAY_ENTITY_RELATIVE_MOVE: {
        S2C_play_entity_relative_move_packet_t data =
            unpack_S2C_play_entity_relative_move_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.entity_relative_move(data);
      }
      case PACKETID_S2C_PLAY_ENTITY_LOOK: {
        S2C_play_entity_look_packet_t data =
            unpack_S2C_play_entity_look_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.entity_look(data);
      }
      case PACKETID_S2C_PLAY_ENTITY_LOOK_AND_RELATIVE_MOVE: {
        S2C_play_entity_look_and_relative_move_packet_t data =
            unpack_S2C_play_entity_look_and_relative_move_packet(packet,
                                                                 errmsg);
        if (*errmsg)
          break;
        conn->on_packet.entity_look_and_relative_move(data);
      }
      case PACKETID_S2C_PLAY_ENTITY_TELEPORT: {
        S2C_play_entity_teleport_packet_t data =
            unpack_S2C_play_entity_teleport_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.entity_teleport(data);
      }
      case PACKETID_S2C_PLAY_ENTITY_HEAD_LOOK: {
        S2C_play_entity_head_look_packet_t data =
            unpack_S2C_play_entity_head_look_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.entity_head_look(data);
      }
      case PACKETID_S2C_PLAY_ENTITY_STATUS: {
        S2C_play_entity_status_packet_t data =
            unpack_S2C_play_entity_status_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.entity_status(data);
      }
      case PACKETID_S2C_PLAY_ATTACH_ENTITY: {
        S2C_play_attach_entity_packet_t data =
            unpack_S2C_play_attach_entity_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.attach_entity(data);
      }
      case PACKETID_S2C_PLAY_ENTITY_METADATA: {
        S2C_play_entity_metadata_packet_t data =
            unpack_S2C_play_entity_metadata_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.entity_metadata(data);
      }
      case PACKETID_S2C_PLAY_ENTITY_EFFECT: {
        S2C_play_entity_effect_packet_t data =
            unpack_S2C_play_entity_effect_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.entity_effect(data);
      }
      case PACKETID_S2C_PLAY_REMOVE_ENTITY_EFFECT: {
        S2C_play_remove_entity_effect_packet_t data =
            unpack_S2C_play_remove_entity_effect_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.remove_entity_effect(data);
      }
      case PACKETID_S2C_PLAY_SET_EXPERIENCE: {
        S2C_play_set_experience_packet_t data =
            unpack_S2C_play_set_experience_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.set_experience(data);
      }
      case PACKETID_S2C_PLAY_CHUNK_DATA: {
        S2C_play_chunk_data_packet_t data =
            unpack_S2C_play_chunk_data_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.chunk_data(data);
      }
      case PACKETID_S2C_PLAY_BLOCK_CHANGE: {
        S2C_play_block_change_packet_t data =
            unpack_S2C_play_block_change_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.block_change(data);
      }
      case PACKETID_S2C_PLAY_BLOCK_ACTION: {
        S2C_play_block_action_packet_t data =
            unpack_S2C_play_block_action_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.block_action(data);
      }
      case PACKETID_S2C_PLAY_BLOCK_BREAK_ANIMATION: {
        S2C_play_block_break_animation_packet_t data =
            unpack_S2C_play_block_break_animation_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.block_break_animation(data);
      }
      case PACKETID_S2C_PLAY_EFFECT: {
        S2C_play_effect_packet_t data =
            unpack_S2C_play_effect_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.effect(data);
      }
      case PACKETID_S2C_PLAY_SOUND_EFFECT: {
        S2C_play_sound_effect_packet_t data =
            unpack_S2C_play_sound_effect_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.sound_effect(data);
      }
      case PACKETID_S2C_PLAY_CHANGE_GAME_STATE: {
        S2C_play_change_game_state_packet_t data =
            unpack_S2C_play_change_game_state_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.change_game_state(data);
      }
      case PACKETID_S2C_PLAY_PLAYER_ABILITIES: {
        S2C_play_player_abilities_packet_t data =
            unpack_S2C_play_player_abilities_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.player_abilities(data);
      }
      case PACKETID_S2C_PLAY_PLUGIN_MESSAGE: {
        S2C_play_plugin_message_packet_t data =
            unpack_S2C_play_plugin_message_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.plugin_message(data);
      }
      case PACKETID_S2C_PLAY_DISCONNECT: {
        S2C_play_disconnect_packet_t data =
            unpack_S2C_play_disconnect_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.disconnect(data);
      }
      case PACKETID_S2C_PLAY_CHANGE_DIFFICULTY: {
        S2C_play_change_difficulty_packet_t data =
            unpack_S2C_play_change_difficulty_packet(packet, errmsg);
        if (*errmsg)
          break;
        conn->on_packet.change_difficulty(data);
      }
        // CGSE: loop_handler
      }
    default:
      break;
    }
  }

  ERR_IF_NOT(close(conn->sockfd), "unable to close connection");
  return;
}