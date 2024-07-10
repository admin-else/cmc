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

    

  return CMC_ERR_NO;
}
