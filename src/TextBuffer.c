/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <phk@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Poul-Henning Kamp
 * ----------------------------------------------------------------------------
 */

/*
 * NOTICE: Some functions may be modifyed or copied from this repo https://github.dev/chmod222/cNBT.
 */

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TextBuffer.h"

#ifdef __GNUC__
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect((x), 0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

int text_buffer_lazy_init(text_buffer *b) {
  assert(b->data == NULL);

  size_t cap = 1024;

  *b = (text_buffer){.data = malloc(cap), .len = 0, .cap = cap};

  if (unlikely(b->data == NULL))
    return 1;

  return 0;
}

void text_buffer_free(text_buffer *b) {
  assert(b);

  free(b->data);

  b->data = NULL;
  b->len = 0;
  b->cap = 0;
}

int text_buffer_reserve(text_buffer *b, size_t reserved_amount) {
  assert(b);

  if (unlikely(b->data == NULL) && unlikely(text_buffer_lazy_init(b)))
    return 1;

  if (likely(b->cap >= reserved_amount))
    return 0;

  while (b->cap < reserved_amount)
    b->cap *= 2;

  unsigned char *temp = realloc(b->data, b->cap);

  if (unlikely(temp == NULL))
    return text_buffer_free(b), 1;

  b->data = temp;

  return 0;
}

int text_buffer_append(text_buffer *b, const void *data, size_t n) {
  assert(b);

  if (unlikely(b->data == NULL) && unlikely(text_buffer_lazy_init(b)))
    return 1;

  if (unlikely(text_buffer_reserve(b, b->len + n)))
    return 1;

  memcpy(b->data + b->len, data, n);
  b->len += n;

  return 0;
}

void text_buffer_bprintf(text_buffer *b, const char *format, ...) {
  va_list args;
  int siz;

  va_start(args, format);
  siz = vsnprintf(NULL, 0, format, args);
  va_end(args);

  text_buffer_reserve(b, b->len + siz + 1);

  va_start(args, format);
  vsnprintf((char *)(b->data + b->len), siz + 1, format, args);
  va_end(args);

  b->len += siz; // remember - no null terminator!
}

void text_buffer_indent(text_buffer *b, size_t amount) {
  size_t spaces = amount * 4; /* 4 spaces per indent */

  char temp[spaces + 1];

  for (size_t i = 0; i < spaces; ++i)
    temp[i] = ' ';
  temp[spaces] = '\0';

  text_buffer_bprintf(b, "%s", temp);
}