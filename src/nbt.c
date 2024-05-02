/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <phk@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Poul-Henning Kamp
 * ----------------------------------------------------------------------------
 */

/*
 * NOTICE: Some or all functions may be modifyed or copied from this repo
 * https://github.dev/chmod222/cNBT.
 */

#include "nbt.h"
#include "buffer.h"
#include "err.h"
#include "heap_utils.h"
#include "text_buffer.h"
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Utils and macros
/*
 * Is the list all one type? If yes, return the type. Otherwise, return
 * TAG_END
 */
static nbt_type cmc_nbt_is_list_homogenous(const struct nbt_list *list) {
  nbt_type type = TAG_END;

  const struct list_head *pos;
  list_for_each(pos, &list->entry) {
    const struct nbt_list *cur = list_entry(pos, const struct nbt_list, entry);

    assert(cur->data);
    assert(cur->data->type != TAG_END);

    if (cur->data->type == TAG_END)
      return TAG_END;

    /* if we're the first type, just set it to our current type */
    if (type == TAG_END)
      type = cur->data->type;

    if (type != cur->data->type)
      return TAG_END;
  }

  /* if the list was empty, use the sentinel type */
  if (type == TAG_END && list->data != NULL)
    type = list->data->type;

  return type;
}

const char *nbt_type_to_string(nbt_type t) {
#define DEF_CASE(name)                                                         \
  case name:                                                                   \
    return #name;
  switch (t) {
  case 0:
    return "TAG_END";
    DEF_CASE(TAG_BYTE);
    DEF_CASE(TAG_SHORT);
    DEF_CASE(TAG_INT);
    DEF_CASE(TAG_LONG);
    DEF_CASE(TAG_FLOAT);
    DEF_CASE(TAG_DOUBLE);
    DEF_CASE(TAG_BYTE_ARRAY);
    DEF_CASE(TAG_STRING);
    DEF_CASE(TAG_LIST);
    DEF_CASE(TAG_COMPOUND);
    DEF_CASE(TAG_INT_ARRAY);
  default:
    return "TAG_UNKNOWN";
  }
#undef DEF_CASE
}

// Endian Utils

/* are we running on a little-endian system? */
static int little_endian() {
  uint16_t t = 0x0001;
  char c[2];
  memcpy(c, &t, sizeof t);
  return c[0];
}

static void *swap_bytes(void *s, size_t len) {
  for (char *b = s, *e = b + len - 1; b < e; b++, e--) {
    char t = *b;

    *b = *e;
    *e = t;
  }

  return s;
}

/* big endian to native endian. works in-place */
static void *be2ne(void *s, size_t len) {
  return little_endian() ? swap_bytes(s, len) : s;
}

/* native endian to big endian. works the exact same as its inverse */
#define ne2be be2ne

// NBT free

void nbt_free(cmc_nbt *tree);

void nbt_free_list(struct nbt_list *list) {
  if (!list)
    return;

  struct list_head *current;
  struct list_head *temp;
  list_for_each_safe(current, temp, &list->entry) {
    struct nbt_list *entry = list_entry(current, struct nbt_list, entry);

    nbt_free(entry->data);
    FREE(entry);
  }

  FREE(list->data);
  FREE(list);
}

void nbt_free(cmc_nbt *tree) {
  if (tree == NULL)
    return;

  // TODO: this can be a switch statement
  if (tree->type == TAG_LIST)
    nbt_free_list(tree->payload.tag_list);

  else if (tree->type == TAG_COMPOUND)
    nbt_free_list(tree->payload.tag_compound);

  else if (tree->type == TAG_BYTE_ARRAY)
    FREE(tree->payload.tag_byte_array.data);

  else if (tree->type == TAG_INT_ARRAY)
    FREE(tree->payload.tag_int_array.data);

  else if (tree->type == TAG_LONG_ARRAY)
    FREE(tree->payload.tag_long_array.data);

  else if (tree->type == TAG_STRING)
    FREE(tree->payload.tag_string);

  FREE(tree->name);
  FREE(tree);
}

