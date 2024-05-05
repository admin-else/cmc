#include <cmc/buffer.h>
#include <cmc/err.h>
#include <cmc/heap_utils.h>
#include <cmc/nbt.h>
#include <ctype.h>
#include <endian.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VARINT_SEGMENT_BITS 0x7F
#define VARINT_CONTINUE_BIT 0x80
#define DEFAULT_MAX_STRING_LENGTH INT16_MAX

void cmc_buffer_print_info(cmc_buffer *buff) {
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

cmc_buffer *cmc_buffer_init(int protocol_version) {
  cmc_buffer *buffer = MALLOC(sizeof(cmc_buffer));
  buffer->data = NULL;
  buffer->capacity = 0;
  buffer->position = 0;
  buffer->length = 0;
  buffer->protocol_version = protocol_version;
  return buffer;
}

void cmc_buffer_free(cmc_buffer *buffer) {
  if (buffer->capacity) {
    FREE(buffer->data);
  }
  buffer->data = NULL;
  buffer->capacity = 0;
  buffer->length = 0;
  FREE(buffer);
}

cmc_buffer *cmc_buffer_combine(cmc_buffer *buff1, cmc_buffer *buff2) {
  cmc_buffer_pack(buff1, buff2->data, buff2->length);
  cmc_buffer_free(buff2);
  return buff1;
}

void cmc_buffer_pack(cmc_buffer *buffer, const void *data, size_t data_size) {
  if (buffer == NULL) {
    ERR(CMC_ERR_INVALID_ARGUMENTS, return;);
    return;
  }

  if (data_size == 0)
    return; // we dont have to do anything...

  if (buffer->data == NULL) {
    buffer->data = MALLOC(data_size);
    buffer->capacity = data_size;
    buffer->length = 0;
  } else if (buffer->length + data_size > buffer->capacity) {
    size_t new_capacity = buffer->capacity * 2;
    while (buffer->length + data_size > new_capacity) {
      new_capacity *= 2;
    }

    unsigned char *new_data =
        (unsigned char *)realloc(buffer->data, new_capacity);
    if (new_data == NULL)
      ERR(CMC_ERR_MEM, return;);

    buffer->data = new_data;
    buffer->capacity = new_capacity;
  }
  if (data != NULL)
    memcpy(buffer->data + buffer->length, data, data_size);
  buffer->length += data_size;
  return;
}

unsigned char *cmc_buffer_unpack(cmc_buffer *buffer, size_t n) {
  if (n <= 0 || buffer == NULL)
    ERR(CMC_ERR_INVALID_ARGUMENTS, return NULL;);
  if (buffer->position + n > buffer->length)
    ERR(CMC_ERR_BUFFER_OVERFLOW, return NULL;);

  unsigned char *readData = MALLOC(n);
  memcpy(readData, buffer->data + buffer->position, n);
  buffer->position += n;
  return readData;
}

// C types

#define NUM_PACK_AND_UNPACK_FUNC_FACTORY(name, type)                           \
  type cmc_buffer_unpack_##name(cmc_buffer *buffer) {                          \
    unsigned char *data = cmc_buffer_unpack(buffer, sizeof(type));             \
    if (data == NULL)                                                          \
      return (type)0;                                                          \
    type result = *((type *)data);                                             \
    FREE(data);                                                                \
    return result;                                                             \
  }                                                                            \
                                                                               \
  void cmc_buffer_pack_##name(cmc_buffer *buffer, type data) {                 \
    cmc_buffer_pack(buffer, &data, sizeof(type));                              \
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

void cmc_buffer_pack_bool(cmc_buffer *buffer, bool value) {
  unsigned char byte_val = 0;
  if (value)
    byte_val = 1;

  cmc_buffer_pack(buffer, &byte_val, 1);
}

bool cmc_buffer_unpack_bool(cmc_buffer *buffer) {
  unsigned char *data = cmc_buffer_unpack(buffer, 1);
  if (data == NULL)
    return false;
  unsigned char byte_val = *data;
  FREE(data);
  if (byte_val)
    return true;
  else
    return false;
}

void cmc_buffer_pack_varint(cmc_buffer *buff, int signed_number) {
  unsigned int number = (unsigned int)signed_number;
  for (int i = 0; i < 5; i++) {
    uint8_t b = number & 0x7F;
    number >>= 7;
    b = b | (number > 0 ? 0x80 : 0);
    cmc_buffer_pack_byte(buff, b);
    ERR_CHECK(return;);
    if (number == 0)
      break;
  }
}

int32_t cmc_buffer_unpack_varint(cmc_buffer *buff) {
  uint32_t number = 0;
  for (int i = 0; i < 5; i++) {
    uint8_t b = cmc_buffer_unpack_byte(buff);
    ERR_CHECK(return 0;);
    number |= (b & VARINT_SEGMENT_BITS) << (7 * i);
    if (!(b & VARINT_CONTINUE_BIT))
      break;
  }
  return (int32_t)number;
}

