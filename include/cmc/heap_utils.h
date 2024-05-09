#pragma once

#include <cmc/conn.h>
#include <cmc/err.h>
#include <stddef.h>

void *cmc_malloc(size_t n, cmc_err_extra *err);

void *cmc_realloc(void *p, size_t n, cmc_err_extra *err);