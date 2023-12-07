#include "mcbuffer.h"
#include "mctypes.h"
#include "nbt.h"
#include "heap_utils.h"
#include "text_color.h"
#include <ctype.h>
#include <jansson.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VARINT_SEGMENT_BITS 0x7F
#define VARINT_CONTINUE_BIT 0x80
#define DEFAULT_MAX_STRING_LENGTH INT16_MAX

#define CHECK_ERRMSG                                                           \
  if (*errmsg != NULL)                                                         \
    return 0;

// Endian Utils

/* are we running on a little-endian system? */
static int little_endian() {
  uint16_t t = 0x0001;
  char c[2];
  memcpy(c, &t, sizeof t);
  return c[0];
}

static void *swap_bytes(void *s, size_t len) {
  for (char *b = s, *e = b + len - 1; b < e; b++, e--) {
    char t = *b;

    *b = *e;
    *e = t;
  }

  return s;
}

/* big endian to native endian. works in-place */
static void *be2ne(void *s, size_t len) {
  return little_endian() ? swap_bytes(s, len) : s;
}

/* native endian to big endian. works the exact same as its inverse */
#define ne2be be2ne

#define BE2NE_UTIL(var) be2ne(&var, sizeof(var))

void MCbuffer_print_info(MCbuffer *buff) {
  printf("Data      %p\n", buff->data);
  printf("Length    %li\n", buff->length);
  printf("Position  %li\n", buff->position);
  printf("Capacity  %li\n", buff->capacity);
  printf("Hex: \n");
  for (int i = 0; i < buff->length; i++) {
    printf("%02X ", buff->data[i]);
  }
  printf("\nPython: \nb'");
  for (int i = 0; i < buff->length; i++) {
    if (isprint(buff->data[i])) {
      printf("%c", buff->data[i]);
    } else {
      printf("\\x%02x", buff->data[i]);
    }
  }
  printf("'\n");
}

MCbuffer *MCbuffer_init() {
  MCbuffer *buffer = MALLOC(sizeof(MCbuffer));
  buffer->data = NULL;
  buffer->capacity = 0;
  buffer->position = 0;
  buffer->length = 0;
  return buffer;
}

MCbuffer *MCbuffer_init_w_size(size_t n) {
  MCbuffer *buffer = MALLOC(sizeof(MCbuffer));
  buffer->data = MALLOC(n);
  buffer->capacity = n;
  buffer->length = n;
  buffer->position = 0;
  return buffer;
}

void MCbuffer_free(MCbuffer *buffer) {
  if (buffer->capacity) {
    FREE(buffer->data);
  }
  buffer->data = NULL;
  buffer->capacity = 0;
  buffer->length = 0;
  FREE(buffer);
}

MCbuffer *MCbuffer_combine(MCbuffer *buff1, MCbuffer *buff2, char **errmsg) {
  MCbuffer_pack(buff1, buff2->data, buff2->length, errmsg);
  MCbuffer_free(buff2);
  return buff1;
}

void MCbuffer_pack(MCbuffer *buffer, const void *data, size_t data_size,
                   char **errmsg) {
  if (buffer == NULL || data == NULL || errmsg == NULL || data_size == 0) {
    *errmsg = "Invalid input arguments";
    return;
  }

  if (buffer->data == NULL) {
    buffer->data = (unsigned char *)malloc(data_size);
    if (buffer->data == NULL) {
      *errmsg = "Memory allocation failed";
      return;
    }
    buffer->capacity = data_size;
    buffer->length = 0;
  } else if (buffer->length + data_size > buffer->capacity) {
    size_t new_capacity = buffer->capacity * 2;
    while (buffer->length + data_size > new_capacity) {
      new_capacity *= 2;
    }

    unsigned char *new_data =
        (unsigned char *)realloc(buffer->data, new_capacity);
    if (new_data == NULL) {
      *errmsg = "Memory reallocation failed";
      return;
    }

    buffer->data = new_data;
    buffer->capacity = new_capacity;
  }

  memcpy(buffer->data + buffer->length, data, data_size);
  buffer->length += data_size;
  return;
}

