#pragma once

#include "nbt_types.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint8_t *data;
  size_t position;
  size_t length;
  size_t capacity;
  int protcol_version;
} cmc_buffer;

cmc_buffer *cmc_buffer_init();
cmc_buffer *cmc_buffer_init_w_size(size_t n);
void cmc_buffer_print_info(cmc_buffer *buff);
void cmc_buffer_free(cmc_buffer *buffer);
cmc_buffer *cmc_buffer_combine(cmc_buffer *buff1, cmc_buffer *buff2);

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

// booleans
void cmc_buffer_pack_bool(cmc_buffer *buffer, bool value);
bool cmc_buffer_unpack_bool(cmc_buffer *buffer);

// varints
void cmc_buffer_pack_varint(cmc_buffer *buff, int value);
int32_t cmc_buffer_unpack_varint(cmc_buffer *buff);

// strings
void cmc_buffer_pack_string_w_max_len(cmc_buffer *buff, const char *value,
                                      int max_len);
void cmc_buffer_pack_string(cmc_buffer *buff, const char *value);
char *cmc_buffer_unpack_string_w_max_len(cmc_buffer *buff, int max_len);
char *cmc_buffer_unpack_string(cmc_buffer *buff);
void free_string(char *str);

// Block pos
typedef struct {
  long x;
  long y;
  long z;
} cmc_block_pos;

void cmc_buffer_pack_position(cmc_buffer *buff, cmc_block_pos pos);
cmc_block_pos cmc_buffer_unpack_position(cmc_buffer *buff);

// nbt
nbt_node *cmc_buffer_unpack_nbt(cmc_buffer *buff);
void cmc_buffer_pack_nbt(cmc_buffer *buff, nbt_node *nbt);

// byte arrays
cmc_buffer *cmc_buffer_unpack_byte_array(cmc_buffer *buff);
void cmc_buffer_pack_byte_array(cmc_buffer *buff, cmc_buffer *byte_array);

// Slots
typedef struct {
  int item_id;
  char slot_size;
  short meta_data;
  nbt_node *tag_compound;
} cmc_slot;

void cmc_buffer_pack_slot(cmc_buffer *buff, cmc_slot *slot);
cmc_slot *cmc_buffer_unpack_slot(cmc_buffer *buff);
void free_slot(cmc_slot *slot);

// Entity metadata
typedef enum {
  ENTITY_METADATA_ENTRY_TYPE_BYTE,
  ENTITY_METADATA_ENTRY_TYPE_SHORT,
  ENTITY_METADATA_ENTRY_TYPE_INT,
  ENTITY_METADATA_ENTRY_TYPE_FLOAT,
  ENTITY_METADATA_ENTRY_TYPE_STRING,
  ENTITY_METADATA_ENTRY_TYPE_SLOT,
  ENTITY_METADATA_ENTRY_TYPE_POSITION,
  ENTITY_METADATA_ENTRY_TYPE_ROTATION
} entity_metadata_entry_types;

typedef struct {
  entity_metadata_entry_types type;
  char index;
  union {
    char byte_data;
    short short_data;
    int int_data;
    float float_data;
    char *string_data;
    cmc_slot *slot_data;
    cmc_block_pos position_data;
    struct {
      float x;
      float y;
      float z;
    } rotation_data;
  } payload;
} cmc_entity_metadata_entry;

typedef struct {
  size_t size;
  cmc_entity_metadata_entry *entries;
} cmc_entity_metadata;

void cmc_buffer_pack_entity_metadata(cmc_buffer *buff,
                                     cmc_entity_metadata metadata);

cmc_entity_metadata cmc_buffer_unpack_entity_metadata(cmc_buffer *buff);

void free_entity_metadata(cmc_entity_metadata metadata);

#define free_byte_array cmc_buffer_free