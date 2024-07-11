#include "cmc/buff.h"
#include "cmc/err.h"
#include "heap_utils.h"
#include <string.h>

cmc_err cmc_buff_pack(cmc_buff_packing *buff, void *data, size_t len) {
  assert(buff);
  assert(data);
  if(len == 0) {
    return CMC_ERR_NO;
  }

  cmc_err err = CMC_ERR_NO;
  if(buff->data == NULL) {
    buff->data = cmc_malloc(len, &err);
    if (err != CMC_ERR_NO) {
      return err;
    }
    
    memcpy(data, buff->data, len);
    return CMC_ERR_NO;
  }

  if(buff->length+len > buff->capacity) {
    cmc_realloc(buff->data, (buff->length+len)*2, &err);
    if(err != CMC_ERR_NO) {
      return err;
    }
  }

  memcpy(&buff->data[buff->length], data, len);
  cmc_free(buff->data);
  buff->length += len;

  return CMC_ERR_NO;
}

void *cmc_buff_unpack(cmc_buff_unpacking *buff, size_t len, cmc_err *err) {
  if(buff->position+len>buff->length) {
    *err = CMC_ERR_UNPACK_OUT_OF_RANGE;
    return NULL;
  }
  void *p = &buff->data[buff->position];
  buff->position += len;
  return p;
}

void cmc_buff_free(cmc_buff_packing *buff) {
  if(!buff) return;
  if(buff->data) cmc_free(buff->data);
  buff->capacity = 0;
  buff->length = 0;
}