unsigned char *MCbuffer_unpack(MCbuffer *buffer, size_t n, char **errmsg) {
  if (buffer == NULL || buffer->position + n > buffer->length) {
    *errmsg = "Invalid buffer or read position exceeds buffer length";
    return NULL;
  }

  unsigned char *readData = (unsigned char *)MALLOC(n);

  if (n > 0) {
    memcpy(readData, buffer->data + buffer->position, n);
  }

  buffer->position += n;
  return readData;
}

// C types

#define NUM_PACK_AND_UNPACK_FUNC_FACTORY(name, type)                           \
  type MCbuffer_unpack_##name(MCbuffer *buffer, char **errmsg) {               \
    unsigned char *data = MCbuffer_unpack(buffer, sizeof(type), errmsg);       \
    if (data == NULL)                                                          \
      return (type)0;                                                          \
    type result = *((type *)data);                                             \
    FREE(data);                                                                \
    return result;                                                             \
  }                                                                            \
                                                                               \
  void MCbuffer_pack_##name(MCbuffer *buffer, type data, char **errmsg) {      \
    MCbuffer_pack(buffer, &data, sizeof(type), errmsg);                        \
  }

#define NUM_PACK_AND_UNPACK_FUNC_FACTORY_WITH_ENDIAN(name, type)               \
  type MCbuffer_unpack_##name(MCbuffer *buffer, char **errmsg) {               \
    unsigned char *data = MCbuffer_unpack(buffer, sizeof(type), errmsg);       \
    if (data == NULL)                                                          \
      return (type)0;                                                          \
    type result = *((type *)data);                                             \
    FREE(data);                                                                \
    be2ne(&result, sizeof(type));                                              \
    return result;                                                             \
  }                                                                            \
                                                                               \
  void MCbuffer_pack_##name(MCbuffer *buffer, type data, char **errmsg) {      \
    ne2be(&data, sizeof(type));                                                \
    MCbuffer_pack(buffer, &data, sizeof(type), errmsg);                        \
  }

NUM_PACK_AND_UNPACK_FUNC_FACTORY(char, char);
NUM_PACK_AND_UNPACK_FUNC_FACTORY(byte, unsigned char);
NUM_PACK_AND_UNPACK_FUNC_FACTORY(short, short);
NUM_PACK_AND_UNPACK_FUNC_FACTORY(ushort, unsigned short);
NUM_PACK_AND_UNPACK_FUNC_FACTORY_WITH_ENDIAN(int, int);
NUM_PACK_AND_UNPACK_FUNC_FACTORY(
    uint, unsigned int); // when you see a weird number than just swap the func
                         // factory lmao
NUM_PACK_AND_UNPACK_FUNC_FACTORY(long, long);
NUM_PACK_AND_UNPACK_FUNC_FACTORY(ulong, unsigned long);
NUM_PACK_AND_UNPACK_FUNC_FACTORY(float, float);
NUM_PACK_AND_UNPACK_FUNC_FACTORY(double, double);
NUM_PACK_AND_UNPACK_FUNC_FACTORY(llong, long long);
NUM_PACK_AND_UNPACK_FUNC_FACTORY(ullong, unsigned long long);

#undef NUM_PACK_AND_UNPACK_FUNC_FACTORY
// other types

void MCbuffer_pack_bool(MCbuffer *buffer, bool value, char **errmsg) {
  unsigned char byte_val = 0;
  if (value)
    byte_val = 1;

  MCbuffer_pack(buffer, &byte_val, 1, errmsg);
}

bool MCbuffer_unpack_bool(MCbuffer *buffer, char **errmsg) {
  unsigned char *data = MCbuffer_unpack(buffer, 1, errmsg);
  if (data == NULL)
    return false;
  unsigned char byte_val = *data;
  FREE(data);
  if (byte_val)
    return true;
  else
    return false;
}

