#include "MCbuffer.h"
#include "MCtypes.h"
#include "Nbt.h"
#include "heap-utils.h"
#include "textcolor.h"
#include <jansson.h>
#include <stdbool.h>
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
    printf("\\x%02x", buff->data[i]);
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
  if (readData == NULL) {
    *errmsg = "Failed to allocate memory for readData";
    return NULL;
  }

  if (n > 0) {
    memcpy(readData, buffer->data + buffer->position, n);
  }

  buffer->position += n;
  return readData;
}

// C types

#define DATA_PACK_AND_UNPACK_FUNC_FACTORY(name, type)                          \
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

DATA_PACK_AND_UNPACK_FUNC_FACTORY(char, char);
DATA_PACK_AND_UNPACK_FUNC_FACTORY(byte, unsigned char);
DATA_PACK_AND_UNPACK_FUNC_FACTORY(short, short);
DATA_PACK_AND_UNPACK_FUNC_FACTORY(ushort, unsigned short);
DATA_PACK_AND_UNPACK_FUNC_FACTORY(int, int);
DATA_PACK_AND_UNPACK_FUNC_FACTORY(uint, unsigned int);
DATA_PACK_AND_UNPACK_FUNC_FACTORY(long, long);
DATA_PACK_AND_UNPACK_FUNC_FACTORY(ulong, unsigned long);
DATA_PACK_AND_UNPACK_FUNC_FACTORY(float, float);
DATA_PACK_AND_UNPACK_FUNC_FACTORY(double, double);
DATA_PACK_AND_UNPACK_FUNC_FACTORY(llong, long long);
DATA_PACK_AND_UNPACK_FUNC_FACTORY(ullong, unsigned long long);

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
  if (str_len > max_len) {
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
  return (char *)MCbuffer_unpack(buff, str_len, errmsg);
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

// Todo we also need a MCbuffer_pack_nbt yay more Strg+C and V from cNBT YAAAYYY

json_t *MCbuffer_unpack_json(MCbuffer *buff, char **errmsg) {
  json_t *root = NULL;
  json_error_t error;

  char *str = MCbuffer_unpack_string(buff, errmsg);
  root = json_loads(str, 0, &error);
  FREE(str);
  if (!root) {
    sprintf(*errmsg, "Error while reading json: %s", error.text);
    return NULL;
  }

  return root;
}

void MCbuffer_pack_json(MCbuffer *buff, json_t *json, char **errmsg) {
  char *jsonString = json_dumps(json, 0);
  MCbuffer_pack_string(buff, jsonString, errmsg);
  FREE(jsonString);
}

void MCbuffer_pack_byte_array(MCbuffer *buff, MCbuffer *byte_array, char **errmsg) {
  MCbuffer_pack_varint(buff, byte_array->length, errmsg);
  MCbuffer_pack(buff, byte_array->data, byte_array->length, errmsg);
}

MCbuffer *MCbuffer_unpack_byte_array(MCbuffer *buff, char **errmsg) {
  int ret_buff_len = MCbuffer_unpack_varint(buff, errmsg);
  MCbuffer *ret = MCbuffer_init();
  ret->capacity = ret_buff_len;
  ret->length   = ret_buff_len;
  ret->data     = MCbuffer_unpack(buff, ret_buff_len, errmsg);
  return ret;
}