// NBT reading utils

static char *nbt_read_string(cmc_buffer *buff) {
  int16_t str_len = cmc_buffer_unpack_short(buff);
  ERR_CHECK(return NULL;);

  be2ne(&str_len, sizeof(int16_t));
  ERR_IF(str_len <= 0, CMC_ERR_INVALID_STRING, return NULL;);

  char *string = MALLOC(str_len + 1); // null terminator
  char *heap_data = (char *)cmc_buffer_unpack(buff, str_len);
  ERR_CHECK(return NULL;);

  memcpy(string, heap_data, str_len);
  FREE(heap_data);
  string[str_len] = '\0';

  ERR_CHECK(return NULL;);

  return string;
}

cmc_nbt *parse_unnamed_tag(cmc_buffer *buff, nbt_type type, char *name);

static struct nbt_list *read_compound(cmc_buffer *buff) {
  struct nbt_list *ret;

  ret = MALLOC(sizeof(struct nbt_list));
  ret->data = NULL;
  INIT_LIST_HEAD(&ret->entry);

  for (;;) {
    uint8_t type;
    char *name = NULL;
    struct nbt_list *new_entry;

    type = ERR_ABLE(cmc_buffer_unpack_byte(buff), goto err;);

    if (type == TAG_END)
      break;

    name = ERR_ABLE(nbt_read_string(buff), goto err;);

    new_entry = MALLOC(sizeof(struct nbt_list));

    new_entry->data =
        ERR_ABLE(parse_unnamed_tag(buff, (nbt_type)type, name), goto err;);

    if (new_entry->data == NULL) {
      FREE(new_entry);
      FREE(name);
      goto err;
    }

    list_add_tail(&new_entry->entry, &ret->entry);
  }

  return ret;

err:
  nbt_free_list(ret);

  return NULL;
}

static struct nbt_list *read_list(cmc_buffer *buff) {
  uint8_t type;
  int32_t elems;
  struct nbt_list *ret = NULL;

  ret = MALLOC(sizeof(struct nbt_list));

  /* we allocate the data pointer to store the type of the list in the first
   * sentinel element */
  ret->data = MALLOC(sizeof(struct nbt_list));

  INIT_LIST_HEAD(&ret->entry);

  type = cmc_buffer_unpack_byte(buff);
  ERR_CHECK(goto err;);

  elems = cmc_buffer_unpack_int(buff);
  ERR_CHECK(goto err;);
  be2ne(&elems, sizeof(int32_t));

  ret->data->type = type == TAG_END ? TAG_COMPOUND : (nbt_type)type;

  for (int32_t i = 0; i < elems; i++) {
    struct nbt_list *new;

    new = MALLOC(sizeof(struct nbt_list));

    new->data = parse_unnamed_tag(buff, (nbt_type)type, NULL);

    if (new->data == NULL) {
      FREE(new);
      goto err;
    }

    list_add_tail(&new->entry, &ret->entry);
  }

  return ret;

err:
  nbt_free_list(ret);
  return NULL;
}

static struct nbt_byte_array read_byte_array(cmc_buffer *buff) {
  struct nbt_byte_array ret;
  ret.data = NULL;

  ret.length = cmc_buffer_unpack_int(buff);
  ERR_CHECK(return ret;);

  be2ne(&ret.length, sizeof(int32_t));

  ERR_IF(ret.length <= 0, CMC_ERR_INVALID_LENGHT, return ret;);

  ret.data = cmc_buffer_unpack(buff, ret.length);

  return ret;
}

static struct nbt_int_array read_int_array(cmc_buffer *buff) {
  struct nbt_int_array ret;
  ret.data = NULL;

  ret.length = cmc_buffer_unpack_int(buff);
  ERR_CHECK(return ret;);
  be2ne(&ret.length, sizeof(int32_t));

  ERR_IF(ret.length <= 0, CMC_ERR_INVALID_LENGHT, return ret;);

