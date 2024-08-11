#pragma once
#include "cmc/buff.h"

#define CMC_PSTRING_MAX INT16_MAX

// I still have no idea what the p stands for
// but prismarine calls it that so ok ig
typedef char *cmc_pstring;

cmc_err cmc_pstring_pack(cmc_buff *buff, cmc_pstring str, size_t len);

cmc_pstring cmc_pstring_unpack(cmc_span *buff, size_t len, cmc_err *err);
