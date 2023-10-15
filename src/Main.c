#include "MCbuffer.h"
#include "MConn.h"
#include "MCtypes.h"
#include "textcolor.h"
#include <curses.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "FileUtils.h"
#include "Nbt.h"

#define EXIT_IF_ERR(message) if(errmsg != NULL) {fprintf(stderr, message, errmsg); exit(1);}

void print_bytes_hex(unsigned char *bytes, size_t len) {
    for (int i = 0; i < len; i++) {
        printf("%02X ", bytes[i]);
    }
    printf("\n");
}

void MCbuffer_unpack_nbt(MCbuffer *buff, char **errmsg) {
  nbt_parse_named_tag(buff, errmsg);
}

int main() {
  char *errmsg = NULL;

  size_t file_size;
  byte_t *nbt_file = read_binary_file("example-data/hello_world.nbt", &file_size, &errmsg);
  EXIT_IF_ERR("Error while readig file: %s\n")

  MCbuffer *buff = MCbuffer_init();
  buff->data = nbt_file;
  buff->length = file_size;
  buff->length = file_size;
  
  MCbuffer_unpack_nbt(buff, &errmsg);
  EXIT_IF_ERR("%s\n")

  return 0;
}