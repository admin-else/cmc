/* 
this file contains crypto and comprssion

*/
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <stddef.h>

RSA *pubkeyDER_to_RSA(uint8_t *pubkeyDER, size_t pubkeyDER_len, char **errmsg) {
  BIO *pubkeyBIO = BIO_new_mem_buf(pubkeyDER, pubkeyDER_len);

  if (pubkeyBIO == NULL) {
    *errmsg = "Failed to create BIO structure";
    free(pubkeyDER);
    return NULL;
  }

  EVP_PKEY *pubkeyEVP = d2i_PUBKEY_bio(pubkeyBIO, NULL);

  if (pubkeyEVP == NULL) {
    *errmsg = "Failed to read public key";
    free(pubkeyDER);
    BIO_free(pubkeyBIO);
    return NULL;
  }

  RSA *pubkeyRSA = EVP_PKEY_get1_RSA(pubkeyEVP);
  free(pubkeyDER);
  BIO_free(pubkeyBIO);
  EVP_PKEY_free(pubkeyEVP);
  return pubkeyRSA;
}