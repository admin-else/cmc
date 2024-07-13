#include "cmc/buff.h"
#include "cmc/err.h"

cmc_err cmc_pack_varint(cmc_buff_packing *buff, int32_t value) {
  for(int i = 0; i < 5; ++i) {
    value >> 7*i
  }
}
