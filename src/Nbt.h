/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <phk@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Poul-Henning Kamp
 * ----------------------------------------------------------------------------
 */

/*
 * NOTICE: Some functions may be modifyed or copied from this repo https://github.dev/chmod222/cNBT.
 */

#pragma once

#include <stdint.h>
#include "MCbuffer.h"
#include "list.h"
#include "MCtypes.h"

nbt_node *nbt_parse_named_tag(MCbuffer *buff, char **errmsg);

void nbt_free(nbt_node *tree);

char *nbt_dump_ascii(const nbt_node *tree, char **errmsg);