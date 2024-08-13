#pragma once
#include <cmc/err.h>

#define CMC_ERR_ABLE(code, action) code; if(err != CMC_ERR_NO) {action;}
#define CMC_ERRP_ABLE(code, action) code; if(*err != CMC_ERR_NO) {action;}