  ret.data = (int32_t *)cmc_buffer_unpack(buff, sizeof(int32_t) * ret.length);
  ERR_CHECK(return ret;);

  // Byteswap the whole array.
  for (int32_t i = 0; i < ret.length; i++)
    be2ne(ret.data + i, sizeof(int32_t));

  return ret;
}

static struct nbt_long_array read_long_array(cmc_buffer *buff) {
  struct nbt_long_array ret;
  ret.data = NULL;

  ret.length = cmc_buffer_unpack_int(buff);
  ERR_CHECK(return ret;);
  be2ne(&ret.length, sizeof(int64_t));

  ERR_IF(ret.length <= 0, CMC_ERR_INVALID_LENGHT, return ret;);

  ret.data = (int64_t *)cmc_buffer_unpack(buff, sizeof(int64_t) * ret.length);
  ERR_CHECK(return ret;);

  // Byteswap the whole array.
  for (int32_t i = 0; i < ret.length; i++)
    be2ne(ret.data + i, sizeof(int64_t));

  return ret;
}

cmc_nbt *parse_unnamed_tag(cmc_buffer *buff, nbt_type type, char *name) {
  cmc_nbt *ret = NULL;
  ERR_CHECK(return ret;);
  ret = MALLOC(sizeof(cmc_nbt));

  ret->name = name;
  ret->type = type;

  switch (type) {
// Numbers
#define UNPACK_NUMBER(payload_attrib_name, type)                               \
  do {                                                                         \
    unsigned char *data = cmc_buffer_unpack(buff, sizeof(type));               \
    ERR_CHECK(return ret;);                                                    \
    type result = *((type *)data);                                             \
    FREE(data);                                                                \
    ret->payload.payload_attrib_name = result;                                 \
  } while (0)
  case TAG_BYTE:
    UNPACK_NUMBER(tag_byte, int8_t);
    break;
  case TAG_SHORT:
    UNPACK_NUMBER(tag_short, int16_t);
    break;
  case TAG_INT:
    UNPACK_NUMBER(tag_int, int32_t);
    break;
  case TAG_LONG:
    UNPACK_NUMBER(tag_long, int64_t);
    break;
  case TAG_FLOAT:
    UNPACK_NUMBER(tag_float, float);
    break;
  case TAG_DOUBLE:
    UNPACK_NUMBER(tag_double, double);
    break;
#undef UNPACK_NUMBER

  case TAG_COMPOUND:
    ret->payload.tag_compound = read_compound(buff);
    break;
  case TAG_STRING:
    ret->payload.tag_string = nbt_read_string(buff);
    break;
  case TAG_LIST:
    ret->payload.tag_list = read_list(buff);
    break;
  case TAG_BYTE_ARRAY:
    ret->payload.tag_byte_array = read_byte_array(buff);
    break;
  case TAG_INT_ARRAY:
    ret->payload.tag_int_array = read_int_array(buff);
    break;
  case TAG_LONG_ARRAY:
    ret->payload.tag_long_array = read_long_array(buff);
    break;
  default:
    ERR(CMC_ERR_INVALID_NBT_TAG_TYPE, goto err;);
  }

  ERR_CHECK(goto err;);

  return ret;
err:
  nbt_free(ret);
  return NULL;
}

cmc_nbt *nbt_parse_named_tag(cmc_buffer *buff) {
  cmc_nbt *ret = NULL;
  char *name = NULL;
  uint8_t type = cmc_buffer_unpack_byte(buff);
  ERR_CHECK(return ret;);

  name = nbt_read_string(buff);
  ERR_CHECK(return ret;);

  ret = parse_unnamed_tag(buff, type, name);
  ERR_CHECK(return ret;);
  return ret;
}

// Nbt printing utils
static void __nbt_dump_ascii(const cmc_nbt *tree, text_buffer *b, size_t ident);

/* prints the node's name, or (null) if it has none. */
#define SAFE_NAME(node) ((node)->name ? (node)->name : "<null>")

