#include <cmc/buff.h>

#include <cmc/err.h>
#include <cmc/heap_utils.h>
#include <cmc/nbt.h>

#include <endian.h>

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "err_macros.h"

#define VARINT_SEGMENT_BITS (uint8_t)0x7F
#define VARINT_CONTINUE_BIT (uint8_t)0x80
#define DEFAULT_MAX_STRING_LENGTH INT16_MAX
#define EMPTY_SLOT                                                             \
  (cmc_slot){                                                                  \
      .slot_size = 0, .meta_data = 0, .item_id = -1, .tag_compound = NULL};

void cmc_buff_print_info(cmc_buff *buff) {
  printf("Data      %p\n", buff->data);
  printf("Length    %li\n", buff->length);
  printf("Position  %li\n", buff->position);
  printf("Capacity  %li\n", buff->capacity);
  printf("Hex: \n");
  for (size_t i = 0; i < buff->length; i++) {
    printf("%02X ", buff->data[i]);
  }
  printf("\nPython: \nb'");
  for (size_t i = 0; i < buff->length; i++) {
    if (isprint(buff->data[i])) {
      printf("%c", buff->data[i]);
    } else {
      printf("\\x%02x", buff->data[i]);
    }
  }
  printf("'\n");
}

cmc_buff *cmc_buff_init(cmc_protocol_version protocol_version,
                        cmc_err_extra *err) {
  cmc_buff *buff = malloc(sizeof(cmc_buff));
  if (!buff)
    return NULL;
  buff->data = NULL;
  buff->capacity = 0;
  buff->position = 0;
  buff->length = 0;
  buff->protocol_version = protocol_version;
  buff->err = err;
  return buff;
}

void cmc_buff_free(cmc_buff *buff) {
  if (!buff) // Nothin to do (:
    return;
  if (buff->capacity) {
    free(buff->data);
  }
  buff->data = NULL;
  buff->capacity = 0;
  buff->length = 0;
  free(buff);
}

cmc_buff *cmc_buff_combine(cmc_buff *buff, cmc_buff *tmp) {
  CMC_ERRB_ABLE(cmc_buff_pack(buff, tmp->data, tmp->length), return NULL;);
  cmc_buff_free(tmp);
  return buff;
}

bool cmc_buff_compare(cmc_buff *buff, cmc_buff *buff2) {
  if (buff->length != buff2->length)
    return false;
  if (memcmp(buff->data, buff2->data, buff->length))
    return false;
  return true;
}

cmc_err cmc_buff_pack(cmc_buff *buff, const void *data, size_t data_size) {
  assert(data);
  assert(buff);
  if (data_size == 0)
    return CMC_ERR_NO; // we dont have to do anything...

  if (buff->data == NULL) {
    buff->data = CMC_ERRRB_ABLE(cmc_malloc(data_size, buff->err));
    buff->capacity = data_size;
    buff->length = 0;
  } else if (buff->length + data_size > buff->capacity) {
    size_t new_capacity = buff->capacity * 2;
    while (buff->length + data_size > new_capacity) {
      new_capacity *= 2;
    }

    unsigned char *new_data =
        CMC_ERRRB_ABLE(cmc_realloc(buff->data, new_capacity, buff->err));

    buff->data = new_data;
    buff->capacity = new_capacity;
  }
  memcpy(buff->data + buff->length, data, data_size);
  buff->length += data_size;
  return CMC_ERR_NO;
}

void *cmc_buff_unpack(cmc_buff *buff, size_t n) {
  assert(n > 0);
  assert(buff);
  if (buff->position + n > buff->length)
    CMC_ERRB(CMC_ERR_BUFF_OVERFLOW, return NULL;);

  void *read_data = CMC_ERRB_ABLE(cmc_malloc(n, buff->err), return NULL);
  memcpy(read_data, buff->data + buff->position, n);
  buff->position += n;
  return read_data;
}

// May in to future also implement
cmc_err cmc_buff_unpack_into(cmc_buff *buff, size_t n, void *write_to) {
  assert(n > 0);
  assert(buff);
  if (buff->position + n > buff->length)
    CMC_ERRRB(CMC_ERR_BUFF_OVERFLOW);

  memcpy(write_to, buff->data + buff->position, n);
  buff->position += n;
  return CMC_ERR_NO;
}

