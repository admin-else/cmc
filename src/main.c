#include "MCbuffer.h"
#include "MConn.h"
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

uint64_t pack_position(uint32_t x, uint32_t y, uint32_t z) {
    uint64_t result = ((uint64_t)(x & 0x3FFFFFF) << 38) | ((uint64_t)(z & 0x3FFFFFF) << 12) | (y & 0xFFF);
    return result;
}


int main() {
  uint64_t packed_pos = pack_position(18357644, -20882616, 831);
  printf("%lu", packed_pos);
  return 0;
}