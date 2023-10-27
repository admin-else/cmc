#pragma once

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <stddef.h>
#include "MCtypes.h"

RSA *pubkeyDER_to_RSA(uint8_t *pubkeyDER, size_t pubkeyDER_len, char **errmsg);

byte_t *generate_random_bytes(int len);

char *mc_sha_final(SHA_CTX *sha1);