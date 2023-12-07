#pragma once

#include "mcbuffer.h"
#include "mctypes.h"
#include <stdio.h>

// Function to read a text file and return its content as a string
char *read_text_file(const char *filename, char **errmsg);

// Function to read a binary file and return the result as an unsigned int array
byte_t *read_binary_file(const char *filename, size_t *length, char **errmsg);

// Function to write a string to a text file
int write_text_file(const char *filename, const char *text, char **errmsg);

// Function to write an unsigned char array to a binary file
int write_binary_file(const char *filename, const unsigned char *data,
                      size_t length, char **errmsg);

MCbuffer *read_file_into_buffer(FILE *fp, char **errmsg);
