#pragma once

#include "MCtypes.h"
#include <jansson.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

MCbuffer *MCbuffer_init();

void MCbuffer_free(MCbuffer *buffer);

void MCbuffer_pack(MCbuffer *buffer, const void *data, size_t data_size,
                   char **errmsg);

unsigned char *MCbuffer_unpack(MCbuffer *buffer, size_t n, char **errmsg);

#define DATA_PACK_AND_UNPACK_FUNC_FACTORY_H(name, type)                        \
  type MCbuffer_unpack_##name(MCbuffer *buffer, char **errmsg);                \
  void MCbuffer_pack_##name(MCbuffer *buffer, type data, char **errmsg);

DATA_PACK_AND_UNPACK_FUNC_FACTORY_H(char, char);
DATA_PACK_AND_UNPACK_FUNC_FACTORY_H(byte, unsigned char);
DATA_PACK_AND_UNPACK_FUNC_FACTORY_H(short, short);
DATA_PACK_AND_UNPACK_FUNC_FACTORY_H(ushort, unsigned short);
DATA_PACK_AND_UNPACK_FUNC_FACTORY_H(int, int);
DATA_PACK_AND_UNPACK_FUNC_FACTORY_H(uint, unsigned int);
DATA_PACK_AND_UNPACK_FUNC_FACTORY_H(long, long);
DATA_PACK_AND_UNPACK_FUNC_FACTORY_H(ulong, unsigned long);
DATA_PACK_AND_UNPACK_FUNC_FACTORY_H(float, float);
DATA_PACK_AND_UNPACK_FUNC_FACTORY_H(double, double);
DATA_PACK_AND_UNPACK_FUNC_FACTORY_H(llong, long long);
DATA_PACK_AND_UNPACK_FUNC_FACTORY_H(ullong, unsigned long long);

void MCbuffer_pack_bool(MCbuffer *buffer, bool value, char **errmsg);

bool MCbuffer_unpack_bool(MCbuffer *buffer, char **errmsg);

void MCbuffer_pack_varint(MCbuffer *buff, int value, char **errmsg);

int32_t MCbuffer_unpack_varint(MCbuffer *buff, char **errmsg);

void MCbuffer_pack_string_w_max_len(MCbuffer *buff, const char *value, int max_len, char **errmsg);

void MCbuffer_pack_string(MCbuffer *buff, const char *value, char **errmsg);

char *MCbuffer_unpack_string_w_max_len(MCbuffer *buff, int max_len, char **errmsg);

char *MCbuffer_unpack_string(MCbuffer *buff, char **errmsg);

void MCbuffer_pack_position(MCbuffer *buff, block_pos_t pos, char **errmsg);

block_pos_t MCbuffer_unpack_position(MCbuffer *buff, char **errmsg);

nbt_node *MCbuffer_unpack_nbt(MCbuffer *buff, char **errmsg);

json_t *MCbuffer_unpack_json(MCbuffer *buff, char **errmsg);

void MCbuffer_pack_json(MCbuffer *buff, json_t *json, char **errmsg);

void MCbuffer_print_info(MCbuffer *buff);

MCbuffer *MCbuffer_combine(MCbuffer *buff1, MCbuffer *buff2, char **errmsg);

MCbuffer *MCbuffer_unpack_byte_array(MCbuffer *buff, char **errmsg);

void MCbuffer_pack_byte_array(MCbuffer *buff, MCbuffer *byte_array, char **errmsg);

MCbuffer *MCbuffer_init_w_size(size_t n);