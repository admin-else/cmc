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

#include "list.h"
#include "mcbuffer.h"
#include "mctypes.h"
#include <stdint.h>

nbt_node *nbt_parse_named_tag(MCbuffer *buff);

void nbt_free(nbt_node *tree);

char *nbt_dump_ascii(const nbt_node *tree);

MCbuffer *nbt_dump_binary(const nbt_node *tree);