#pragma once

#include <stdint.h>
#include <stddef.h>

// used in unpack functions
typedef struct {
  uint8_t *data;
  size_t length;
  size_t position;
} cmc_buff_unpacking;

// used in pack functions
typedef struct {
  uint8_t *data;
  size_t length;
} cmc_buff_packing;