void MCbuffer_pack_varint(MCbuffer *buff, int signed_number, char **errmsg) {
  unsigned int number = (unsigned int)signed_number;
  for (int i = 0; i < 5; i++) {
    uint8_t b = number & 0x7F;
    number >>= 7;
    b = b | (number > 0 ? 0x80 : 0);
    MCbuffer_pack_byte(buff, b, errmsg);
    if (number == 0 || *errmsg != NULL)
      break;
  }
}

int32_t MCbuffer_unpack_varint(MCbuffer *buff, char **errmsg) {
  uint32_t number = 0;
  for (int i = 0; i < 5; i++) {
    uint8_t b = MCbuffer_unpack_byte(buff, errmsg);
    CHECK_ERRMSG
    number |= (b & VARINT_SEGMENT_BITS) << (7 * i);
    if (!(b & VARINT_CONTINUE_BIT))
      break;
  }
  return (int32_t)number;
}

void MCbuffer_pack_string_w_max_len(MCbuffer *buff, const char *value,
                                    int max_len, char **errmsg) {
  int str_len = strlen(value);
  if (str_len > max_len) {
    sprintf(*errmsg, "String too big (was %d characters, max %d)", str_len,
            max_len);
    return;
  }
  MCbuffer_pack_varint(buff, str_len, errmsg);
  MCbuffer_pack(buff, value, str_len, errmsg);
}

void MCbuffer_pack_string(MCbuffer *buff, const char *value, char **errmsg) {
  MCbuffer_pack_string_w_max_len(buff, value, DEFAULT_MAX_STRING_LENGTH,
                                 errmsg);
}

char *MCbuffer_unpack_string_w_max_len(MCbuffer *buff, int max_len,
                                       char **errmsg) {
  int str_len = MCbuffer_unpack_varint(buff, errmsg);
  CHECK_ERRMSG
  if (str_len > max_len * 4) {
    sprintf(*errmsg,
            "The received encoded string buffer length is longer than maximum"
            "allowed (%i > %i)",
            str_len, max_len);
    return NULL;
  }

  if (str_len < 0) {
    *errmsg = "The received encoded string buffer length is less than zero! "
              "Weird string!";
    return NULL;
  }

  if (str_len == 0) {
    char *str = MALLOC(1);
    *str = '\0';
    return str;
  }

  if (buff == NULL || buff->position + str_len > buff->length) {
    *errmsg = "Invalid buffer or read position exceeds buffer length";
    return NULL;
  }

  char *str = (char *)MALLOC(str_len + 1);

  memcpy(str, buff->data + buff->position, str_len);
  str[str_len] = '\0';

  buff->position += str_len;

  return str;
}

char *MCbuffer_unpack_string(MCbuffer *buff, char **errmsg) {
  return MCbuffer_unpack_string_w_max_len(buff, DEFAULT_MAX_STRING_LENGTH,
                                          errmsg);
}

void MCbuffer_pack_position(MCbuffer *buff, block_pos_t pos, char **errmsg) {
  uint64_t encoded_pos = ((pos.x & 0x3FFFFFF) << 38) | ((pos.y & 0xFFF) << 26) |
                         (pos.z & 0x3FFFFFF);
  MCbuffer_pack_long(buff, encoded_pos, errmsg);
}

block_pos_t MCbuffer_unpack_position(MCbuffer *buff, char **errmsg) {
  uint64_t val = MCbuffer_unpack_long(buff, errmsg);
  block_pos_t pos = {0, 0, 0};

  if (*errmsg != NULL)
    return pos;
  pos.x = val >> 38;
  pos.y = (val >> 26) & 0xFFF;
  pos.z = val << 38 >> 38;

  // this is for negatives
  if (pos.x >= 33554432)
    pos.x -= 67108864;
  if (pos.y >= 2048)
    pos.y -= 4096;
  if (pos.z >= 33554432)
    pos.z -= 67108864;

  return pos;
}

