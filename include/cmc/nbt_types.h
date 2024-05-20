/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <phk@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Poul-Henning Kamp
 * ----------------------------------------------------------------------------
 */

/*
 * NOTICE: Some or all functions may be modifyed or copied from this repo
 * https://github.dev/chmod222/cNBT.
 */
#pragma once

#include <stdint.h>

typedef enum {
  CMC_NBT_TAG_END = 0,
  CMC_NBT_TAG_BYTE = 1,
  CMC_NBT_TAG_SHORT = 2,
  CMC_NBT_TAG_INT = 3,
  CMC_NBT_TAG_LONG = 4,
  CMC_NBT_TAG_FLOAT = 5,
  CMC_NBT_TAG_DOUBLE = 6,
  CMC_NBT_TAG_BYTE_ARRAY = 7,
  CMC_NBT_TAG_STRING = 8,
  CMC_NBT_TAG_LIST = 9,
  CMC_NBT_TAG_COMPOUND = 10,
  CMC_NBT_TAG_INT_ARRAY = 11,
  CMC_NBT_TAG_LONG_ARRAY = 12
} cmc_nbt_type;

typedef struct {
  unsigned char *data;
  int32_t length;
} cmc_nbt_byte_array;

typedef struct {
  int32_t *data;
  int32_t length;
} cmc_nbt_int_array;

typedef struct cmc_nbt_long_array {
  int64_t *data;
  int32_t length;
} cmc_nbt_long_array;

typedef struct {
  struct cmc_nbt **data;
  int32_t length;
} cmc_nbt_compound;

struct cmc_nbt_list;

struct cmc_nbt_list {
  cmc_nbt_type type;
  int32_t length;
  union {
    int8_t *tag_byte;
    int16_t *tag_short;
    int32_t *tag_int;
    int64_t *tag_long;
    float *tag_float;
    double *tag_double;
    cmc_nbt_byte_array *tag_byte_array;
    cmc_nbt_int_array *tag_int_array;
    cmc_nbt_long_array *tag_long_array;
    char **tag_string;
    cmc_nbt_compound *tag_compound;
  } payload;
};

typedef struct cmc_nbt_list cmc_nbt_list;

typedef struct cmc_nbt {
  cmc_nbt_type type;
  char *name;

  union {
    int8_t tag_byte;
    int16_t tag_short;
    int32_t tag_int;
    int64_t tag_long;
    float tag_float;
    double tag_double;
    cmc_nbt_byte_array tag_byte_array;
    cmc_nbt_int_array tag_int_array;
    cmc_nbt_long_array tag_long_array;
    char *tag_string;
    cmc_nbt_compound tag_compound;
    cmc_nbt_list tag_list;
  } payload;
} cmc_nbt;
