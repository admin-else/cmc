#pragma once

#include "cmc/buff.h"
#include <stdint.h>

typedef int32_t cmc_varint;
typedef int64_t cmc_varlong;

cmc_err cmc_varint_pack(cmc_buff *buff, cmc_varint number);
cmc_varint cmc_varint_unpack(cmc_span *buff, cmc_err *err);
cmc_err cmc_varlong_pack(cmc_buff *buff, cmc_varlong number);
cmc_varlong cmc_varlong_unpack(cmc_span *buff, cmc_err *err);
