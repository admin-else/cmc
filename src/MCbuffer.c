#include "MCbuffer.h"
#include "MCtypes.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VARINT_SEGMENT_BITS 0x7F
#define VARINT_CONTINUE_BIT 0x80
#define DEFAULT_MAX_STRING_LENGTH INT16_MAX
#define X_MASK 0x3FFFFFF
#define X_SHIFT 38
#define Y_MASK 0xFFF
#define Y_SHIFT 26
#define Z_MASK 0x3FFFFFF
#define Z_SHIFT 0

#define CHECK_ERRMSG                                                           \
  if (*errmsg != NULL)                                                         \
    return 0;

MCbuffer *MCbuffer_init() {
  MCbuffer *buffer = malloc(sizeof(MCbuffer));
  buffer->data = NULL;
  buffer->capacity = 0;
  buffer->length = 0;
  buffer->position = 0;
  return buffer;
}

void MCbuffer_free(MCbuffer *buffer) {
  if (buffer->capacity) {
    free(buffer->data);
  }
  buffer->data = NULL;
  buffer->capacity = 0;
  buffer->length = 0;
  buffer->position = 0;
  free(buffer);
}

void MCbuffer_pack(MCbuffer *buffer, const void *data, size_t dataSize,
                   char **errmsg) {
  if (buffer->position + dataSize > buffer->length) {
    size_t newCapacity = buffer->position + dataSize;
    if (newCapacity > buffer->capacity) {
      unsigned char *newData =
          (unsigned char *)realloc(buffer->data, newCapacity);
      if (newData == NULL) {
        *errmsg = "Failed to allocate memory for buffer";
        return;
      }
      buffer->data = newData;
      buffer->capacity = newCapacity;
    }
    buffer->length = buffer->position + dataSize;
  }

  if (buffer->data == NULL) {
    memmove(buffer->data + buffer->position + dataSize,
            buffer->data + buffer->position, buffer->length - buffer->position);
  }
  memcpy(buffer->data + buffer->position, data, dataSize);
  buffer->position += dataSize;
}

unsigned char *MCbuffer_unpack(MCbuffer *buffer, size_t n, char **errmsg) {
  if (buffer->position + n > buffer->length) {
    *errmsg = "Read position exceeds buffer length";
    return NULL;
  }

  unsigned char *readData = (unsigned char *)malloc(n);
  if (readData == NULL) {
    *errmsg = "Failed to allocate memory for readData";
    return NULL;
  }

  memcpy(readData, buffer->data + buffer->position, n);
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
    free(data);                                                                \
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
  free(data);
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
    number |= (b & 0x7F) << (7 * i);
    if (!(b & 0x80))
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
            "The received encoded string buffer length is longer than maximum "
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
  block_pos_t pos;

  if (*errmsg != NULL)
    return pos;
  pos.x = val >> 38;
  pos.y = (val >> 26) & 0xFFF;
  pos.z = val << 38 >> 38;

  // this is for negatives
  if (pos.x >= 33554432) pos.x -= 67108864;
  if (pos.y >= 2048    ) pos.y -=     4096;
  if (pos.z >= 33554432) pos.z -= 67108864;

  return pos;
}