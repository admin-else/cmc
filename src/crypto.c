/*
this file contains crypto and compression

*/
#include "heap_utils.h"
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/*
// this code is unused but i may use it later when i implement accounts
RSA *pubkeyDER_to_RSA(uint8_t *pubkeyDER, size_t pubkeyDER_len, char **errmsg) {
  unsigned char *malloced_buffer = MALLOC(pubkeyDER_len);
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
}*/

unsigned char *generate_random_bytes(int len) {
  unsigned char *generated_bytes = MALLOC(len);

  for (int i = 0; i < len; i++) {
    generated_bytes[i] = rand() % 0xFF;
  }
  return generated_bytes;
}

char *mc_sha_final(EVP_MD_CTX *mdctx) {
  unsigned char hash[SHA_DIGEST_LENGTH];
  EVP_DigestFinal(mdctx, hash, NULL);

  bool negativ = false;
  if (hash[0] & 0x80) {
    negativ = true;
    for (size_t i = 0; i < SHA_DIGEST_LENGTH; i++) {
      hash[i] = ~hash[i];
    }
    for (int i = SHA_DIGEST_LENGTH - 1; i >= 0; i--) {
      hash[i] += 1;
      if (hash[i] != 0)
        break;
    }
  }

  char result[41];
  for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
    sprintf(&result[i * 2], "%02x", hash[i]);
  }
  int i = 0;
  while (result[i] == '0' && result[i + 1] != '\0')
    i++;

  // Create a new string starting from the first non-zero character
  size_t final_strlen = (41 + negativ) - i;
  char *final_result = MALLOC(final_strlen);
  strcpy(negativ + final_result, &result[i]);
  if (negativ)
    final_result[0] = '-';

  final_result[final_strlen] = '\0';
  return final_result;
}