nbt_node *MCbuffer_unpack_nbt(MCbuffer *buff, char **errmsg) {
  nbt_node *nbt = nbt_parse_named_tag(buff, errmsg);
  if (*errmsg != NULL) {
    nbt_free(nbt);
    return NULL;
  }
  return nbt;
}

void MCbuffer_pack_nbt(MCbuffer *buff, nbt_node *nbt, char **errmsg) {
  if (buff != NULL) {
    MCbuffer_pack_varint(buff, 0, errmsg);
    return;
  }

  MCbuffer *tmp_buff = nbt_dump_binary(nbt, errmsg);
  if (*errmsg != NULL) {
    MCbuffer_free(tmp_buff);
    return;
  }
  MCbuffer_combine(buff, tmp_buff, errmsg);
}

json_t *MCbuffer_unpack_json(MCbuffer *buff, char **errmsg) {
  json_t *root = NULL;
  json_error_t error;

  char *str = MCbuffer_unpack_string(buff, errmsg);
  root = json_loads(str, 0, &error);
  if (!root) {
    FREE(str);
    printf("%s\n", error.text);
    printf("%s\n", error.source);
    *errmsg = "Error while reading json";
    return NULL;
  }

  return root;
}

void MCbuffer_pack_json(MCbuffer *buff, json_t *json, char **errmsg) {
  char *jsonString = json_dumps(json, 0);
  MCbuffer_pack_string(buff, jsonString, errmsg);
  FREE(jsonString);
}

void MCbuffer_pack_byte_array(MCbuffer *buff, MCbuffer *byte_array,
                              char **errmsg) {
  MCbuffer_pack_varint(buff, byte_array->length, errmsg);
  MCbuffer_pack(buff, byte_array->data, byte_array->length, errmsg);
}

MCbuffer *MCbuffer_unpack_byte_array(MCbuffer *buff, char **errmsg) {
  int ret_buff_len = MCbuffer_unpack_varint(buff, errmsg);

  MCbuffer *ret = MCbuffer_init();
  ret->capacity = ret_buff_len;
  ret->length = ret_buff_len;
  if (ret_buff_len > 0)
    ret->data = MCbuffer_unpack(buff, ret_buff_len, errmsg);
  return ret;
}

void MCbuffer_pack_slot(MCbuffer *buff, slot_t *slot, char **errmsg) {
  if (slot == NULL) {
    MCbuffer_pack_short(buff, -1, errmsg);
    return;
  }

  MCbuffer_pack_short(buff, slot->item_id, errmsg);
  MCbuffer_pack_char(buff, slot->slot_size, errmsg);
  MCbuffer_pack_short(buff, slot->meta_data, errmsg);
  MCbuffer_pack_nbt(buff, slot->tag_compound, errmsg);
}

slot_t *MCbuffer_unpack_slot(MCbuffer *buff, char **errmsg) {
  short item_id = MCbuffer_unpack_short(buff, errmsg);
  if (item_id < 0)
    return NULL;
  slot_t *slot = MALLOC(sizeof(slot_t));
  slot->item_id = item_id;
  slot->slot_size = MCbuffer_unpack_byte(buff, errmsg);
  slot->meta_data = MCbuffer_unpack_short(buff, errmsg);
  slot->tag_compound = MCbuffer_unpack_nbt(buff, errmsg);
  return slot;
}

