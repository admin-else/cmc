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
#include <cmc/list.h>
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

    struct cmc_nbt_byte_array {
      unsigned char *data;
      int32_t length;
    } tag_byte_array;

    struct cmc_nbt_int_array {
      int32_t *data;
      int32_t length;
    } tag_int_array;

    struct cmc_nbt_long_array {
      int64_t *data;
      int32_t length;
    } tag_long_array;

    char *tag_string;

    struct cmc_nbt_list {
      struct cmc_nbt *data;
      struct list_head entry;
    } *tag_list, *tag_compound;
  } payload;
} cmc_nbt;