static void dump_byte_array(const struct nbt_byte_array ba, text_buffer *b) {
  assert(ba.length >= 0);

  text_buffer_bprintf(b, "[ ");
  for (int32_t i = 0; i < ba.length; ++i)
    text_buffer_bprintf(b, "%u ", +ba.data[i]);
  text_buffer_bprintf(b, "]");
}

static void dump_int_array(const struct nbt_int_array ia, text_buffer *b) {
  assert(ia.length >= 0);

  text_buffer_bprintf(b, "[ ");
  for (int32_t i = 0; i < ia.length; ++i)
    text_buffer_bprintf(b, "%u ", +ia.data[i]);
  text_buffer_bprintf(b, "]");
}

static void dump_long_array(const struct nbt_long_array la, text_buffer *b) {
  assert(la.length >= 0);

  text_buffer_bprintf(b, "[ ");
  for (int32_t i = 0; i < la.length; ++i)
    text_buffer_bprintf(b, "%u ", +la.data[i]);
  text_buffer_bprintf(b, "]");
}

static void dump_list_contents_ascii(const struct nbt_list *list,
                                     text_buffer *b, size_t ident) {
  const struct list_head *pos;

  list_for_each(pos, &list->entry) {
    const struct nbt_list *entry =
        list_entry(pos, const struct nbt_list, entry);

    __nbt_dump_ascii(entry->data, b, ident);
    ERR_CHECK(return;);
  }

  return;
}

static void __nbt_dump_ascii(const cmc_nbt *tree, text_buffer *b,
                             size_t ident) {
  if (tree == NULL)
    ERR(CMC_ERR_INVALID_ARGUMENTS, return;);

  text_buffer_indent(b, ident);

  if (tree->type == TAG_BYTE)
    text_buffer_bprintf(b, "TAG_Byte(\"%s\"): %i\n", SAFE_NAME(tree),
                        (int)tree->payload.tag_byte);
  else if (tree->type == TAG_SHORT)
    text_buffer_bprintf(b, "TAG_Short(\"%s\"): %i\n", SAFE_NAME(tree),
                        (int)tree->payload.tag_short);
  else if (tree->type == TAG_INT)
    text_buffer_bprintf(b, "TAG_Int(\"%s\"): %i\n", SAFE_NAME(tree),
                        (int)tree->payload.tag_int);
  else if (tree->type == TAG_LONG)
    text_buffer_bprintf(b, "TAG_Long(\"%s\"): %li\n", SAFE_NAME(tree),
                        tree->payload.tag_long);
  else if (tree->type == TAG_FLOAT)
    text_buffer_bprintf(b, "TAG_Float(\"%s\"): %f\n", SAFE_NAME(tree),
                        (double)tree->payload.tag_float);
  else if (tree->type == TAG_DOUBLE)
    text_buffer_bprintf(b, "TAG_Double(\"%s\"): %f\n", SAFE_NAME(tree),
                        tree->payload.tag_double);
  else if (tree->type == TAG_BYTE_ARRAY) {
    text_buffer_bprintf(b, "TAG_Byte_Array(\"%s\"): ", SAFE_NAME(tree));
    dump_byte_array(tree->payload.tag_byte_array, b);
    text_buffer_bprintf(b, "\n");
  } else if (tree->type == TAG_INT_ARRAY) {
    text_buffer_bprintf(b, "Tag_Int_Array(\"%s\"): ", SAFE_NAME(tree));
    dump_int_array(tree->payload.tag_int_array, b);
    text_buffer_bprintf(b, "\n");
  } else if (tree->type == TAG_LONG_ARRAY) {
    text_buffer_bprintf(b, "Tag_Long_Array(\"%s\"): ", SAFE_NAME(tree));
    dump_long_array(tree->payload.tag_long_array, b);
    text_buffer_bprintf(b, "\n");
  } else if (tree->type == TAG_STRING) {
    if (tree->payload.tag_string == NULL)
      ERR(CMC_ERR_INVALID_STRING, return;);
    text_buffer_bprintf(b, "TAG_String(\"%s\"): %s\n", SAFE_NAME(tree),
                        tree->payload.tag_string);
  } else if (tree->type == TAG_LIST) {
    text_buffer_bprintf(b, "TAG_List(\"%s\") [%s]\n", SAFE_NAME(tree),
                        nbt_type_to_string(tree->payload.tag_list->data->type));
    text_buffer_indent(b, ident);
    text_buffer_bprintf(b, "{\n");

    dump_list_contents_ascii(tree->payload.tag_list, b, ident + 1);
    ERR_CHECK(return;);

    text_buffer_indent(b, ident);
    text_buffer_bprintf(b, "}\n");

    ERR_CHECK(return;);
  } else if (tree->type == TAG_COMPOUND) {
    text_buffer_bprintf(b, "TAG_Compound(\"%s\")\n", SAFE_NAME(tree));
    text_buffer_indent(b, ident);
    text_buffer_bprintf(b, "{\n");

    dump_list_contents_ascii(tree->payload.tag_compound, b, ident + 1);
    ERR_CHECK(return;);

    text_buffer_indent(b, ident);
    text_buffer_bprintf(b, "}\n");

    ERR_CHECK(return;);
  } else
    ERR(CMC_ERR_INVALID_NBT_TAG_TYPE, return;);
}

