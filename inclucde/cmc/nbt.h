/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <phk@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Poul-Henning Kamp
 * ----------------------------------------------------------------------------
 */

/*
 * NOTICE: Some or all functions may be modifyed or copied from this repo
 * https://github.dev/chmod222/cNBT.
 */

#pragma once

#include <cmc/buffer.h>
#include <cmc/list.h>
#include <cmc/nbt_types.h>
#include <stdint.h>

cmc_nbt *nbt_parse_named_tag(cmc_buffer *buff);

#define free_nbt nbt_free
void nbt_free(cmc_nbt *tree);

char *nbt_dump_ascii(const cmc_nbt *tree);

cmc_buffer *nbt_dump_binary(const cmc_nbt *tree);