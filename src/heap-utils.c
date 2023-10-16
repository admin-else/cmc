
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void *MALLOC(size_t n) {
  if(n == 0) {
    sprintf(stderr, "Tried to mallocate 0 bytes");
    abort();
  }
  void *p = malloc(n);
  if(p == NULL) {
    sprintf(stderr, "Ran out of memory");
    abort();
  }
  return p;
}

