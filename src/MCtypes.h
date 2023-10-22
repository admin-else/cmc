#pragma once

#include <stdint.h>
#include "list.h"

typedef struct {
  unsigned char *data;
  size_t position;
  size_t length;
  size_t capacity;
} MCbuffer;

typedef struct {
    long x;
    long y;
    long z;
} block_pos_t;

typedef int varint_t;
typedef unsigned char byte_t ;

typedef enum {
    TAG_END        = 0, 
    TAG_BYTE       = 1, 
    TAG_SHORT      = 2, 
    TAG_INT        = 3, 
    TAG_LONG       = 4, 
    TAG_FLOAT      = 5, 
    TAG_DOUBLE     = 6, 
    TAG_BYTE_ARRAY = 7, 
    TAG_STRING     = 8, 
    TAG_LIST       = 9, 
    TAG_COMPOUND   = 10,
    TAG_INT_ARRAY  = 11,
    TAG_LONG_ARRAY = 12 

} nbt_type;

typedef struct nbt_node {
    nbt_type type;
    char* name; /* This may be NULL. Check your damn pointers. */

    union { /* payload */

        /* tag_end has no payload */
        int8_t  tag_byte;
        int16_t tag_short;
        int32_t tag_int;
        int64_t tag_long;
        float   tag_float;
        double  tag_double;

        struct nbt_byte_array {
            unsigned char* data;
            int32_t length;
        } tag_byte_array;

        struct nbt_int_array {
            int32_t* data;
            int32_t length;
        } tag_int_array;

        struct nbt_long_array {
            int64_t* data;
            int32_t length;
        } tag_long_array;

        char* tag_string; /* TODO: technically, this should be a UTF-8 string */

        /*
         * Design addendum: we make tag_list a linked list instead of an array
         * so that nbt_node can be a true recursive data structure. If we used
         * an array, it would be incorrect to call free() on any element except
         * the first one. By using a linked list, the context of the node is
         * irrelevant. One tradeoff of this design is that we don't get tight
         * list packing when memory is a concern and huge lists are created.
         *
         * For more information on using the linked list, see `list.h'. The API
         * is well documented.
         */
        struct nbt_list {
            struct nbt_node* data; /* A single node's data. */
            struct list_head entry;
        } * tag_list,
          * tag_compound;

        /*
         * The primary difference between a tag_list and a tag_compound is the
         * use of the first (sentinel) node.
         *
         * In an nbt_list, the sentinel node contains a valid data pointer with
         * only the type filled in. This is to deal with empty lists which
         * still posess types. Therefore, the sentinel's data pointer must be
         * deallocated.
         *
         * In the tag_compound, the only use of the sentinel is to get the
         * beginning and end of the doubly linked list. The data pointer is
         * unused and set to NULL.
         */
    } payload;
} nbt_node;