#define NUM_PACK_AND_UNPACK_FUNC_FACTORY(name, type)                           \
  type cmc_buff_unpack_##name(cmc_buff *buff) {                                \
    void *data = CMC_ERRB_ABLE(cmc_buff_unpack(buff, sizeof(type)), return 0); \
    type result = *((type *)data);                                             \
    free(data);                                                                \
    return result;                                                             \
  }                                                                            \
                                                                               \
  cmc_err cmc_buff_pack_##name(cmc_buff *buff, type data) {                    \
    return cmc_buff_pack(buff, &data, sizeof(type));                           \
  }

NUM_PACK_AND_UNPACK_FUNC_FACTORY(char, char);
NUM_PACK_AND_UNPACK_FUNC_FACTORY(byte, uint8_t);
NUM_PACK_AND_UNPACK_FUNC_FACTORY(short, int16_t);
NUM_PACK_AND_UNPACK_FUNC_FACTORY(ushort, uint16_t);
NUM_PACK_AND_UNPACK_FUNC_FACTORY(int, int32_t);
NUM_PACK_AND_UNPACK_FUNC_FACTORY(uint, uint32_t);
NUM_PACK_AND_UNPACK_FUNC_FACTORY(long, int64_t);
NUM_PACK_AND_UNPACK_FUNC_FACTORY(ulong, uint64_t);
NUM_PACK_AND_UNPACK_FUNC_FACTORY(float, float);
NUM_PACK_AND_UNPACK_FUNC_FACTORY(double, double);

#undef NUM_PACK_AND_UNPACK_FUNC_FACTORY
// other types

cmc_err cmc_buff_pack_bool(cmc_buff *buff, bool value) {
  assert(buff);
  unsigned char byte_val = 0;
  if (value)
    byte_val = 1;

  return cmc_buff_pack(buff, &byte_val, 1);
}

bool cmc_buff_unpack_bool(cmc_buff *buff) {
  assert(buff);
  unsigned char *data = CMC_ERRB_ABLE(cmc_buff_unpack(buff, 1), goto err;);
  unsigned char byte_val = *data;
  free(data);
  return byte_val;
err:
  free(data);
  return false;
}

cmc_err cmc_buff_pack_varint(cmc_buff *buff, int32_t number) {
  assert(buff);
  for (int i = 0; i < 5; i++) {
    uint8_t b = number & 0x7F;
    number >>= 7;
    b |= number > 0 ? 0x80 : 0;
    CMC_ERRRB_ABLE(cmc_buff_pack_byte(buff, b));
    if (number == 0)
      break;
  }
  return CMC_ERR_NO;
}

int32_t cmc_buff_unpack_varint(cmc_buff *buff) {
  assert(buff);
  int64_t number = 0; // 64 to prevent ub by overflow
  for (int i = 0; i < 5; i++) {
    uint8_t b = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), return 0);
    number |= (int64_t)(uint8_t)(b & VARINT_SEGMENT_BITS) << (7 * i);
    if (!(b & VARINT_CONTINUE_BIT))
      break;
  }
  return (int32_t)number;
}

char *cmc_buff_unpack_string_w_max_len(cmc_buff *buff, int max_len) {
  assert(buff);
  assert(max_len > 0);

  int n = CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), return NULL;);

  if (max_len * 4 < n)
    CMC_ERRB(CMC_ERR_STRING_LENGTH, return NULL;);
  if (n < 0)
    CMC_ERRB(CMC_ERR_NEGATIVE_STRING_LENGTH, return NULL;);

  char *str = NULL;
  if (n > 0)
    str = CMC_ERRB_ABLE(cmc_buff_unpack(buff, n), return NULL;);

  str = CMC_ERRB_ABLE(cmc_realloc(str, n + 1, buff->err), goto err);
  str[n] = '\0';

  int utf_str_len = 0;
  for (int i = 0; i < n; ++i)
    if ((str[i] & 0xC0) != 0x80)
      ++utf_str_len;
  CMC_ERRB_IF(utf_str_len > max_len, CMC_ERR_STRING_LENGTH, goto err;);
  return str;
err:
  free(str);
  return NULL;
}

cmc_err cmc_buff_pack_string_w_max_len(cmc_buff *buff, const char *str,
                                       size_t max_len) {
  assert(buff);
  assert(str);
  size_t str_len_bytes = strlen(str);
  size_t str_len = 0;
  for (size_t i = 0; i < str_len_bytes; ++i)
    if ((str[i] & 0xC0) != 0x80)
      ++str_len;
  if (str_len > max_len)
    CMC_ERRRB(CMC_ERR_STRING_LENGTH);
  CMC_ERRRB_ABLE(cmc_buff_pack_varint(buff, str_len_bytes));
  CMC_ERRRB_ABLE(cmc_buff_pack(buff, str, str_len_bytes));
  return CMC_ERR_NO;
}

