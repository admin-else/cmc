#pragma once

#include "cmc/err.h"
#include <stddef.h>
#include <stdint.h>

// used in unpack functions
typedef struct {
  uint8_t *data;
  size_t length;
  size_t position;
} cmc_span;

// used in pack functions
typedef struct {
  uint8_t *data;
  size_t length;
  size_t capacity;
} cmc_buff;

static inline cmc_span cmc_span_of(cmc_buff const *buff) {
  return (cmc_span){buff->data, buff->length};
}

void *cmc_unpack(cmc_span *buff, size_t len, cmc_err *err);
cmc_err cmc_pack(cmc_buff *buff, void *data, size_t len);
void cmc_buff_free(cmc_buff *buff);
