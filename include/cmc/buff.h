#pragma once

#include <cmc/err.h>
#include <cmc/nbt_types.h>
#include <cmc/protocol.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint8_t *data;
  size_t position;
  size_t length;
  size_t capacity;
  cmc_protocol_version protocol_version;
  cmc_err_extra *err;
} cmc_buff;

/*
May return null if malloc failed.
*/
cmc_buff *cmc_buff_init(cmc_protocol_version protocol_version,
                        cmc_err_extra *err);
void cmc_buff_print_info(cmc_buff *buff);
void cmc_buff_free(cmc_buff *buff);
cmc_buff *cmc_buff_combine(cmc_buff *buff, cmc_buff *tmp);
bool cmc_buff_compare(cmc_buff *buff, cmc_buff *buff2);

cmc_err cmc_buff_pack(cmc_buff *buff, const void *data, size_t data_size);
void *cmc_buff_unpack(cmc_buff *buff, size_t n);
cmc_err cmc_buff_unpack_into(cmc_buff *buff, size_t n, void *write_to);

#define NUM_PACK_AND_UNPACK_FUNC_FACTORY_H(name, type)                         \
  type cmc_buff_unpack_##name(cmc_buff *buff);                                 \
  cmc_err cmc_buff_pack_##name(cmc_buff *buff, type data);

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

#undef NUM_PACK_AND_UNPACK_FUNC_FACTORY_H

// booleans
cmc_err cmc_buff_pack_bool(cmc_buff *buff, bool value);
bool cmc_buff_unpack_bool(cmc_buff *buff);

// varints
cmc_err cmc_buff_pack_varint(cmc_buff *buff, int n);
int32_t cmc_buff_unpack_varint(cmc_buff *buff);

// strings
cmc_err cmc_buff_pack_string_w_max_len(cmc_buff *buff, const char *str,
                                       size_t max_len);
cmc_err cmc_buff_pack_string(cmc_buff *buff, const char *value);
char *cmc_buff_unpack_string_w_max_len(cmc_buff *buff, int max_len);
char *cmc_buff_unpack_string(cmc_buff *buff);
void cmc_string_free(char *str);

// Block pos
typedef struct {
  long x;
  long y;
  long z;
} cmc_block_pos;

cmc_err cmc_buff_pack_position(cmc_buff *buff, cmc_block_pos pos);
cmc_block_pos cmc_buff_unpack_position(cmc_buff *buff);

// nbt
cmc_nbt *cmc_buff_unpack_nbt(cmc_buff *buff);
cmc_err cmc_buff_pack_nbt(cmc_buff *buff, cmc_nbt *nbt);

// byte arrays
cmc_buff *cmc_buff_unpack_buff(cmc_buff *buff);
cmc_err cmc_buff_pack_buff(cmc_buff *buff, cmc_buff *buff2);

// Slots
typedef struct {
  int item_id;
  char slot_size;
  short meta_data;
  cmc_nbt *tag_compound;
} cmc_slot;

cmc_err cmc_buff_pack_slot(cmc_buff *buff, cmc_slot *slot);
cmc_slot *cmc_buff_unpack_slot(cmc_buff *buff);
void cmc_slot_free(cmc_slot *slot);

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

cmc_err cmc_buff_pack_entity_metadata(cmc_buff *buff,
                                      cmc_entity_metadata metadata);

cmc_entity_metadata cmc_buff_unpack_entity_metadata(cmc_buff *buff);

cmc_err cmc_entity_metadata_free(cmc_entity_metadata metadata);

// uuids
typedef struct {
  uint64_t upper;
  uint64_t lower;
} cmc_uuid;

cmc_uuid cmc_buff_unpack_uuid(cmc_buff *buff);
void cmc_buff_pack_uuid(cmc_buff *buff, cmc_uuid uuid);

// array
typedef struct {
  void *data;
  size_t size;
} cmc_array;
