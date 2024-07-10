#include "cmc/buff.h"
#include "cmc/err.h"
#include "heap_utils.h"
#include <string.h>

cmc_err cmc_buff_pack(cmc_buff_packing *buff, uint8_t *data, size_t len) {
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

  uint8_t *tmp = cmc_malloc(buff->length + len, &err);
  if(err != CMC_ERR_NO) {
    return err;
  }

  memcpy(tmp, buff->data, buff->length);
  memcpy(&tmp[buff->length], data, len);
  cmc_free(buff->data);
  buff->data = tmp;
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
