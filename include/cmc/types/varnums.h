#pragma once

#include "cmc/buff.h"
#include <stdint.h>

typedef int32_t cmc_varint;
typedef int64_t cmc_varlong;

cmc_err cmc_pack_varint(cmc_buff_packing *buff, cmc_varint number);
cmc_varint cmc_unpack_varint(cmc_buff_unpacking *buff, cmc_err *err);
cmc_err cmc_pack_varlong(cmc_buff_packing *buff, cmc_varlong number);
cmc_varlong cmc_unpack_varlong(cmc_buff_unpacking *buff, cmc_err *err);
