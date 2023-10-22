#pragma once

#include <stdint.h>
#include "MCbuffer.h"
#include "list.h"
#include "MCtypes.h"

nbt_node *nbt_parse_named_tag(MCbuffer *buff, char **errmsg);

void nbt_free(nbt_node *tree);

char *nbt_dump_ascii(const nbt_node *tree, char **errmsg);