#include "packets.h"
#include "buffer.h"
#include "conn.h"
#include "err.h"
#include "heap_utils.h"
#include "packet_types.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNPACK_ERR_HANDELER                                                    \
  ERR_CHECK(return packet;);                                                   \
  if (buff->position != buff->length)                                          \
    ERR(ERR_BUFFER_UNDERUN, return packet;);

char *packet_id_to_string(int packet_id, cmc_conn_state state,
                          packet_direction direction) {
#define PACKET_ID(packet_id, state, direction)                                 \
  (packet_id & 0x00FFFFFF) | (state << 24) | (direction << 23)

  int combined_packet_data =
      (packet_id & 0x00FFFFFF) | (state << 24) | (direction << 23);

  switch (combined_packet_data) {
    // CGSS: packet_id_to_string
  case PACKET_ID(0x00, CMC_CONN_STATE_HANDSHAKE, CMC_DIRECTION_C2S):
    return "C2S_handshake_handshake";
  case PACKET_ID(0x00, CMC_CONN_STATE_STATUS, CMC_DIRECTION_S2C):
    return "S2C_status_response";
  case PACKET_ID(0x01, CMC_CONN_STATE_STATUS, CMC_DIRECTION_S2C):
    return "S2C_status_pong";
  case PACKET_ID(0x00, CMC_CONN_STATE_STATUS, CMC_DIRECTION_C2S):
    return "C2S_status_request";
  case PACKET_ID(0x01, CMC_CONN_STATE_STATUS, CMC_DIRECTION_C2S):
    return "C2S_status_ping";
  case PACKET_ID(0x00, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_S2C):
    return "S2C_login_disconnect";
  case PACKET_ID(0x01, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_S2C):
    return "S2C_login_encryption_request";
  case PACKET_ID(0x02, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_S2C):
    return "S2C_login_success";
  case PACKET_ID(0x03, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_S2C):
    return "S2C_login_set_compression";
  case PACKET_ID(0x00, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_C2S):
    return "C2S_login_start";
  case PACKET_ID(0x01, CMC_CONN_STATE_LOGIN, CMC_DIRECTION_C2S):
    return "C2S_login_encryption_response";
  case PACKET_ID(0x00, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_keep_alive";
  case PACKET_ID(0x01, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_join_game";
  case PACKET_ID(0x02, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_chat_message";
  case PACKET_ID(0x03, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_time_update";
  case PACKET_ID(0x04, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_entity_equipment";
  case PACKET_ID(0x05, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_spawn_position";
  case PACKET_ID(0x06, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_update_health";
  case PACKET_ID(0x07, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_respawn";
  case PACKET_ID(0x08, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_player_look_and_position";
  case PACKET_ID(0x09, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_held_item_change";
  case PACKET_ID(0x0A, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_use_bed";
  case PACKET_ID(0x0B, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_animation";
  case PACKET_ID(0x0C, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_spawn_player";
  case PACKET_ID(0x0D, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_collect_item";
  case PACKET_ID(0x0F, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_spawn_mob";
  case PACKET_ID(0x10, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_spawn_painting";
  case PACKET_ID(0x11, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_spawn_experience_orb";
  case PACKET_ID(0x12, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_entity_velocity";
  case PACKET_ID(0x14, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_entity";
  case PACKET_ID(0x15, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_entity_relative_move";
  case PACKET_ID(0x16, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_entity_look";
  case PACKET_ID(0x17, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_entity_look_and_relative_move";
  case PACKET_ID(0x18, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_entity_teleport";
  case PACKET_ID(0x19, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_entity_head_look";
  case PACKET_ID(0x1A, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_entity_status";
  case PACKET_ID(0x1B, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_attach_entity";
  case PACKET_ID(0x1C, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_entity_metadata";
  case PACKET_ID(0x1D, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_entity_effect";
  case PACKET_ID(0x1E, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_remove_entity_effect";
  case PACKET_ID(0x1F, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_set_experience";
  case PACKET_ID(0x21, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_chunk_data";
  case PACKET_ID(0x23, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_block_change";
  case PACKET_ID(0x24, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_block_action";
  case PACKET_ID(0x25, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_block_break_animation";
  case PACKET_ID(0x28, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_effect";
  case PACKET_ID(0x29, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_sound_effect";
  case PACKET_ID(0x2B, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_change_game_state";
  case PACKET_ID(0x39, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_player_abilities";
  case PACKET_ID(0x3F, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_plugin_message";
  case PACKET_ID(0x40, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_disconnect";
  case PACKET_ID(0x41, CMC_CONN_STATE_PLAY, CMC_DIRECTION_S2C):
    return "S2C_play_change_difficulty";
  case PACKET_ID(0x00, CMC_CONN_STATE_PLAY, CMC_DIRECTION_C2S):
    return "C2S_play_keep_alive";
    // CGSE: packet_id_to_string
  default:
    return "PACKET_UNKNOWN";
  }

#undef PACKET_ID_TO_STRING_UTIL
}

// CGSS: send_methods_c
void cmc_send_C2S_handshake_handshake_packet(cmc_conn *conn, C2S_handshake_handshake_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x00);packet->protocole_version=cmc_buffer_unpack_varint(buff);packet->server_addr=cmc_buffer_unpack_string(buff);packet->server_port=cmc_buffer_unpack_ushort(buff);packet->next_state=cmc_buffer_unpack_varint(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_status_response_packet(cmc_conn *conn, S2C_status_response_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x00);packet->response=cmc_buffer_unpack_string(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_status_pong_packet(cmc_conn *conn, S2C_status_pong_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x01);packet->payload=cmc_buffer_unpack_long(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_C2S_status_request_packet(cmc_conn *conn){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x00);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_C2S_status_ping_packet(cmc_conn *conn, C2S_status_ping_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x01);packet->payload=cmc_buffer_unpack_long(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_login_disconnect_packet(cmc_conn *conn, S2C_login_disconnect_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x00);packet->reason=cmc_buffer_unpack_string(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_login_encryption_request_packet(cmc_conn *conn, S2C_login_encryption_request_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x01);packet->server_id=cmc_buffer_unpack_string(buff);packet->public_key=cmc_buffer_unpack_byte_array(buff);packet->verify_token=cmc_buffer_unpack_byte_array(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_login_success_packet(cmc_conn *conn, S2C_login_success_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x02);packet->uuid=cmc_buffer_unpack_string(buff);packet->name=cmc_buffer_unpack_string(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_login_set_compression_packet(cmc_conn *conn, S2C_login_set_compression_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x03);packet->threshold=cmc_buffer_unpack_varint(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_C2S_login_start_packet(cmc_conn *conn, C2S_login_start_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x00);packet->name=cmc_buffer_unpack_string(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_C2S_login_encryption_response_packet(cmc_conn *conn, C2S_login_encryption_response_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x01);packet->shared_secret=cmc_buffer_unpack_byte_array(buff);packet->verify_token=cmc_buffer_unpack_byte_array(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_keep_alive_packet(cmc_conn *conn, S2C_play_keep_alive_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x00);packet->keep_alive_id=cmc_buffer_unpack_varint(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_join_game_packet(cmc_conn *conn, S2C_play_join_game_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x01);packet->entity_id=cmc_buffer_unpack_int(buff);packet->gamemode=cmc_buffer_unpack_byte(buff);packet->dimension=cmc_buffer_unpack_char(buff);packet->difficulty=cmc_buffer_unpack_byte(buff);packet->max_players=cmc_buffer_unpack_byte(buff);packet->level_type=cmc_buffer_unpack_string(buff);packet->reduced_debug_info=cmc_buffer_unpack_bool(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_chat_message_packet(cmc_conn *conn, S2C_play_chat_message_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x02);packet->message=cmc_buffer_unpack_string(buff);packet->position=cmc_buffer_unpack_char(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_time_update_packet(cmc_conn *conn, S2C_play_time_update_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x03);packet->world_age=cmc_buffer_unpack_long(buff);packet->time_of_day=cmc_buffer_unpack_long(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_entity_equipment_packet(cmc_conn *conn, S2C_play_entity_equipment_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x04);packet->entity_id=cmc_buffer_unpack_varint(buff);packet->slot=cmc_buffer_unpack_short(buff);packet->item=cmc_buffer_unpack_slot(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_spawn_position_packet(cmc_conn *conn, S2C_play_spawn_position_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x05);packet->location=cmc_buffer_unpack_position(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_update_health_packet(cmc_conn *conn, S2C_play_update_health_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x06);packet->health=cmc_buffer_unpack_float(buff);packet->food=cmc_buffer_unpack_varint(buff);packet->food_saturation=cmc_buffer_unpack_float(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_respawn_packet(cmc_conn *conn, S2C_play_respawn_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x07);packet->dimesion=cmc_buffer_unpack_int(buff);packet->difficulty=cmc_buffer_unpack_byte(buff);packet->gamemode=cmc_buffer_unpack_byte(buff);packet->level_type=cmc_buffer_unpack_string(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_player_look_and_position_packet(cmc_conn *conn, S2C_play_player_look_and_position_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x08);packet->x=cmc_buffer_unpack_double(buff);packet->y=cmc_buffer_unpack_double(buff);packet->z=cmc_buffer_unpack_double(buff);packet->yaw=cmc_buffer_unpack_float(buff);packet->pitch=cmc_buffer_unpack_float(buff);packet->flags=cmc_buffer_unpack_byte(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_held_item_change_packet(cmc_conn *conn, S2C_play_held_item_change_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x09);packet->slot=cmc_buffer_unpack_char(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_use_bed_packet(cmc_conn *conn, S2C_play_use_bed_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x0A);packet->entity_id=cmc_buffer_unpack_varint(buff);packet->location=cmc_buffer_unpack_position(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_animation_packet(cmc_conn *conn, S2C_play_animation_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x0B);packet->entity_id=cmc_buffer_unpack_varint(buff);packet->animation=cmc_buffer_unpack_byte(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_spawn_player_packet(cmc_conn *conn, S2C_play_spawn_player_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x0C);packet->entity_id=cmc_buffer_unpack_varint(buff);packet->uuid=cmc_buffer_unpack_ullong(buff);packet->x=cmc_buffer_unpack_int(buff);packet->y=cmc_buffer_unpack_int(buff);packet->z=cmc_buffer_unpack_int(buff);packet->yaw=cmc_buffer_unpack_byte(buff);packet->pitch=cmc_buffer_unpack_byte(buff);packet->current_item=cmc_buffer_unpack_short(buff);packet->meta_data=cmc_buffer_unpack_entity_metadata(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_collect_item_packet(cmc_conn *conn, S2C_play_collect_item_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x0D);packet->collected_entity_id=cmc_buffer_unpack_varint(buff);packet->collector_entity_id=cmc_buffer_unpack_varint(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_spawn_mob_packet(cmc_conn *conn, S2C_play_spawn_mob_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x0F);packet->entity_id=cmc_buffer_unpack_varint(buff);packet->type=cmc_buffer_unpack_byte(buff);packet->x=cmc_buffer_unpack_int(buff);packet->y=cmc_buffer_unpack_int(buff);packet->z=cmc_buffer_unpack_int(buff);packet->yaw=cmc_buffer_unpack_byte(buff);packet->pitch=cmc_buffer_unpack_byte(buff);packet->head_pitch=cmc_buffer_unpack_byte(buff);packet->x_vel=cmc_buffer_unpack_short(buff);packet->y_vel=cmc_buffer_unpack_short(buff);packet->z_vel=cmc_buffer_unpack_short(buff);packet->meta_data=cmc_buffer_unpack_entity_metadata(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_spawn_painting_packet(cmc_conn *conn, S2C_play_spawn_painting_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x10);packet->entity_id=cmc_buffer_unpack_varint(buff);packet->title=cmc_buffer_unpack_string(buff);packet->location=cmc_buffer_unpack_position(buff);packet->direction=cmc_buffer_unpack_byte(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_spawn_experience_orb_packet(cmc_conn *conn, S2C_play_spawn_experience_orb_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x11);packet->entity_id=cmc_buffer_unpack_varint(buff);packet->x=cmc_buffer_unpack_int(buff);packet->y=cmc_buffer_unpack_int(buff);packet->z=cmc_buffer_unpack_int(buff);packet->count=cmc_buffer_unpack_short(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_entity_velocity_packet(cmc_conn *conn, S2C_play_entity_velocity_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x12);packet->entity_id=cmc_buffer_unpack_varint(buff);packet->x_vel=cmc_buffer_unpack_short(buff);packet->y_vel=cmc_buffer_unpack_short(buff);packet->z_vel=cmc_buffer_unpack_short(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_entity_packet(cmc_conn *conn, S2C_play_entity_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x14);packet->entity_id=cmc_buffer_unpack_varint(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_entity_relative_move_packet(cmc_conn *conn, S2C_play_entity_relative_move_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x15);packet->entity_id=cmc_buffer_unpack_varint(buff);packet->delta_x=cmc_buffer_unpack_char(buff);packet->delta_y=cmc_buffer_unpack_char(buff);packet->delta_z=cmc_buffer_unpack_char(buff);packet->on_ground=cmc_buffer_unpack_bool(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_entity_look_packet(cmc_conn *conn, S2C_play_entity_look_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x16);packet->entity_id=cmc_buffer_unpack_varint(buff);packet->yaw=cmc_buffer_unpack_byte(buff);packet->pitch=cmc_buffer_unpack_byte(buff);packet->on_ground=cmc_buffer_unpack_bool(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_entity_look_and_relative_move_packet(cmc_conn *conn, S2C_play_entity_look_and_relative_move_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x17);packet->entity_id=cmc_buffer_unpack_varint(buff);packet->delta_x=cmc_buffer_unpack_char(buff);packet->delta_y=cmc_buffer_unpack_char(buff);packet->delta_z=cmc_buffer_unpack_char(buff);packet->yaw=cmc_buffer_unpack_byte(buff);packet->pitch=cmc_buffer_unpack_byte(buff);packet->on_ground=cmc_buffer_unpack_bool(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_entity_teleport_packet(cmc_conn *conn, S2C_play_entity_teleport_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x18);packet->entity_id=cmc_buffer_unpack_varint(buff);packet->x=cmc_buffer_unpack_int(buff);packet->y=cmc_buffer_unpack_int(buff);packet->z=cmc_buffer_unpack_int(buff);packet->yaw=cmc_buffer_unpack_byte(buff);packet->pitch=cmc_buffer_unpack_byte(buff);packet->on_ground=cmc_buffer_unpack_bool(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_entity_head_look_packet(cmc_conn *conn, S2C_play_entity_head_look_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x19);packet->entity_id=cmc_buffer_unpack_varint(buff);packet->head_yaw=cmc_buffer_unpack_byte(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_entity_status_packet(cmc_conn *conn, S2C_play_entity_status_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x1A);packet->entity_id=cmc_buffer_unpack_int(buff);packet->entity_status=cmc_buffer_unpack_char(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_attach_entity_packet(cmc_conn *conn, S2C_play_attach_entity_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x1B);packet->entity_id=cmc_buffer_unpack_int(buff);packet->vehicle_id=cmc_buffer_unpack_int(buff);packet->leash=cmc_buffer_unpack_bool(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_entity_metadata_packet(cmc_conn *conn, S2C_play_entity_metadata_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x1C);packet->entity_id=cmc_buffer_unpack_varint(buff);packet->meta_data=cmc_buffer_unpack_entity_metadata(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_entity_effect_packet(cmc_conn *conn, S2C_play_entity_effect_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x1D);packet->entity_id=cmc_buffer_unpack_varint(buff);packet->effect_id=cmc_buffer_unpack_char(buff);packet->amplifier=cmc_buffer_unpack_char(buff);packet->duration=cmc_buffer_unpack_varint(buff);packet->hide_particles=cmc_buffer_unpack_bool(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_remove_entity_effect_packet(cmc_conn *conn, S2C_play_remove_entity_effect_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x1E);packet->entity_id=cmc_buffer_unpack_varint(buff);packet->effect_id=cmc_buffer_unpack_char(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_set_experience_packet(cmc_conn *conn, S2C_play_set_experience_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x1F);packet->experience_bar=cmc_buffer_unpack_float(buff);packet->level=cmc_buffer_unpack_varint(buff);packet->total_experience=cmc_buffer_unpack_varint(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_chunk_data_packet(cmc_conn *conn, S2C_play_chunk_data_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x21);packet->chunk_x=cmc_buffer_unpack_int(buff);packet->chunk_z=cmc_buffer_unpack_int(buff);packet->ground_up_continuous=cmc_buffer_unpack_bool(buff);packet->primary_bitmask=cmc_buffer_unpack_ushort(buff);packet->chunk=cmc_buffer_unpack_byte_array(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_block_change_packet(cmc_conn *conn, S2C_play_block_change_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x23);packet->location=cmc_buffer_unpack_position(buff);packet->block_id=cmc_buffer_unpack_varint(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_block_action_packet(cmc_conn *conn, S2C_play_block_action_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x24);packet->location=cmc_buffer_unpack_position(buff);packet->block_data_1=cmc_buffer_unpack_byte(buff);packet->block_data_2=cmc_buffer_unpack_byte(buff);packet->block_type=cmc_buffer_unpack_varint(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_block_break_animation_packet(cmc_conn *conn, S2C_play_block_break_animation_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x25);packet->entity_id=cmc_buffer_unpack_varint(buff);packet->location=cmc_buffer_unpack_position(buff);packet->destroy_stage=cmc_buffer_unpack_char(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_effect_packet(cmc_conn *conn, S2C_play_effect_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x28);packet->effect_id=cmc_buffer_unpack_int(buff);packet->location=cmc_buffer_unpack_position(buff);packet->data=cmc_buffer_unpack_int(buff);packet->d=cmc_buffer_unpack_bool(buff);packet->particle_id=cmc_buffer_unpack_int(buff);packet->long_distances=cmc_buffer_unpack_bool(buff);packet->x=cmc_buffer_unpack_float(buff);packet->y=cmc_buffer_unpack_float(buff);packet->z=cmc_buffer_unpack_float(buff);packet->x_offset=cmc_buffer_unpack_float(buff);packet->y_offset=cmc_buffer_unpack_float(buff);packet->z_offset=cmc_buffer_unpack_float(buff);packet->particle_data=cmc_buffer_unpack_float(buff);packet->particle_count=cmc_buffer_unpack_int(buff);packet->sable_relative_volume=cmc_buffer_unpack_int(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_sound_effect_packet(cmc_conn *conn, S2C_play_sound_effect_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x29);packet->sound_name=cmc_buffer_unpack_string(buff);packet->x=cmc_buffer_unpack_int(buff);packet->y=cmc_buffer_unpack_int(buff);packet->z=cmc_buffer_unpack_int(buff);packet->volume=cmc_buffer_unpack_float(buff);packet->pitch=cmc_buffer_unpack_byte(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_change_game_state_packet(cmc_conn *conn, S2C_play_change_game_state_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x2B);packet->reason=cmc_buffer_unpack_byte(buff);packet->value=cmc_buffer_unpack_float(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_player_abilities_packet(cmc_conn *conn, S2C_play_player_abilities_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x39);packet->flags=cmc_buffer_unpack_char(buff);packet->flying_speed=cmc_buffer_unpack_float(buff);packet->fov_modifier=cmc_buffer_unpack_float(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_plugin_message_packet(cmc_conn *conn, S2C_play_plugin_message_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x3F);packet->channel=cmc_buffer_unpack_string(buff);packet->data=cmc_buffer_unpack_byte_array(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_disconnect_packet(cmc_conn *conn, S2C_play_disconnect_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x40);packet->reason=cmc_buffer_unpack_string(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_S2C_play_change_difficulty_packet(cmc_conn *conn, S2C_play_change_difficulty_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x41);packet->difficulty=cmc_buffer_unpack_byte(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }

void cmc_send_C2S_play_keep_alive_packet(cmc_conn *conn, C2S_play_keep_alive_packet *packet){cmc_buffer *buff = cmc_buffer_init();switch(buff->protocol_version) {case 47: {cmc_buffer_pack_varint(buff, 0x00);packet->keep_alive_id=cmc_buffer_unpack_varint(buff);break;}default: 
        cmc_buffer_free(buff);
        ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return;);
    }
    cmc_conn_send_packet(conn, buff);
    cmc_buffer_free(buff);
    }


// CGSE: send_methods_c

// CGSS: unpack_methods_c
C2S_handshake_handshake_packet *unpack_C2S_handshake_handshake_packet(cmc_buffer *buff) {C2S_handshake_handshake_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->protocole_version=cmc_buffer_unpack_varint(buff);packet->server_addr=cmc_buffer_unpack_string(buff);packet->server_port=cmc_buffer_unpack_ushort(buff);packet->next_state=cmc_buffer_unpack_varint(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_status_response_packet *unpack_S2C_status_response_packet(cmc_buffer *buff) {S2C_status_response_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->response=cmc_buffer_unpack_string(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_status_pong_packet *unpack_S2C_status_pong_packet(cmc_buffer *buff) {S2C_status_pong_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->payload=cmc_buffer_unpack_long(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

C2S_status_ping_packet *unpack_C2S_status_ping_packet(cmc_buffer *buff) {C2S_status_ping_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->payload=cmc_buffer_unpack_long(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_login_disconnect_packet *unpack_S2C_login_disconnect_packet(cmc_buffer *buff) {S2C_login_disconnect_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->reason=cmc_buffer_unpack_string(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_login_encryption_request_packet *unpack_S2C_login_encryption_request_packet(cmc_buffer *buff) {S2C_login_encryption_request_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->server_id=cmc_buffer_unpack_string(buff);packet->public_key=cmc_buffer_unpack_byte_array(buff);packet->verify_token=cmc_buffer_unpack_byte_array(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_login_success_packet *unpack_S2C_login_success_packet(cmc_buffer *buff) {S2C_login_success_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->uuid=cmc_buffer_unpack_string(buff);packet->name=cmc_buffer_unpack_string(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_login_set_compression_packet *unpack_S2C_login_set_compression_packet(cmc_buffer *buff) {S2C_login_set_compression_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->threshold=cmc_buffer_unpack_varint(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

C2S_login_start_packet *unpack_C2S_login_start_packet(cmc_buffer *buff) {C2S_login_start_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->name=cmc_buffer_unpack_string(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

C2S_login_encryption_response_packet *unpack_C2S_login_encryption_response_packet(cmc_buffer *buff) {C2S_login_encryption_response_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->shared_secret=cmc_buffer_unpack_byte_array(buff);packet->verify_token=cmc_buffer_unpack_byte_array(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_keep_alive_packet *unpack_S2C_play_keep_alive_packet(cmc_buffer *buff) {S2C_play_keep_alive_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->keep_alive_id=cmc_buffer_unpack_varint(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_join_game_packet *unpack_S2C_play_join_game_packet(cmc_buffer *buff) {S2C_play_join_game_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->entity_id=cmc_buffer_unpack_int(buff);packet->gamemode=cmc_buffer_unpack_byte(buff);packet->dimension=cmc_buffer_unpack_char(buff);packet->difficulty=cmc_buffer_unpack_byte(buff);packet->max_players=cmc_buffer_unpack_byte(buff);packet->level_type=cmc_buffer_unpack_string(buff);packet->reduced_debug_info=cmc_buffer_unpack_bool(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_chat_message_packet *unpack_S2C_play_chat_message_packet(cmc_buffer *buff) {S2C_play_chat_message_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->message=cmc_buffer_unpack_string(buff);packet->position=cmc_buffer_unpack_char(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_time_update_packet *unpack_S2C_play_time_update_packet(cmc_buffer *buff) {S2C_play_time_update_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->world_age=cmc_buffer_unpack_long(buff);packet->time_of_day=cmc_buffer_unpack_long(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_entity_equipment_packet *unpack_S2C_play_entity_equipment_packet(cmc_buffer *buff) {S2C_play_entity_equipment_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->entity_id=cmc_buffer_unpack_varint(buff);packet->slot=cmc_buffer_unpack_short(buff);packet->item=cmc_buffer_unpack_slot(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_spawn_position_packet *unpack_S2C_play_spawn_position_packet(cmc_buffer *buff) {S2C_play_spawn_position_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->location=cmc_buffer_unpack_position(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_update_health_packet *unpack_S2C_play_update_health_packet(cmc_buffer *buff) {S2C_play_update_health_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->health=cmc_buffer_unpack_float(buff);packet->food=cmc_buffer_unpack_varint(buff);packet->food_saturation=cmc_buffer_unpack_float(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_respawn_packet *unpack_S2C_play_respawn_packet(cmc_buffer *buff) {S2C_play_respawn_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->dimesion=cmc_buffer_unpack_int(buff);packet->difficulty=cmc_buffer_unpack_byte(buff);packet->gamemode=cmc_buffer_unpack_byte(buff);packet->level_type=cmc_buffer_unpack_string(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_player_look_and_position_packet *unpack_S2C_play_player_look_and_position_packet(cmc_buffer *buff) {S2C_play_player_look_and_position_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->x=cmc_buffer_unpack_double(buff);packet->y=cmc_buffer_unpack_double(buff);packet->z=cmc_buffer_unpack_double(buff);packet->yaw=cmc_buffer_unpack_float(buff);packet->pitch=cmc_buffer_unpack_float(buff);packet->flags=cmc_buffer_unpack_byte(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_held_item_change_packet *unpack_S2C_play_held_item_change_packet(cmc_buffer *buff) {S2C_play_held_item_change_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->slot=cmc_buffer_unpack_char(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_use_bed_packet *unpack_S2C_play_use_bed_packet(cmc_buffer *buff) {S2C_play_use_bed_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->entity_id=cmc_buffer_unpack_varint(buff);packet->location=cmc_buffer_unpack_position(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_animation_packet *unpack_S2C_play_animation_packet(cmc_buffer *buff) {S2C_play_animation_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->entity_id=cmc_buffer_unpack_varint(buff);packet->animation=cmc_buffer_unpack_byte(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_spawn_player_packet *unpack_S2C_play_spawn_player_packet(cmc_buffer *buff) {S2C_play_spawn_player_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->entity_id=cmc_buffer_unpack_varint(buff);packet->uuid=cmc_buffer_unpack_ullong(buff);packet->x=cmc_buffer_unpack_int(buff);packet->y=cmc_buffer_unpack_int(buff);packet->z=cmc_buffer_unpack_int(buff);packet->yaw=cmc_buffer_unpack_byte(buff);packet->pitch=cmc_buffer_unpack_byte(buff);packet->current_item=cmc_buffer_unpack_short(buff);packet->meta_data=cmc_buffer_unpack_entity_metadata(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_collect_item_packet *unpack_S2C_play_collect_item_packet(cmc_buffer *buff) {S2C_play_collect_item_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->collected_entity_id=cmc_buffer_unpack_varint(buff);packet->collector_entity_id=cmc_buffer_unpack_varint(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_spawn_mob_packet *unpack_S2C_play_spawn_mob_packet(cmc_buffer *buff) {S2C_play_spawn_mob_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->entity_id=cmc_buffer_unpack_varint(buff);packet->type=cmc_buffer_unpack_byte(buff);packet->x=cmc_buffer_unpack_int(buff);packet->y=cmc_buffer_unpack_int(buff);packet->z=cmc_buffer_unpack_int(buff);packet->yaw=cmc_buffer_unpack_byte(buff);packet->pitch=cmc_buffer_unpack_byte(buff);packet->head_pitch=cmc_buffer_unpack_byte(buff);packet->x_vel=cmc_buffer_unpack_short(buff);packet->y_vel=cmc_buffer_unpack_short(buff);packet->z_vel=cmc_buffer_unpack_short(buff);packet->meta_data=cmc_buffer_unpack_entity_metadata(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_spawn_painting_packet *unpack_S2C_play_spawn_painting_packet(cmc_buffer *buff) {S2C_play_spawn_painting_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->entity_id=cmc_buffer_unpack_varint(buff);packet->title=cmc_buffer_unpack_string(buff);packet->location=cmc_buffer_unpack_position(buff);packet->direction=cmc_buffer_unpack_byte(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_spawn_experience_orb_packet *unpack_S2C_play_spawn_experience_orb_packet(cmc_buffer *buff) {S2C_play_spawn_experience_orb_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->entity_id=cmc_buffer_unpack_varint(buff);packet->x=cmc_buffer_unpack_int(buff);packet->y=cmc_buffer_unpack_int(buff);packet->z=cmc_buffer_unpack_int(buff);packet->count=cmc_buffer_unpack_short(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_entity_velocity_packet *unpack_S2C_play_entity_velocity_packet(cmc_buffer *buff) {S2C_play_entity_velocity_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->entity_id=cmc_buffer_unpack_varint(buff);packet->x_vel=cmc_buffer_unpack_short(buff);packet->y_vel=cmc_buffer_unpack_short(buff);packet->z_vel=cmc_buffer_unpack_short(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_entity_packet *unpack_S2C_play_entity_packet(cmc_buffer *buff) {S2C_play_entity_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->entity_id=cmc_buffer_unpack_varint(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_entity_relative_move_packet *unpack_S2C_play_entity_relative_move_packet(cmc_buffer *buff) {S2C_play_entity_relative_move_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->entity_id=cmc_buffer_unpack_varint(buff);packet->delta_x=cmc_buffer_unpack_char(buff);packet->delta_y=cmc_buffer_unpack_char(buff);packet->delta_z=cmc_buffer_unpack_char(buff);packet->on_ground=cmc_buffer_unpack_bool(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_entity_look_packet *unpack_S2C_play_entity_look_packet(cmc_buffer *buff) {S2C_play_entity_look_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->entity_id=cmc_buffer_unpack_varint(buff);packet->yaw=cmc_buffer_unpack_byte(buff);packet->pitch=cmc_buffer_unpack_byte(buff);packet->on_ground=cmc_buffer_unpack_bool(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_entity_look_and_relative_move_packet *unpack_S2C_play_entity_look_and_relative_move_packet(cmc_buffer *buff) {S2C_play_entity_look_and_relative_move_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->entity_id=cmc_buffer_unpack_varint(buff);packet->delta_x=cmc_buffer_unpack_char(buff);packet->delta_y=cmc_buffer_unpack_char(buff);packet->delta_z=cmc_buffer_unpack_char(buff);packet->yaw=cmc_buffer_unpack_byte(buff);packet->pitch=cmc_buffer_unpack_byte(buff);packet->on_ground=cmc_buffer_unpack_bool(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_entity_teleport_packet *unpack_S2C_play_entity_teleport_packet(cmc_buffer *buff) {S2C_play_entity_teleport_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->entity_id=cmc_buffer_unpack_varint(buff);packet->x=cmc_buffer_unpack_int(buff);packet->y=cmc_buffer_unpack_int(buff);packet->z=cmc_buffer_unpack_int(buff);packet->yaw=cmc_buffer_unpack_byte(buff);packet->pitch=cmc_buffer_unpack_byte(buff);packet->on_ground=cmc_buffer_unpack_bool(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_entity_head_look_packet *unpack_S2C_play_entity_head_look_packet(cmc_buffer *buff) {S2C_play_entity_head_look_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->entity_id=cmc_buffer_unpack_varint(buff);packet->head_yaw=cmc_buffer_unpack_byte(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_entity_status_packet *unpack_S2C_play_entity_status_packet(cmc_buffer *buff) {S2C_play_entity_status_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->entity_id=cmc_buffer_unpack_int(buff);packet->entity_status=cmc_buffer_unpack_char(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_attach_entity_packet *unpack_S2C_play_attach_entity_packet(cmc_buffer *buff) {S2C_play_attach_entity_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->entity_id=cmc_buffer_unpack_int(buff);packet->vehicle_id=cmc_buffer_unpack_int(buff);packet->leash=cmc_buffer_unpack_bool(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_entity_metadata_packet *unpack_S2C_play_entity_metadata_packet(cmc_buffer *buff) {S2C_play_entity_metadata_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->entity_id=cmc_buffer_unpack_varint(buff);packet->meta_data=cmc_buffer_unpack_entity_metadata(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_entity_effect_packet *unpack_S2C_play_entity_effect_packet(cmc_buffer *buff) {S2C_play_entity_effect_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->entity_id=cmc_buffer_unpack_varint(buff);packet->effect_id=cmc_buffer_unpack_char(buff);packet->amplifier=cmc_buffer_unpack_char(buff);packet->duration=cmc_buffer_unpack_varint(buff);packet->hide_particles=cmc_buffer_unpack_bool(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_remove_entity_effect_packet *unpack_S2C_play_remove_entity_effect_packet(cmc_buffer *buff) {S2C_play_remove_entity_effect_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->entity_id=cmc_buffer_unpack_varint(buff);packet->effect_id=cmc_buffer_unpack_char(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_set_experience_packet *unpack_S2C_play_set_experience_packet(cmc_buffer *buff) {S2C_play_set_experience_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->experience_bar=cmc_buffer_unpack_float(buff);packet->level=cmc_buffer_unpack_varint(buff);packet->total_experience=cmc_buffer_unpack_varint(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_chunk_data_packet *unpack_S2C_play_chunk_data_packet(cmc_buffer *buff) {S2C_play_chunk_data_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->chunk_x=cmc_buffer_unpack_int(buff);packet->chunk_z=cmc_buffer_unpack_int(buff);packet->ground_up_continuous=cmc_buffer_unpack_bool(buff);packet->primary_bitmask=cmc_buffer_unpack_ushort(buff);packet->chunk=cmc_buffer_unpack_byte_array(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_block_change_packet *unpack_S2C_play_block_change_packet(cmc_buffer *buff) {S2C_play_block_change_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->location=cmc_buffer_unpack_position(buff);packet->block_id=cmc_buffer_unpack_varint(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_block_action_packet *unpack_S2C_play_block_action_packet(cmc_buffer *buff) {S2C_play_block_action_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->location=cmc_buffer_unpack_position(buff);packet->block_data_1=cmc_buffer_unpack_byte(buff);packet->block_data_2=cmc_buffer_unpack_byte(buff);packet->block_type=cmc_buffer_unpack_varint(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_block_break_animation_packet *unpack_S2C_play_block_break_animation_packet(cmc_buffer *buff) {S2C_play_block_break_animation_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->entity_id=cmc_buffer_unpack_varint(buff);packet->location=cmc_buffer_unpack_position(buff);packet->destroy_stage=cmc_buffer_unpack_char(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_effect_packet *unpack_S2C_play_effect_packet(cmc_buffer *buff) {S2C_play_effect_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->effect_id=cmc_buffer_unpack_int(buff);packet->location=cmc_buffer_unpack_position(buff);packet->data=cmc_buffer_unpack_int(buff);packet->d=cmc_buffer_unpack_bool(buff);packet->particle_id=cmc_buffer_unpack_int(buff);packet->long_distances=cmc_buffer_unpack_bool(buff);packet->x=cmc_buffer_unpack_float(buff);packet->y=cmc_buffer_unpack_float(buff);packet->z=cmc_buffer_unpack_float(buff);packet->x_offset=cmc_buffer_unpack_float(buff);packet->y_offset=cmc_buffer_unpack_float(buff);packet->z_offset=cmc_buffer_unpack_float(buff);packet->particle_data=cmc_buffer_unpack_float(buff);packet->particle_count=cmc_buffer_unpack_int(buff);packet->sable_relative_volume=cmc_buffer_unpack_int(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_sound_effect_packet *unpack_S2C_play_sound_effect_packet(cmc_buffer *buff) {S2C_play_sound_effect_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->sound_name=cmc_buffer_unpack_string(buff);packet->x=cmc_buffer_unpack_int(buff);packet->y=cmc_buffer_unpack_int(buff);packet->z=cmc_buffer_unpack_int(buff);packet->volume=cmc_buffer_unpack_float(buff);packet->pitch=cmc_buffer_unpack_byte(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_change_game_state_packet *unpack_S2C_play_change_game_state_packet(cmc_buffer *buff) {S2C_play_change_game_state_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->reason=cmc_buffer_unpack_byte(buff);packet->value=cmc_buffer_unpack_float(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_player_abilities_packet *unpack_S2C_play_player_abilities_packet(cmc_buffer *buff) {S2C_play_player_abilities_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->flags=cmc_buffer_unpack_char(buff);packet->flying_speed=cmc_buffer_unpack_float(buff);packet->fov_modifier=cmc_buffer_unpack_float(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_plugin_message_packet *unpack_S2C_play_plugin_message_packet(cmc_buffer *buff) {S2C_play_plugin_message_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->channel=cmc_buffer_unpack_string(buff);packet->data=cmc_buffer_unpack_byte_array(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_disconnect_packet *unpack_S2C_play_disconnect_packet(cmc_buffer *buff) {S2C_play_disconnect_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->reason=cmc_buffer_unpack_string(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

S2C_play_change_difficulty_packet *unpack_S2C_play_change_difficulty_packet(cmc_buffer *buff) {S2C_play_change_difficulty_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->difficulty=cmc_buffer_unpack_byte(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}

C2S_play_keep_alive_packet *unpack_C2S_play_keep_alive_packet(cmc_buffer *buff) {C2S_play_keep_alive_packet *packet = MALLOC(sizeof(*packet));switch(buff->protocol_version) {case 47: {packet->keep_alive_id=cmc_buffer_unpack_varint(buff);break;}default: ERR(ERR_UNSUPPORTED_PROTOCOL_VERSION, return NULL;);}
               UNPACK_ERR_HANDELER;return packet;}


// CGSE: unpack_methods_c

// CGSS: free_methods_c
void cmc_free_C2S_handshake_handshake_packet(
    C2S_handshake_handshake_packet packet) {
  free_string(packet.server_addr);
}
void cmc_free_S2C_status_response_packet(S2C_status_response_packet packet) {
  free_string(packet.response);
}

void cmc_free_S2C_login_disconnect_packet(S2C_login_disconnect_packet packet) {
  free_string(packet.reason);
}
void cmc_free_S2C_login_encryption_request_packet(
    S2C_login_encryption_request_packet packet) {
  free_string(packet.server_id);
  free_byte_array(packet.public_key);
  free_byte_array(packet.verify_token);
}
void cmc_free_S2C_login_success_packet(S2C_login_success_packet packet) {
  free_string(packet.uuid);
  free_string(packet.name);
}

void cmc_free_C2S_login_start_packet(C2S_login_start_packet packet) {
  free_string(packet.name);
}
void cmc_free_C2S_login_encryption_response_packet(
    C2S_login_encryption_response_packet packet) {
  free_byte_array(packet.shared_secret);
  free_byte_array(packet.verify_token);
}

void cmc_free_S2C_play_join_game_packet(S2C_play_join_game_packet packet) {
  free_string(packet.level_type);
}
void cmc_free_S2C_play_chat_message_packet(
    S2C_play_chat_message_packet packet) {
  free_string(packet.message);
}

void cmc_free_S2C_play_entity_equipment_packet(
    S2C_play_entity_equipment_packet packet) {
  free_slot(packet.item);
}

void cmc_free_S2C_play_respawn_packet(S2C_play_respawn_packet packet) {
  free_string(packet.level_type);
}

void cmc_free_S2C_play_spawn_player_packet(
    S2C_play_spawn_player_packet packet) {
  free_entity_metadata(packet.meta_data);
}

void cmc_free_S2C_play_spawn_mob_packet(S2C_play_spawn_mob_packet packet) {
  free_entity_metadata(packet.meta_data);
}
void cmc_free_S2C_play_spawn_painting_packet(
    S2C_play_spawn_painting_packet packet) {
  free_string(packet.title);
}

void cmc_free_S2C_play_entity_metadata_packet(
    S2C_play_entity_metadata_packet packet) {
  free_entity_metadata(packet.meta_data);
}

void cmc_free_S2C_play_chunk_data_packet(S2C_play_chunk_data_packet packet) {
  free_byte_array(packet.chunk);
}

void cmc_free_S2C_play_sound_effect_packet(
    S2C_play_sound_effect_packet packet) {
  free_string(packet.sound_name);
}

void cmc_free_S2C_play_plugin_message_packet(
    S2C_play_plugin_message_packet packet) {
  free_string(packet.channel);
  free_byte_array(packet.data);
}
void cmc_free_S2C_play_disconnect_packet(S2C_play_disconnect_packet packet) {
  free_string(packet.reason);
}

// CGSE: free_methods_c