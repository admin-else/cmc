#pragma once











#include<cmc/err_macros.h>
#include<cmc/type/c_types.h>
#include<cmc/err.h>

typedef struct {

float x;

float y;

float z;

float w;

} cmc_765_vec4f;

cmc_765_vec4f cmc_765_vec4f_unpack(cmc_span *span, cmc_err *err);

cmc_err cmc_765_vec4f_pack(cmc_buff *buff, cmc_765_vec4f data);