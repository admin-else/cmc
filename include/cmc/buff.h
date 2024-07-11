#pragma once

#include "cmc/err.h"
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
  size_t capacity;
} cmc_buff_packing;

void *cmc_buff_unpack(cmc_buff_unpacking *buff, size_t len, cmc_err *err);
cmc_err cmc_buff_pack(cmc_buff_packing *buff, void *data, size_t len);
void cmc_buff_free(cmc_buff_packing *buff);