cmc_err cmc_buff_pack_string(cmc_buff *buff, const char *value) {
  return cmc_buff_pack_string_w_max_len(buff, value, DEFAULT_MAX_STRING_LENGTH);
}

char *cmc_buff_unpack_string(cmc_buff *buff) {
  return cmc_buff_unpack_string_w_max_len(buff, DEFAULT_MAX_STRING_LENGTH);
}

cmc_err cmc_buff_pack_position(cmc_buff *buff, cmc_block_pos pos) {
  assert(buff);
  uint64_t encoded_pos = ((pos.x & 0x3FFFFFF) << 38) | ((pos.y & 0xFFF) << 26) |
                         (pos.z & 0x3FFFFFF);
  return cmc_buff_pack_long(buff, encoded_pos);
}

cmc_block_pos cmc_buff_unpack_position(cmc_buff *buff) {
  assert(buff);
  cmc_block_pos pos = {0, 0, 0};
  uint64_t val = CMC_ERRB_ABLE(cmc_buff_unpack_long(buff), return pos);

  pos.x = val >> 38;
  pos.y = (val >> 26) & 0xFFF;
  pos.z = val << 38 >> 38;

  // this is for negatives
  pos.x = (pos.x >= 33554432) ? (pos.x % 67108864) : pos.x;
  pos.y = (pos.y >= 2048) ? (pos.y % 4096) : pos.y;
  pos.z = (pos.z >= 33554432) ? (pos.z % 67108864) : pos.z;

  return pos;
}

cmc_nbt *cmc_buff_unpack_nbt(cmc_buff *buff) { return cmc_nbt_parse(buff); }

cmc_err cmc_buff_pack_nbt(cmc_buff *buff, cmc_nbt *nbt) {
  cmc_buff *tmp_buff = CMC_ERRRB_ABLE(cmc_nbt_dump(nbt, buff->err));
  cmc_buff_combine(buff, tmp_buff);
  return CMC_ERR_NO;
}

cmc_err cmc_buff_pack_buff(cmc_buff *buff, cmc_buff *buff2) {
  if (!buff2 || buff2->length <= 0) // Nothin to do if there is nothin
    return CMC_ERR_NO;
  CMC_ERRRB_ABLE(cmc_buff_pack_varint(buff, buff2->length));
  CMC_ERRRB_ABLE(cmc_buff_pack(buff, buff2->data, buff2->length));
  return CMC_ERR_NO;
}

cmc_buff *cmc_buff_unpack_buff(cmc_buff *buff) {
  int32_t ret_buff_len =
      CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), return NULL);

  if (ret_buff_len == 0)
    return NULL;
  if (ret_buff_len < 0) // This should never happen
    CMC_ERRB(CMC_ERR_NEGATIVE_BUFF_LEN, return NULL;);

  cmc_buff *ret = CMC_ERRB_ABLE(
      cmc_buff_init(buff->protocol_version, buff->err), return NULL;);
  void *tmp = CMC_ERRB_ABLE(cmc_buff_unpack(buff, ret_buff_len), goto err;);
  CMC_ERRB_ABLE(cmc_buff_pack(ret, tmp, ret_buff_len), goto err1;);
  free(tmp);

  return ret;
err1:
  free(tmp);
err:
  free(ret);
  return NULL;
}

cmc_err cmc_buff_pack_slot(cmc_buff *buff, cmc_slot *slot) {
  if (slot == NULL) {
    CMC_ERRRB_ABLE(cmc_buff_pack_short(buff, -1));
    return CMC_ERR_NO;
  }

  CMC_ERRRB_ABLE(cmc_buff_pack_short(buff, slot->item_id));
  CMC_ERRRB_ABLE(cmc_buff_pack_char(buff, slot->slot_size));
  CMC_ERRRB_ABLE(cmc_buff_pack_short(buff, slot->meta_data));
  CMC_ERRRB_ABLE(cmc_buff_pack_nbt(buff, slot->tag_compound));
  return CMC_ERR_NO;
}

