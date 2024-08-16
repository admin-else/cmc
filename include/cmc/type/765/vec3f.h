#pragma once









#include<cmc/err_macros.h>
#include<cmc/type/c_types.h>
#include<cmc/err.h>

typedef struct {

float x;

float y;

float z;

} cmc_765_vec3f;

cmc_765_vec3f cmc_765_vec3f_unpack(cmc_span *span, cmc_err *err);

cmc_err cmc_765_vec3f_pack(cmc_buff *buff, cmc_765_vec3f data);