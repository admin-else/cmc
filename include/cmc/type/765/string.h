#pragma once






#include<cmc/type/pstring.h>
#include<cmc/err_macros.h>
#include<string.h>
#include<cmc/type/varnums.h>
#include<cmc/err.h>

typedef cmc_pstring cmc_765_string;

cmc_765_string cmc_765_string_unpack(cmc_span *span, cmc_err *err);

cmc_err cmc_765_string_pack(cmc_buff *buff, cmc_765_string data);