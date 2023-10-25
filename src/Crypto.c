/* 
this file contains crypto and comprssion

*/
#include "MCtypes.h"
#include "heap-utils.h"
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <stddef.h>
#include <string.h>

RSA *pubkeyDER_to_RSA(uint8_t *pubkeyDER, size_t pubkeyDER_len, char **errmsg) {
  byte_t *malloced_buffer = MALLOC(pubkeyDER_len);
  memcpy(malloced_buffer, pubkeyDER, pubkeyDER_len);
  
  BIO *pubkeyBIO = BIO_new_mem_buf(malloced_buffer, pubkeyDER_len);

  if (pubkeyBIO == NULL) {
    *errmsg = "Failed to create BIO structure";
    free(malloced_buffer);
    return NULL;
  }

  EVP_PKEY *pubkeyEVP = d2i_PUBKEY_bio(pubkeyBIO, NULL);

  if (pubkeyEVP == NULL) {
    *errmsg = "Failed to read public key";
    free(malloced_buffer);
    BIO_free(pubkeyBIO);
    return NULL;
  }

  RSA *pubkeyRSA = EVP_PKEY_get1_RSA(pubkeyEVP);
  free(malloced_buffer);
  BIO_free(pubkeyBIO);
  EVP_PKEY_free(pubkeyEVP);
  return pubkeyRSA;
}

byte_t *generate_random_bytes(int len) {
  byte_t *generated_bytes = malloc(len);

  for (int i = 0; i < len; i++) {
    generated_bytes[i] = rand() % 256;
  }
  return generated_bytes;
}