void cmc_buffer_pack_string_w_max_len(cmc_buffer *buff, const char *value,
                                      int max_len) {
  int str_len = strlen(value);
  if (str_len > max_len)
    ERR(CMC_ERR_STRING_LENGHT, return;);
  cmc_buffer_pack_varint(buff, str_len);
  cmc_buffer_pack(buff, value, str_len);
}

inline void cmc_buffer_pack_string(cmc_buffer *buff, const char *value) {
  cmc_buffer_pack_string_w_max_len(buff, value, DEFAULT_MAX_STRING_LENGTH);
}

char *cmc_buffer_unpack_string_w_max_len(cmc_buffer *buff, int max_len) {
  int str_len = cmc_buffer_unpack_varint(buff);
  ERR_CHECK(return NULL;);
  if (str_len > max_len * 4 || str_len < 0)
    ERR(CMC_ERR_INVALID_ARGUMENTS, return NULL;);

  if (str_len == 0) {
    char *str = MALLOC(1);
    *str = '\0';
    return str;
  }

  if (buff == NULL || buff->position + str_len > buff->length)
    ERR(CMC_ERR_BUFFER_OVERFLOW, return NULL;);

  char *str = (char *)MALLOC(str_len + 1);

  memcpy(str, buff->data + buff->position, str_len);
  str[str_len] = '\0';

  buff->position += str_len;

  return str;
}

char *cmc_buffer_unpack_string(cmc_buffer *buff) {
  return cmc_buffer_unpack_string_w_max_len(buff, DEFAULT_MAX_STRING_LENGTH);
}

void cmc_buffer_pack_position(cmc_buffer *buff, cmc_block_pos pos) {
  uint64_t encoded_pos = ((pos.x & 0x3FFFFFF) << 38) | ((pos.y & 0xFFF) << 26) |
                         (pos.z & 0x3FFFFFF);
  cmc_buffer_pack_long(buff, encoded_pos);
}

cmc_block_pos cmc_buffer_unpack_position(cmc_buffer *buff) {
  uint64_t val = cmc_buffer_unpack_long(buff);
  cmc_block_pos pos = {0, 0, 0};

  ERR_CHECK(return pos;);
  pos.x = val >> 38;
  pos.y = (val >> 26) & 0xFFF;
  pos.z = val << 38 >> 38;

  // this is for negatives
  pos.x = (pos.x >= 33554432) ? (pos.x % 67108864) : pos.x;
  pos.y = (pos.y >= 2048) ? (pos.y % 4096) : pos.y;
  pos.z = (pos.z >= 33554432) ? (pos.z % 67108864) : pos.z;

  return pos;
}

cmc_nbt *cmc_buffer_unpack_nbt(cmc_buffer *buff) {
  return nbt_parse_named_tag(buff);
}

void cmc_buffer_pack_nbt(cmc_buffer *buff, cmc_nbt *nbt) {
  cmc_buffer *tmp_buff = ERR_ABLE(nbt_dump_binary(nbt), return;);
  cmc_buffer_combine(buff, tmp_buff);
}

void cmc_buffer_pack_byte_array(cmc_buffer *buff, cmc_buffer *byte_array) {
  cmc_buffer_pack_varint(buff, byte_array->length);
  cmc_buffer_pack(buff, byte_array->data, byte_array->length);
}

cmc_buffer *cmc_buffer_unpack_byte_array(cmc_buffer *buff) {
  int ret_buff_len = cmc_buffer_unpack_varint(buff);

  cmc_buffer *ret = cmc_buffer_init(buff->protocol_version);
  ret->capacity = ret_buff_len;
  ret->length = ret_buff_len;
  if (ret_buff_len > 0)
    ret->data = cmc_buffer_unpack(buff, ret_buff_len);
  return ret;
}

void cmc_buffer_pack_slot(cmc_buffer *buff, cmc_slot *slot) {
  if (slot == NULL) {
    cmc_buffer_pack_short(buff, -1);
    return;
  }

  cmc_buffer_pack_short(buff, slot->item_id);
  cmc_buffer_pack_char(buff, slot->slot_size);
  cmc_buffer_pack_short(buff, slot->meta_data);
  cmc_buffer_pack_nbt(buff, slot->tag_compound);
}

cmc_slot *cmc_buffer_unpack_slot(cmc_buffer *buff) {
  short item_id = cmc_buffer_unpack_short(buff);
  if (item_id < 0)
    return NULL;
  cmc_slot *slot = MALLOC(sizeof(cmc_slot));
  slot->item_id = item_id;
  slot->slot_size = cmc_buffer_unpack_byte(buff);
  slot->meta_data = cmc_buffer_unpack_short(buff);
  slot->tag_compound = cmc_buffer_unpack_nbt(buff);
  return slot;
}

