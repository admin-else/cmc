#pragma once

#include "MCtypes.h"


int join_server(char *access_token, char *selected_profile, char *server_id,
                byte_t *shared_secret, byte_t *public_key, size_t public_key_len, char **errmsg);