char *nbt_dump_ascii(const cmc_nbt *tree) {
  text_buffer b = TEXT_BUFFER_INIT;

  ERR_ABLE(__nbt_dump_ascii(tree, &b, 0), goto err;);
  ERR_ABLE(text_buffer_reserve(&b, b.len + 1), goto err;);

  b.data[b.len] = '\0'; /* null-terminate that biatch, since text_buffer_bprintf
                           doesn't do that for us. */

  return (char *)b.data;

err:

  text_buffer_free(&b);
  return NULL;
}

// Nbt dumping as binary

static void dump_byte_array_binary(const struct nbt_byte_array ba,
                                   cmc_buffer *buff) {
  int32_t dumped_length = ba.length;

  ne2be(&dumped_length, sizeof dumped_length);

  cmc_buffer_pack_int(buff, dumped_length);

  if (ba.length)
    assert(ba.data);

  cmc_buffer_pack(buff, ba.data, ba.length);
}

static void dump_int_array_binary(const struct nbt_int_array ia,
                                  cmc_buffer *buff) {
  int32_t dumped_length = ia.length;

  ne2be(&dumped_length, sizeof dumped_length);

  cmc_buffer_pack_int(buff, dumped_length);

  if (ia.length)
    assert(ia.data);

  for (int32_t i = 0; i < ia.length; i++) {
    int32_t swappedElem = ia.data[i];
    ne2be(&swappedElem, sizeof(swappedElem));
    cmc_buffer_pack_int(buff, swappedElem);
  }
}

static void dump_long_array_binary(const struct nbt_long_array la,
                                   cmc_buffer *buff) {
  int32_t dumped_length = la.length;

  ne2be(&dumped_length, sizeof dumped_length);

  cmc_buffer_pack_int(buff, dumped_length);

  if (la.length)
    assert(la.data);

  for (int32_t i = 0; i < la.length; i++) {
    int64_t swappedElem = la.data[i];
    ne2be(&swappedElem, sizeof(swappedElem));
    cmc_buffer_pack_long(buff, swappedElem);
  }
}

static void dump_string_binary(const char *name, cmc_buffer *buff) {
  assert(name);

  size_t len = strlen(name);

  ERR_IF(len > INT16_MAX, CMC_ERR_STRING_LENGHT, return;);

  /* dump the length */
  int16_t dumped_len = (int16_t)len;
  ne2be(&dumped_len, sizeof dumped_len);

  cmc_buffer_pack_short(buff, dumped_len);
  cmc_buffer_pack(buff, name, len);

  return;
}

static void __dump_binary(const cmc_nbt *, bool, cmc_buffer *buff);

