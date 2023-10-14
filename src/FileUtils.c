
#include "FileUtils.h"
#include "MCtypes.h"
#include <stdlib.h>

char* read_text_file(const char* filename, char** errmsg) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        *errmsg = "Error: Unable to open file for reading.";
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(file_size + 1);
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

byte_t* read_binary_file(const char* filename, size_t* length, char** errmsg) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        *errmsg = "Error: Unable to open binary file for reading.";
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    byte_t* buffer = (byte_t*)malloc(file_size);
    if (buffer == NULL) {
        fclose(file);
        *errmsg = "Error: Memory allocation failed.";
        return NULL;
    }

    size_t items_read = fread(buffer, sizeof(byte_t), file_size / sizeof(byte_t), file);
    fclose(file);

    if (length != NULL) {
        *length = items_read * sizeof(byte_t);
    }

    return buffer;
}

int write_text_file(const char* filename, const char* text, char** errmsg) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        *errmsg = "Error: Unable to open file for writing.";
        return 0;
    }

    fprintf(file, "%s", text);
    fclose(file);
    return 1;
}

int write_binary_file(const char* filename, const unsigned char* data, size_t length, char** errmsg) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        *errmsg = "Error: Unable to open binary file for writing.";
        return 0;
    }

    size_t items_written = fwrite(data, 1, length, file);
    fclose(file);

    return items_written == length ? 1 : 0;
}
