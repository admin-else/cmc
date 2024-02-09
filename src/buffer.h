#pragma once

#include "mctypes.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

cmc_buffer *cmc_buffer_init();

void cmc_buffer_free(cmc_buffer *buffer);

void cmc_buffer_pack(cmc_buffer *buffer, const void *data, size_t data_size);

unsigned char *cmc_buffer_unpack(cmc_buffer *buffer, size_t n);

#define NUM_PACK_AND_UNPACK_FUNC_FACTORY_H(name, type)                         \
  type cmc_buffer_unpack_##name(cmc_buffer *buffer);                           \
  void cmc_buffer_pack_##name(cmc_buffer *buffer, type data);

NUM_PACK_AND_UNPACK_FUNC_FACTORY_H(char, char);
NUM_PACK_AND_UNPACK_FUNC_FACTORY_H(byte, unsigned char);
NUM_PACK_AND_UNPACK_FUNC_FACTORY_H(short, short);
NUM_PACK_AND_UNPACK_FUNC_FACTORY_H(ushort, unsigned short);
NUM_PACK_AND_UNPACK_FUNC_FACTORY_H(int, int);
NUM_PACK_AND_UNPACK_FUNC_FACTORY_H(uint, unsigned int);
NUM_PACK_AND_UNPACK_FUNC_FACTORY_H(long, long);
NUM_PACK_AND_UNPACK_FUNC_FACTORY_H(ulong, unsigned long);
NUM_PACK_AND_UNPACK_FUNC_FACTORY_H(float, float);
NUM_PACK_AND_UNPACK_FUNC_FACTORY_H(double, double);
NUM_PACK_AND_UNPACK_FUNC_FACTORY_H(llong, long long);
NUM_PACK_AND_UNPACK_FUNC_FACTORY_H(ullong, unsigned long long);

#undef NUM_PACK_AND_UNPACK_FUNC_FACTORY_H

void cmc_buffer_pack_bool(cmc_buffer *buffer, bool value);

bool cmc_buffer_unpack_bool(cmc_buffer *buffer);

void cmc_buffer_pack_varint(cmc_buffer *buff, int value);

int32_t cmc_buffer_unpack_varint(cmc_buffer *buff);

void cmc_buffer_pack_string_w_max_len(cmc_buffer *buff, const char *value,
                                      int max_len);

void cmc_buffer_pack_string(cmc_buffer *buff, const char *value);

char *cmc_buffer_unpack_string_w_max_len(cmc_buffer *buff, int max_len);

char *cmc_buffer_unpack_string(cmc_buffer *buff);

void cmc_buffer_pack_position(cmc_buffer *buff, block_pos_t pos);

block_pos_t cmc_buffer_unpack_position(cmc_buffer *buff);

nbt_node *cmc_buffer_unpack_nbt(cmc_buffer *buff);

void cmc_buffer_pack_nbt(cmc_buffer *buff, nbt_node *nbt);

void cmc_buffer_print_info(cmc_buffer *buff);

cmc_buffer *cmc_buffer_combine(cmc_buffer *buff1, cmc_buffer *buff2);

cmc_buffer *cmc_buffer_unpack_byte_array(cmc_buffer *buff);

void cmc_buffer_pack_byte_array(cmc_buffer *buff, cmc_buffer *byte_array);

cmc_buffer *cmc_buffer_init_w_size(size_t n);

void cmc_buffer_pack_slot(cmc_buffer *buff, slot_t *slot);

slot_t *cmc_buffer_unpack_slot(cmc_buffer *buff);

void cmc_buffer_pack_byte_array(cmc_buffer *buff, cmc_buffer *byte_array);

void cmc_buffer_pack_entity_metadata(cmc_buffer *buff,
                                     entity_metadata_t metadata);

entity_metadata_t cmc_buffer_unpack_entity_metadata(cmc_buffer *buff);

void free_entity_metadata(entity_metadata_t metadata);

void free_string(char *str);

void free_slot(slot_t *slot);

#define free_byte_array cmc_buffer_free