void MCbuffer_pack_entity_metadata(MCbuffer *buff, entity_metadata_t metadata,
                                   char **errmsg) {
  for (int i; i < metadata.size; i++) {
    entity_metadata_entry_t *entry =
        metadata.entries + i * sizeof(entity_metadata_entry_t);
    MCbuffer_pack_char(buff, entry->type << 5 | entry->index, errmsg);
    switch (entry->type) {
    case ENTITY_METADATA_ENTRY_TYPE_BYTE:
      MCbuffer_pack_byte(buff, entry->payload.byte_data, errmsg);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_SHORT:
      MCbuffer_pack_short(buff, entry->payload.short_data, errmsg);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_INT:
      MCbuffer_pack_int(buff, entry->payload.int_data, errmsg);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_FLOAT:
      MCbuffer_pack_float(buff, entry->payload.float_data, errmsg);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_STRING:
      MCbuffer_pack_string(buff, entry->payload.string_data, errmsg);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_SLOT:
      MCbuffer_pack_slot(buff, entry->payload.slot_data, errmsg);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_POSITION:
      MCbuffer_pack_int(buff, entry->payload.position_data.x, errmsg);
      MCbuffer_pack_int(buff, entry->payload.position_data.y, errmsg);
      MCbuffer_pack_int(buff, entry->payload.position_data.z, errmsg);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_ROTATION:
      MCbuffer_pack_float(buff, entry->payload.rotation_data.x, errmsg);
      MCbuffer_pack_float(buff, entry->payload.rotation_data.y, errmsg);
      MCbuffer_pack_float(buff, entry->payload.rotation_data.z, errmsg);
      break;
    }
  }
  MCbuffer_pack_byte(buff, 127, errmsg);
}

entity_metadata_t MCbuffer_unpack_entity_metadata(MCbuffer *buff,
                                                  char **errmsg) {
  entity_metadata_t meta_data = {0, NULL};

  while (true) {
    int8_t type_and_index = MCbuffer_unpack_char(buff, errmsg);

    if (type_and_index == 127) {
      break;
    }

    entity_metadata_entry_t meta_data_entry;
    meta_data_entry.type = abs(type_and_index >> 5);
    meta_data_entry.index = type_and_index & 0x1F;

    switch (meta_data_entry.type) {
    case ENTITY_METADATA_ENTRY_TYPE_BYTE:
      meta_data_entry.payload.byte_data = MCbuffer_unpack_byte(buff, errmsg);
      BE2NE_UTIL(meta_data_entry.payload.byte_data);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_SHORT:
      meta_data_entry.payload.short_data = MCbuffer_unpack_short(buff, errmsg);
      BE2NE_UTIL(meta_data_entry.payload.short_data);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_INT:
      meta_data_entry.payload.int_data = MCbuffer_unpack_int(buff, errmsg);
      BE2NE_UTIL(meta_data_entry.payload.int_data);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_FLOAT:
      meta_data_entry.payload.float_data = MCbuffer_unpack_float(buff, errmsg);
      BE2NE_UTIL(meta_data_entry.payload.float_data);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_STRING:
      meta_data_entry.payload.string_data =
          MCbuffer_unpack_string(buff, errmsg);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_SLOT:
      meta_data_entry.payload.slot_data = MCbuffer_unpack_slot(buff, errmsg);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_POSITION:
      meta_data_entry.payload.position_data.x =
          MCbuffer_unpack_int(buff, errmsg);
      meta_data_entry.payload.position_data.y =
          MCbuffer_unpack_int(buff, errmsg);
      meta_data_entry.payload.position_data.z =
          MCbuffer_unpack_int(buff, errmsg);
      break;
    case ENTITY_METADATA_ENTRY_TYPE_ROTATION:
      meta_data_entry.payload.rotation_data.x =
          MCbuffer_unpack_float(buff, errmsg);
      meta_data_entry.payload.rotation_data.y =
          MCbuffer_unpack_float(buff, errmsg);
      meta_data_entry.payload.rotation_data.z =
          MCbuffer_unpack_float(buff, errmsg);
      break;
    }

    entity_metadata_entry_t *new_entries =
        realloc(meta_data.entries,
                (meta_data.size + 1) * sizeof(entity_metadata_entry_t));
    if (new_entries == NULL) {
      // Handle reallocation failure
      free(meta_data.entries);
      *errmsg = "Memory reallocation error";
      meta_data.size = 0;
      meta_data.entries = NULL;
      return meta_data;
    }

    meta_data.entries = new_entries;

    meta_data.entries[meta_data.size] = meta_data_entry;

    meta_data.size++;
  }
  return meta_data;
}
