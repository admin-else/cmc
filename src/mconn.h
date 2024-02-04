#pragma once

#include "mcbuffer.h"
#include "mctypes.h"
#include "packet_types.h"
#include <netinet/in.h>
#include <stddef.h>
#include <stdint.h>

struct MConn;

struct MConn {
  int sockfd;
  struct sockaddr_in addr;
  MConn_state state; // SEE MCONN_STATE_ macros
  MConn_state next_state;
  varint_t compression_threshold;
  unsigned char *shared_secret;
  /* string literal not in the heap */
  char *name;
  struct {
    // CGSS: on_packet_fuction_pointers
    void (*keep_alive)(const S2C_play_keep_alive_packet_t packet,
                       struct MConn *conn);
    void (*join_game)(const S2C_play_join_game_packet_t packet,
                      struct MConn *conn);
    void (*chat_message)(const S2C_play_chat_message_packet_t packet,
                         struct MConn *conn);
    void (*time_update)(const S2C_play_time_update_packet_t packet,
                        struct MConn *conn);
    void (*entity_equipment)(const S2C_play_entity_equipment_packet_t packet,
                             struct MConn *conn);
    void (*spawn_position)(const S2C_play_spawn_position_packet_t packet,
                           struct MConn *conn);
    void (*update_health)(const S2C_play_update_health_packet_t packet,
                          struct MConn *conn);
    void (*respawn)(const S2C_play_respawn_packet_t packet, struct MConn *conn);
    void (*player_look_and_position)(
        const S2C_play_player_look_and_position_packet_t packet,
        struct MConn *conn);
    void (*held_item_change)(const S2C_play_held_item_change_packet_t packet,
                             struct MConn *conn);
    void (*use_bed)(const S2C_play_use_bed_packet_t packet, struct MConn *conn);
    void (*animation)(const S2C_play_animation_packet_t packet,
                      struct MConn *conn);
    void (*spawn_player)(const S2C_play_spawn_player_packet_t packet,
                         struct MConn *conn);
    void (*collect_item)(const S2C_play_collect_item_packet_t packet,
                         struct MConn *conn);
    void (*spawn_mob)(const S2C_play_spawn_mob_packet_t packet,
                      struct MConn *conn);
    void (*spawn_painting)(const S2C_play_spawn_painting_packet_t packet,
                           struct MConn *conn);
    void (*spawn_experience_orb)(
        const S2C_play_spawn_experience_orb_packet_t packet,
        struct MConn *conn);
    void (*entity_velocity)(const S2C_play_entity_velocity_packet_t packet,
                            struct MConn *conn);
    void (*entity)(const S2C_play_entity_packet_t packet, struct MConn *conn);
    void (*entity_relative_move)(
        const S2C_play_entity_relative_move_packet_t packet,
        struct MConn *conn);
    void (*entity_look)(const S2C_play_entity_look_packet_t packet,
                        struct MConn *conn);
    void (*entity_look_and_relative_move)(
        const S2C_play_entity_look_and_relative_move_packet_t packet,
        struct MConn *conn);
    void (*entity_teleport)(const S2C_play_entity_teleport_packet_t packet,
                            struct MConn *conn);
    void (*entity_head_look)(const S2C_play_entity_head_look_packet_t packet,
                             struct MConn *conn);
    void (*entity_status)(const S2C_play_entity_status_packet_t packet,
                          struct MConn *conn);
    void (*attach_entity)(const S2C_play_attach_entity_packet_t packet,
                          struct MConn *conn);
    void (*entity_metadata)(const S2C_play_entity_metadata_packet_t packet,
                            struct MConn *conn);
    void (*entity_effect)(const S2C_play_entity_effect_packet_t packet,
                          struct MConn *conn);
    void (*remove_entity_effect)(
        const S2C_play_remove_entity_effect_packet_t packet,
        struct MConn *conn);
    void (*set_experience)(const S2C_play_set_experience_packet_t packet,
                           struct MConn *conn);
    void (*chunk_data)(const S2C_play_chunk_data_packet_t packet,
                       struct MConn *conn);
    void (*block_change)(const S2C_play_block_change_packet_t packet,
                         struct MConn *conn);
    void (*block_action)(const S2C_play_block_action_packet_t packet,
                         struct MConn *conn);
    void (*block_break_animation)(
        const S2C_play_block_break_animation_packet_t packet,
        struct MConn *conn);
    void (*effect)(const S2C_play_effect_packet_t packet, struct MConn *conn);
    void (*sound_effect)(const S2C_play_sound_effect_packet_t packet,
                         struct MConn *conn);
    void (*change_game_state)(const S2C_play_change_game_state_packet_t packet,
                              struct MConn *conn);
    void (*player_abilities)(const S2C_play_player_abilities_packet_t packet,
                             struct MConn *conn);
    void (*plugin_message)(const S2C_play_plugin_message_packet_t packet,
                           struct MConn *conn);
    void (*disconnect)(const S2C_play_disconnect_packet_t packet,
                       struct MConn *conn);
    void (*change_difficulty)(const S2C_play_change_difficulty_packet_t packet,
                              struct MConn *conn);
    // CGSE: on_packet_fuction_pointers
  } on_packet;
};

struct MConn *MConn_init();

MCbuffer *MConn_recive_packet(struct MConn *conn);

void MConn_send_buffer(struct MConn *conn, MCbuffer *buff);

void MConn_send_and_free_buffer(struct MConn *conn, MCbuffer *buff);

void MConn_send_packet(struct MConn *conn, MCbuffer *buff);

MCbuffer *MConn_recive_packet(struct MConn *conn);

void MConn_free(struct MConn *conn);

void MConn_close(struct MConn *conn);

void MConn_loop(struct MConn *conn);