static void dump_list_binary(const struct nbt_list *list, cmc_buffer *buff) {
  nbt_type type = cmc_nbt_is_list_homogenous(list);

  size_t len = list_length(&list->entry);

  ERR_IF(len > INT32_MAX, CMC_ERR_INVALID_LENGHT, return;);

  assert(type != TAG_END);

  int8_t _type = (int8_t)type;
  ne2be(&_type, sizeof _type); /* unnecessary, but left in to keep similar code
                                  looking similar */
  cmc_buffer_pack_char(buff, _type);

  int32_t dumped_len = (int32_t)len;
  ne2be(&dumped_len, sizeof dumped_len);
  cmc_buffer_pack_int(buff, dumped_len);

  const struct list_head *pos;
  list_for_each(pos, &list->entry) {
    const struct nbt_list *entry =
        list_entry(pos, const struct nbt_list, entry);

    ERR_ABLE(__dump_binary(entry->data, false, buff), return;);
  }

  return;
}

static void dump_compound_binary(const struct nbt_list *list,
                                 cmc_buffer *buff) {
  const struct list_head *pos;
  list_for_each(pos, &list->entry) {
    const struct nbt_list *entry =
        list_entry(pos, const struct nbt_list, entry);
    ERR_ABLE(__dump_binary(entry->data, false, buff), return;);
  }

  cmc_buffer_pack_byte(buff, 0x00);
}

/*
 * @param dump_type   Should we dump the type, or just skip it? We need to skip
 *                    when dumping lists, because the list header already says
 *                    the type.
 */
static void __dump_binary(const cmc_nbt *tree, bool dump_type,
                          cmc_buffer *buff) {
  if (dump_type) /* write out the type */
    cmc_buffer_pack_char(buff, (int8_t)tree->type);

  if (tree->name) {
    ERR_ABLE(dump_string_binary(tree->name, buff), return;);
  }

#define DUMP_NUM(type, x)                                                      \
  do {                                                                         \
    type temp = x;                                                             \
    ne2be(&temp, sizeof temp);                                                 \
    cmc_buffer_pack(buff, &temp, sizeof temp);                                 \
  } while (0)

  if (tree->type == TAG_BYTE)
    DUMP_NUM(int8_t, tree->payload.tag_byte);
  else if (tree->type == TAG_SHORT)
    DUMP_NUM(int16_t, tree->payload.tag_short);
  else if (tree->type == TAG_INT)
    DUMP_NUM(int32_t, tree->payload.tag_int);
  else if (tree->type == TAG_LONG)
    DUMP_NUM(int64_t, tree->payload.tag_long);
  else if (tree->type == TAG_FLOAT)
    DUMP_NUM(float, tree->payload.tag_float);
  else if (tree->type == TAG_DOUBLE)
    DUMP_NUM(double, tree->payload.tag_double);
  else if (tree->type == TAG_BYTE_ARRAY)
    return dump_byte_array_binary(tree->payload.tag_byte_array, buff);
  else if (tree->type == TAG_INT_ARRAY)
    return dump_int_array_binary(tree->payload.tag_int_array, buff);
  else if (tree->type == TAG_LONG_ARRAY)
    return dump_long_array_binary(tree->payload.tag_long_array, buff);
  else if (tree->type == TAG_STRING)
    return dump_string_binary(tree->payload.tag_string, buff);
  else if (tree->type == TAG_LIST)
    return dump_list_binary(tree->payload.tag_list, buff);
  else if (tree->type == TAG_COMPOUND)
    return dump_compound_binary(tree->payload.tag_compound, buff);
  else
    ERR(CMC_ERR_INVALID_NBT_TAG_TYPE, return;);

  return;

#undef DUMP_NUM
}

cmc_buffer *nbt_dump_binary(const cmc_nbt *tree) {
  if (tree == NULL)
    return cmc_buffer_init(47);

  cmc_buffer *ret = cmc_buffer_init(47);

  __dump_binary(tree, true, ret);

  return ret;
}
