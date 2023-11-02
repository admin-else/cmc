#include "Crypto.h"
#include "MCtypes.h"
#include "heap-utils.h"
#include "TextBuffer.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include <openssl/sha.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SESSION_SERVER_URL "https://sessionserver.mojang.com/session/minecraft/"

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    char **output = (char **)userp;
    *output = (char *)realloc(*output, total_size + 1);
    if (*output) {
        memcpy(*output, contents, total_size);
        (*output)[total_size] = '\0'; // Null-terminate the string
    }
    return total_size;
}

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
  
  /*
  hnd = curl_easy_init();
  curl_easy_setopt(hnd, CURLOPT_URL,
                   "https://sessionserver.mojang.com/session/minecraft/join");
  curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(
      hnd, CURLOPT_POSTFIELDS,
      "{\"accessToken\":\"eyJraWQiOiJhYzg0YSIsImFsZyI6IkhTMjU2In0."
      "eyJ4dWlkIjoiMjUzNTQwODczNjU4Mzg2NyIsImFnZyI6IkFkdWx0Iiwic3ViIjoiZmFjYWQ2"
      "NjYtNGFiYS00MDU3LTlmOWQtMzQ5NDI2NDQ2MzAzIiwiYXV0aCI6IlhCT1giLCJucyI6ImRl"
      "ZmF1bHQiLCJyb2xlcyI6W10sImlzcyI6ImF1dGhlbnRpY2F0aW9uIiwiZmxhZ3MiOlsidHdv"
      "ZmFjdG9yYXV0aCIsIm1pbmVjcmFmdF9uZXQiLCJtc2FtaWdyYXRpb25fc3RhZ2U0Iiwib3Jk"
      "ZXJzXzIwMjIiXSwicHJvZmlsZXMiOnsibWMiOiIzNjMyMzMwZC0zNzM3LTQyNzAtOGU4Zi0y"
      "NzBlNTgxYzQ1ZGIifSwicGxhdGZvcm0iOiJVTktOT1dOIiwieXVpZCI6ImRhYjRkNGQyZjA1"
      "NzQ3YWNmMTg5YTFlNTE1MDM5ZGNkIiwibmJmIjoxNjk4NzQ4MjMwLCJleHAiOjE2OTg4MzQ2"
      "MzAsImlhdCI6MTY5ODc0ODIzMH0."
      "QEpbBZxhr7n10RLIFJR0T6px8xzOkhD1vgK2hHKsQjs\",\"selectedProfile\":"
      "\"3632330d373742708e8f270e581c45db\",\"serverId\":"
      "\"4e56c18230d06195be075166dae85db285e688ea\"}");
  curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)740);
  curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
  curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
  */

  // Post 
  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_string);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, json_length);
  
  // Headers
  struct curl_slist *headers = NULL;
  headers = curl_slist_append(headers, "Content-Type: application/json");
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

  // Get data back
  char *received_data = NULL;
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &received_data);
  
  
  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    curl_easy_cleanup(curl);
    *errmsg = "curl failed sending request to join minecraft server";
    return -1;
  }
  printf("bruh %s \n", received_data);
  long http_code;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
  curl_easy_cleanup(curl);
  printf("http code %li\n", http_code);
  if (http_code == 204)
    return 0;
  return 1;
}