cmc_slot *cmc_buff_unpack_slot(cmc_buff *buff) {
  short item_id = CMC_ERRB_ABLE(cmc_buff_unpack_short(buff), return NULL;);
  if (item_id < 0)
    return NULL;
  cmc_slot *slot =
      CMC_ERRB_ABLE(cmc_malloc(sizeof(cmc_slot), buff->err), return NULL;);
  slot->item_id = item_id;
  slot->slot_size = CMC_ERRB_ABLE(cmc_buff_unpack_byte(buff), goto err;);
  slot->meta_data = CMC_ERRB_ABLE(cmc_buff_unpack_short(buff), goto err;);
  slot->tag_compound = CMC_ERRB_ABLE(cmc_buff_unpack_nbt(buff), goto err;);
  return slot;
err:
  free(slot);
  return NULL;
}

cmc_err cmc_buff_pack_entity_metadata(cmc_buff *buff,
                                      cmc_entity_metadata metadata) {
  for (size_t i = 0; i < metadata.size; i++) {
    cmc_entity_metadata_entry *entry =
        metadata.entries + i * sizeof(cmc_entity_metadata_entry);
    CMC_ERRRB_ABLE(cmc_buff_pack_char(buff, entry->type << 5 | entry->index));
    switch (entry->type) {
    case ENTITY_METADATA_ENTRY_TYPE_BYTE:
      CMC_ERRRB_ABLE(cmc_buff_pack_byte(buff, entry->payload.byte_data));
      break;
    case ENTITY_METADATA_ENTRY_TYPE_SHORT:
      CMC_ERRRB_ABLE(cmc_buff_pack_short(buff, entry->payload.short_data));
      break;
    case ENTITY_METADATA_ENTRY_TYPE_INT:
      CMC_ERRRB_ABLE(cmc_buff_pack_int(buff, entry->payload.int_data));
      break;
    case ENTITY_METADATA_ENTRY_TYPE_FLOAT:
      CMC_ERRRB_ABLE(cmc_buff_pack_float(buff, entry->payload.float_data));
      break;
    case ENTITY_METADATA_ENTRY_TYPE_STRING:
      CMC_ERRRB_ABLE(cmc_buff_pack_string(buff, entry->payload.string_data));
      break;
    case ENTITY_METADATA_ENTRY_TYPE_SLOT:
      CMC_ERRRB_ABLE(cmc_buff_pack_slot(buff, entry->payload.slot_data));
      break;
    case ENTITY_METADATA_ENTRY_TYPE_POSITION:
      CMC_ERRRB_ABLE(cmc_buff_pack_int(buff, entry->payload.position_data.x));
      CMC_ERRRB_ABLE(cmc_buff_pack_int(buff, entry->payload.position_data.y));
      CMC_ERRRB_ABLE(cmc_buff_pack_int(buff, entry->payload.position_data.z));
      break;
    case ENTITY_METADATA_ENTRY_TYPE_ROTATION:
      CMC_ERRRB_ABLE(cmc_buff_pack_float(buff, entry->payload.rotation_data.x));
      CMC_ERRRB_ABLE(cmc_buff_pack_float(buff, entry->payload.rotation_data.y));
      CMC_ERRRB_ABLE(cmc_buff_pack_float(buff, entry->payload.rotation_data.z));
      break;
    }
  }
  CMC_ERRRB_ABLE(cmc_buff_pack_byte(buff, 127));
  return CMC_ERR_NO;
}

