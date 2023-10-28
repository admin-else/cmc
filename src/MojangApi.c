#include "Crypto.h"
#include "MCtypes.h"
#include "heap-utils.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include <openssl/sha.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define SESSION_SERVER_URL "https://sessionserver.mojang.com/session/minecraft/"

// server id is always just an emtpy string but its just here so we
// dont have to update this function if we want to back track to 1.7
int join_server(char *access_token, char *selected_profile, char *server_id,
                byte_t *shared_secret, byte_t *public_key,
                size_t public_key_len, char **errmsg) {
  SHA_CTX sha1;
  SHA1_Init(&sha1);
  SHA1_Update(&sha1, server_id, strlen(server_id));
  SHA1_Update(&sha1, shared_secret, 16);
  SHA1_Update(&sha1, public_key, public_key_len);
  char *digest = mc_sha_final(&sha1);

  size_t json_length =
      strlen(access_token) + strlen(selected_profile) + strlen(digest) + 64;

  char json_string[json_length];
  snprintf(
      json_string, json_length,
      "{\"accessToken\":\"%s\",\"selectedProfile\":\"%s\",\"serverId\":\"%s\"}",
      access_token, selected_profile, digest);
  FREE(digest);
  printf("%s\n",json_string);

  CURL *curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, SESSION_SERVER_URL "join");
  curl_easy_setopt(curl, CURLOPT_POST, true);
  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_string);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, json_length);
  struct curl_slist *headers = NULL;
  headers = curl_slist_append(headers, "Content-Type: application/json");
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    curl_easy_cleanup(curl);
    *errmsg = "curl failed sending request to join minecraft server";
    return -1;
  }
  long http_code;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
  curl_easy_cleanup(curl);
  printf("http code %li\n", http_code);
  if (http_code == 204)
    return 0;
  return 1;
}