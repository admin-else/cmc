#include "cmc/buff.h"
#include "cmc/err.h"
#include "cmc/err_macros.h"
#include "heap_utils.h"
#include <string.h>

cmc_err cmc_pack(cmc_buff *buff, void *data, size_t len) {
  assert(buff);
  assert(data);
  if (len == 0) {
    return CMC_ERR_NO;
  }

  cmc_err err = CMC_ERR_NO;
  if (buff->length + len > buff->capacity) {
    buff->capacity = 2 * (buff->length + len);
    buff->data = CMC_ERR_ABLE(cmc_realloc(buff->data, buff->capacity, &err), return err;);
  }

  memcpy(&buff->data[buff->length], data, len);
  buff->length += len;

  return CMC_ERR_NO;
}

void *cmc_unpack(cmc_span *buff, size_t len, cmc_err *err) {
  if (buff->position + len > buff->length) {
    *err = CMC_ERR_UNPACK_OUT_OF_RANGE;
    return NULL;
  }
  void *p_old = &buff->data[buff->position];
  buff->position += len;
  return p_old;
}

void cmc_buff_free(cmc_buff *buff) {
  if (!buff) {
    return;
  }
  if (buff->data) {
    cmc_free(buff->data);
  }
  buff->capacity = 0;
  buff->length = 0;
}
