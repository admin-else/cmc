#include "file_utils.h"
#include "heap_utils.h"
#include "mcbuffer.h"
#include "mctypes.h"
#include <stdlib.h>

char *read_text_file(const char *filename, char **errmsg) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    *errmsg = "Error: Unable to open file for reading.";
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  rewind(file);

  char *buffer = (char *)MALLOC(file_size + 1);
  if (buffer == NULL) {
    fclose(file);
    *errmsg = "Error: Memory allocation failed.";
    return NULL;
  }

  size_t bytes_read = fread(buffer, 1, file_size, file);
  buffer[bytes_read] = '\0';

  fclose(file);
  return buffer;
}

unsigned char *read_binary_file(const char *filename, size_t *length, char **errmsg) {
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    *errmsg = "Error: Unable to open binary file for reading.";
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  rewind(file);

  unsigned char *buffer = (unsigned char *)MALLOC(file_size);
  if (buffer == NULL) {
    fclose(file);
    *errmsg = "Error: Memory allocation failed.";
    return NULL;
  }

  size_t items_read =
      fread(buffer, sizeof(unsigned char), file_size / sizeof(unsigned char), file);
  fclose(file);

  if (length != NULL) {
    *length = items_read * sizeof(unsigned char);
  }

  return buffer;
}

int write_text_file(const char *filename, const char *text, char **errmsg) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    *errmsg = "Error: Unable to open file for writing.";
    return 0;
  }

  fprintf(file, "%s", text);
  fclose(file);
  return 1;
}

int write_binary_file(const char *filename, const unsigned char *data,
                      size_t length, char **errmsg) {
  FILE *file = fopen(filename, "wb");
  if (file == NULL) {
    *errmsg = "Error: Unable to open binary file for writing.";
    return 0;
  }

  size_t items_written = fwrite(data, 1, length, file);
  fclose(file);

  return items_written == length ? 1 : 0;
}

MCbuffer *read_file_into_buffer(FILE *fp, char **errmsg) {
  fseek(fp, 0, SEEK_END);
  long file_size = ftell(fp);
  rewind(fp);

  unsigned char *data = (unsigned char *)MALLOC(file_size);
  if (data == NULL) {
    fclose(fp);
    *errmsg = "Error: Memory allocation failed.";
    return NULL;
  }

  size_t length = fread(data, 1, file_size, fp);
  fclose(fp);

  MCbuffer *buffer = MCbuffer_init();
  buffer->capacity = length;
  buffer->length = length;
  buffer->data = data;

  return buffer;
}