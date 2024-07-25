#pragma once

#include "cmc/err.h"

void *cmc_malloc(unsigned long len, cmc_err *err);
void *cmc_realloc(void *in_p, unsigned long len, cmc_err *err);
void cmc_free(void *p);