cmc_entity_metadata cmc_buff_unpack_entity_metadata(cmc_buff *buff) {
  cmc_entity_metadata meta_data = (cmc_entity_metadata){0};

  while (true) {
    int8_t type_and_index = CMC_ERRB_ABLE(cmc_buff_unpack_char(buff),
                                          return (cmc_entity_metadata){0});

    if (type_and_index == 127) {
      break;
    }

    cmc_entity_metadata_entry meta_data_entry;
    meta_data_entry.type = abs(type_and_index >> 5);
    meta_data_entry.index = type_and_index & 0x1F;

    switch (meta_data_entry.type) {
    case ENTITY_METADATA_ENTRY_TYPE_BYTE:
      meta_data_entry.payload.byte_data = CMC_ERRB_ABLE(
          cmc_buff_unpack_byte(buff), return (cmc_entity_metadata){0});
      break;
    case ENTITY_METADATA_ENTRY_TYPE_SHORT:
      meta_data_entry.payload.short_data = CMC_ERRB_ABLE(
          cmc_buff_unpack_short(buff), return (cmc_entity_metadata){0});
      break;
    case ENTITY_METADATA_ENTRY_TYPE_INT:
      meta_data_entry.payload.int_data = CMC_ERRB_ABLE(
          cmc_buff_unpack_int(buff), return (cmc_entity_metadata){0});
      break;
    case ENTITY_METADATA_ENTRY_TYPE_FLOAT:
      meta_data_entry.payload.float_data = CMC_ERRB_ABLE(
          cmc_buff_unpack_float(buff), return (cmc_entity_metadata){0});
      break;
    case ENTITY_METADATA_ENTRY_TYPE_STRING:
      meta_data_entry.payload.string_data = CMC_ERRB_ABLE(
          cmc_buff_unpack_string(buff), return (cmc_entity_metadata){0});
      break;
    case ENTITY_METADATA_ENTRY_TYPE_SLOT:
      meta_data_entry.payload.slot_data = CMC_ERRB_ABLE(
          cmc_buff_unpack_slot(buff), return (cmc_entity_metadata){0});
      break;
    case ENTITY_METADATA_ENTRY_TYPE_POSITION:
      meta_data_entry.payload.position_data.x = CMC_ERRB_ABLE(
          cmc_buff_unpack_int(buff), return (cmc_entity_metadata){0});
      meta_data_entry.payload.position_data.y = CMC_ERRB_ABLE(
          cmc_buff_unpack_int(buff), return (cmc_entity_metadata){0});
      meta_data_entry.payload.position_data.z = CMC_ERRB_ABLE(
          cmc_buff_unpack_int(buff), return (cmc_entity_metadata){0});
      break;
    case ENTITY_METADATA_ENTRY_TYPE_ROTATION:
      meta_data_entry.payload.rotation_data.x = CMC_ERRB_ABLE(
          cmc_buff_unpack_float(buff), return (cmc_entity_metadata){0});
      meta_data_entry.payload.rotation_data.y = CMC_ERRB_ABLE(
          cmc_buff_unpack_float(buff), return (cmc_entity_metadata){0});
      meta_data_entry.payload.rotation_data.z = CMC_ERRB_ABLE(
          cmc_buff_unpack_float(buff), return (cmc_entity_metadata){0});
      break;
    }

    cmc_entity_metadata_entry *new_entries = CMC_ERRB_ABLE(
        cmc_realloc(meta_data.entries,
                    (meta_data.size + 1) * sizeof(cmc_entity_metadata_entry),
                    buff->err),
        return (cmc_entity_metadata){0});
    if (new_entries == NULL) {
      CMC_ERRB(CMC_ERR_MEM, goto on_error;);
    }

    meta_data.entries = new_entries;
    meta_data.entries[meta_data.size] = meta_data_entry;
    meta_data.size++;
  }
  return meta_data;

on_error:
  CMC_ERRB_ABLE(cmc_entity_metadata_free(meta_data), );
  return (cmc_entity_metadata){0};
}

cmc_err cmc_entity_metadata_free(cmc_entity_metadata metadata) {
  for (size_t i = 0; i < metadata.size; i++) {
    cmc_entity_metadata_entry *entry =
        metadata.entries + i * sizeof(cmc_entity_metadata_entry);
    switch (entry->type) {
    case ENTITY_METADATA_ENTRY_TYPE_SLOT:
      cmc_slot_free(entry->payload.slot_data);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_STRING:
      cmc_string_free(entry->payload.string_data);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_BYTE:
    case ENTITY_METADATA_ENTRY_TYPE_SHORT:
    case ENTITY_METADATA_ENTRY_TYPE_INT:
    case ENTITY_METADATA_ENTRY_TYPE_FLOAT:
    case ENTITY_METADATA_ENTRY_TYPE_POSITION:
    case ENTITY_METADATA_ENTRY_TYPE_ROTATION:
      break;
    }
  }
  free(metadata.entries);
  return CMC_ERR_NO;
}

void cmc_string_free(char *str) { free(str); }

void cmc_slot_free(cmc_slot *slot) {
  cmc_nbt_free(slot->tag_compound);
  free(slot);
}

cmc_uuid cmc_buff_unpack_uuid(cmc_buff *buff) {
  return (cmc_uuid){.lower = cmc_buff_unpack_long(buff),
                    .upper = cmc_buff_unpack_long(buff)};
}

void cmc_buff_pack_uuid(cmc_buff *buff, cmc_uuid uuid) {
  cmc_buff_pack_long(buff, uuid.lower);
  cmc_buff_pack_long(buff, uuid.upper);
}
