#pragma once

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <stddef.h>

RSA *pubkeyDER_to_RSA(uint8_t *pubkeyDER, size_t pubkeyDER_len, char **errmsg);