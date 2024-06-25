#pragma once

#include <cmc/buff.h>
#include <cmc/err.h>

#include <stddef.h>

void *cmc_malloc(size_t n, cmc_err_extra *err);

void *cmc_realloc(void *p, size_t n, cmc_err_extra *err);

void *cmc_malloc_packet_array(cmc_buff *buff, size_t size);
