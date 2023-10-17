#include "FileUtils.h"
#include "MCbuffer.h"
#include "MConn.h"
#include "MCtypes.h"
#include "Nbt.h"
#include "heap-utils.h"
#include "textcolor.h"
#include <curses.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_IF_ERR(message)                                                   \
  if (errmsg != NULL) {                                                        \
    fprintf(stderr, message, errmsg);                                          \
    exit(1);                                                                   \
  }

void print_bytes_hex(unsigned char *bytes, size_t len) {
  for (int i = 0; i < len; i++) {
    printf("%02X ", bytes[i]);
  }
  printf("\n");
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  if (size == 0)
    return 0;


  char *errmsg = NULL;
  MCbuffer *buff = MCbuffer_init();
  buff->data = MALLOC(size);
  memcpy(buff->data, data, size);
  buff->length = size;
  buff->capacity = size;

  nbt_node *nbt = nbt_parse_named_tag(buff, &errmsg);
  nbt_free(nbt);
  MCbuffer_free(buff);
  if (errmsg != NULL)
    fprintf(stderr, "Error: \n%s", errmsg);
  return 0;
}

int main1() {
  char *errmsg = NULL;

  size_t file_size;
  byte_t *nbt_file =
      read_binary_file("example-data/bigtest.nbt", &file_size, &errmsg);

  EXIT_IF_ERR("Error while readig file: %s\n")

  MCbuffer *buff = MCbuffer_init();
  buff->data = nbt_file;
  buff->capacity = file_size;
  buff->length = file_size;

  puts("parsing nbt");

  nbt_node *nbt = nbt_parse_named_tag(buff, &errmsg);
  if (errmsg != NULL) {
    fprintf(stderr, "Error while Parsing \n%s\n", errmsg);
    exit(1);
  }

  puts("dumping");

  char *stuff = nbt_dump_ascii(nbt, &errmsg);
  EXIT_IF_ERR("%s\n")

  printf("%s", stuff);
  FREE(stuff);

  nbt_free(nbt);
  EXIT_IF_ERR("%s\n")

  MCbuffer_free(buff);
  EXIT_IF_ERR("%s\n")

  return 0;
}