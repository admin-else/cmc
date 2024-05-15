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

#include <cmc/buff.h>
#include <cmc/list.h>
#include <cmc/nbt_types.h>

cmc_nbt *cmc_nbt_parse(cmc_buff *buff, cmc_err_extra *err);
cmc_buff *cmc_nbt_text(const cmc_nbt *tree, cmc_err_extra *err);
cmc_buff *cmc_nbt_dump(const cmc_nbt *tree, cmc_err_extra *err);
void cmc_nbt_free(cmc_nbt *tree, cmc_err_extra *err);