void cmc_buffer_pack_entity_metadata(cmc_buffer *buff,
                                     cmc_entity_metadata metadata) {
  for (size_t i = 0; i < metadata.size; i++) {
    cmc_entity_metadata_entry *entry =
        metadata.entries + i * sizeof(cmc_entity_metadata_entry);
    cmc_buffer_pack_char(buff, entry->type << 5 | entry->index);
    switch (entry->type) {
    case ENTITY_METADATA_ENTRY_TYPE_BYTE:
      cmc_buffer_pack_byte(buff, entry->payload.byte_data);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_SHORT:
      cmc_buffer_pack_short(buff, entry->payload.short_data);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_INT:
      cmc_buffer_pack_int(buff, entry->payload.int_data);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_FLOAT:
      cmc_buffer_pack_float(buff, entry->payload.float_data);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_STRING:
      cmc_buffer_pack_string(buff, entry->payload.string_data);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_SLOT:
      cmc_buffer_pack_slot(buff, entry->payload.slot_data);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_POSITION:
      cmc_buffer_pack_int(buff, entry->payload.position_data.x);
      cmc_buffer_pack_int(buff, entry->payload.position_data.y);
      cmc_buffer_pack_int(buff, entry->payload.position_data.z);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_ROTATION:
      cmc_buffer_pack_float(buff, entry->payload.rotation_data.x);
      cmc_buffer_pack_float(buff, entry->payload.rotation_data.y);
      cmc_buffer_pack_float(buff, entry->payload.rotation_data.z);
      break;
    }
  }
  cmc_buffer_pack_byte(buff, 127);
}

cmc_entity_metadata cmc_buffer_unpack_entity_metadata(cmc_buffer *buff) {
  cmc_entity_metadata meta_data = {0, NULL};

  while (true) {
    int8_t type_and_index = cmc_buffer_unpack_char(buff);

    if (type_and_index == 127) {
      break;
    }

    cmc_entity_metadata_entry meta_data_entry;
    meta_data_entry.type = abs(type_and_index >> 5);
    meta_data_entry.index = type_and_index & 0x1F;

    switch (meta_data_entry.type) {
    case ENTITY_METADATA_ENTRY_TYPE_BYTE:
      meta_data_entry.payload.byte_data = cmc_buffer_unpack_byte(buff);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_SHORT:
      meta_data_entry.payload.short_data = cmc_buffer_unpack_short(buff);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_INT:
      meta_data_entry.payload.int_data = cmc_buffer_unpack_int(buff);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_FLOAT:
      meta_data_entry.payload.float_data = cmc_buffer_unpack_float(buff);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_STRING:
      meta_data_entry.payload.string_data = cmc_buffer_unpack_string(buff);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_SLOT:
      meta_data_entry.payload.slot_data = cmc_buffer_unpack_slot(buff);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_POSITION:
      meta_data_entry.payload.position_data.x = cmc_buffer_unpack_int(buff);
      meta_data_entry.payload.position_data.y = cmc_buffer_unpack_int(buff);
      meta_data_entry.payload.position_data.z = cmc_buffer_unpack_int(buff);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_ROTATION:
      meta_data_entry.payload.rotation_data.x = cmc_buffer_unpack_float(buff);
      meta_data_entry.payload.rotation_data.y = cmc_buffer_unpack_float(buff);
      meta_data_entry.payload.rotation_data.z = cmc_buffer_unpack_float(buff);
      break;
    }

    cmc_entity_metadata_entry *new_entries =
        realloc(meta_data.entries,
                (meta_data.size + 1) * sizeof(cmc_entity_metadata_entry));
    if (new_entries == NULL) {
      ERR(CMC_ERR_MEM, goto on_error;);
    }

    meta_data.entries = new_entries;

    meta_data.entries[meta_data.size] = meta_data_entry;

    meta_data.size++;
  }
  return meta_data;

on_error:
  FREE(meta_data.entries);
  meta_data.size = 0;
  meta_data.entries = NULL;
  return meta_data;
}

void free_entity_metadata(cmc_entity_metadata metadata) {
  for (size_t i = 0; i < metadata.size; i++) {
    cmc_entity_metadata_entry *entry =
        metadata.entries + i * sizeof(cmc_entity_metadata_entry);
    switch (entry->type) {
    case ENTITY_METADATA_ENTRY_TYPE_SLOT:
      free_slot(entry->payload.slot_data);
      ERR(CMC_ERR_NOT_IMPLEMENTED_YET, );
      break;
    case ENTITY_METADATA_ENTRY_TYPE_STRING:
      FREE(entry->payload.string_data);
      break;
    default:
      break;
    }
  }
  FREE(metadata.entries);
}

void free_string(char *str) { FREE(str); }

void free_slot(cmc_slot *slot) {
  nbt_free(slot->tag_compound);
  FREE(slot);
}

cmc_uuid cmc_buffer_unpack_uuid(cmc_buffer *buff) {
  return (cmc_uuid){.lower = cmc_buffer_unpack_long(buff),
                    .upper = cmc_buffer_unpack_long(buff)};
}

void cmc_buffer_pack_uuid(cmc_buffer *buff, cmc_uuid uuid) {
  cmc_buffer_pack_long(buff, uuid.lower);
  cmc_buffer_pack_long(buff, uuid.upper);
}