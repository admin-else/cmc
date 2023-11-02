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

#pragma once
#include <stddef.h>

#define TEXT_BUFFER_INIT                                                            \
  (text_buffer) { NULL, 0, 0 }

typedef struct{
  unsigned char
      *data;  /* You can access the buffer's raw bytes through this pointer */
  size_t len; /* Only accesses in the interval [data, data + len) are defined */
  size_t cap; /* Internal use. The allocated size of the buffer. */
} text_buffer;

int text_buffer_lazy_init(text_buffer *b);

void text_buffer_free(text_buffer *b);

int text_buffer_reserve(text_buffer *b, size_t reserved_amount);

int text_buffer_append(text_buffer *b, const void *data, size_t n);

void text_buffer_bprintf(text_buffer *b, const char *format, ...);

void text_buffer_indent(text_buffer *b, size_t amount);