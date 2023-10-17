#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void *MALLOC(size_t n) {
  if (n == 0) {
    fprintf(stderr, "Tried to mallocate 0 bytes");
    abort();
  }
  void *p = malloc(n);
  if (p == NULL) {
    fprintf(stderr, "Ran out of memory");
    abort();
  }

  //printf("MALLOC %p (%li)\n", p, n);

  return p;
}

void FREE(void *p) {
  //printf("FREE   %p\n", p);
  free(p);
}