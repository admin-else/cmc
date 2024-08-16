#pragma once



#include<cmc/type/varnums.h>
#include<cmc/err_macros.h>
#include<cmc/err.h>

typedef cmc_varint cmc_765_optvarint;

cmc_765_optvarint cmc_765_optvarint_unpack(cmc_span *span, cmc_err *err);

cmc_err cmc_765_optvarint_pack(cmc_buff *buff, cmc_765_optvarint data);