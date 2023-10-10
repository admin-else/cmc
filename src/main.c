#include "MCbuffer.h"
#include "MConn.h"
#include "MCtypes.h"
#include "textcolor.h"
#include <ctype.h>
#include <curses.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void MCbuffer_py_print(MCbuffer *buff) {
  for (int i = 0; i < buff->length; i++) {
    if (isprint(buff->data[i])) {
      printf("%c", buff->data[i]);
    } else {
      printf("\\x%02X", buff->data[i]);
    }
  }
  printf("\n");
}

void MCbuffer_print_all_info(MCbuffer *buff) {
  printf("Lenght: " TEXT_BOLD "%zu" NLR, buff->length);
  printf("Position: " TEXT_BOLD "%zu" NLR, buff->position);
  printf("Capacity: " TEXT_BOLD "%zu" NLR, buff->capacity);
  MCbuffer_py_print(buff);
}

int main() {
  char *errmsg = NULL;
  MCblockPos pos;
  pos.x = 10000000;
  pos.y = 444;
  pos.z = 100;
  MCbuffer *buff = MCbuffer_init();
  MCbuffer_pack_position(buff, pos, &errmsg);
  buff->position = 0;
  MCblockPos pos2 = MCbuffer_unpack_position(buff, &errmsg);
  MCbuffer_free(buff);

  if(errmsg != NULL) {
    printf("Error while unpacking %s\n", errmsg);
  }

  printf("%ld %ld %ld\n", pos2.x, pos2.y, pos2.z);

  return 0;
}