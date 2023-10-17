#include "Nbt.h"
#include "MCbuffer.h"
#include "err.h"
#include "heap-utils.h"
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Utils and macros

#define ERR_CHECK                                                              \
  if (*errmsg != NULL) {                                                       \
    return ret;                                                                \
  }

#define ERR_IF(condition, desctiption)                                         \
  if (condition) {                                                             \
    *errmsg = desctiption;                                                     \
    return ret;                                                                \
  }

#define BUFFER_INIT                                                            \
  (struct text_buffer) { NULL, 0, 0 }

#ifdef __GNUC__
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect((x), 0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

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

void nbt_free(nbt_node *tree);

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

void nbt_free(nbt_node *tree) {
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

static char *nbt_read_string(MCbuffer *buff, char **errmsg) {
  char *ret = NULL;
  int16_t str_len = MCbuffer_unpack_short(buff, errmsg);
  ERR_CHECK;

  be2ne(&str_len, sizeof(int16_t));
  ERR_IF(str_len < 0, "Nbt String len less than zero?")

  char *string = MALLOC(str_len + 1); // null terminator
  char *heap_data = (char *)MCbuffer_unpack(buff, str_len, errmsg);

  memcpy(string, heap_data, str_len);
  FREE(heap_data);
  string[str_len] = '\0';

  ERR_CHECK;

  return string;
}

nbt_node *parse_unnamed_tag(MCbuffer *buff, nbt_type type, char *name,
                            char **errmsg);

static struct nbt_list *read_compound(MCbuffer *buff, char **errmsg) {
  struct nbt_list *ret;

  ret = MALLOC(sizeof(struct nbt_list));

  ret->data = NULL;
  INIT_LIST_HEAD(&ret->entry);

  for (;;) {
    uint8_t type;
    char *name = NULL;
    struct nbt_list *new_entry;

    type = MCbuffer_unpack_byte(buff, errmsg);
    if (*errmsg != NULL)
      goto err;

    if (type == TAG_END)
      break;

    name = nbt_read_string(buff, errmsg);
    if (*errmsg != NULL)
      goto err;

    new_entry = MALLOC(sizeof(struct nbt_list));

    new_entry->data = parse_unnamed_tag(buff, (nbt_type)type, name, errmsg);

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

static struct nbt_list *read_list(MCbuffer *buff, char **errmsg) {
  uint8_t type;
  int32_t elems;
  struct nbt_list *ret = NULL;

  ret = MALLOC(sizeof(struct nbt_list));

  /* we allocate the data pointer to store the type of the list in the first
   * sentinel element */
  ret->data = MALLOC(sizeof(struct nbt_list));

  INIT_LIST_HEAD(&ret->entry);

  type = MCbuffer_unpack_byte(buff, errmsg);
  if (*errmsg != NULL) {
    goto err;
  }

  elems = MCbuffer_unpack_int(buff, errmsg);
  if (*errmsg != NULL) {
    goto err;
  }
  be2ne(&elems, sizeof(int32_t));

  ret->data->type = type == TAG_END ? TAG_COMPOUND : (nbt_type)type;

  for (int32_t i = 0; i < elems; i++) {
    struct nbt_list *new;

    new = MALLOC(sizeof(struct nbt_list));

    new->data = parse_unnamed_tag(buff, (nbt_type)type, NULL, errmsg);

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

static struct nbt_byte_array read_byte_array(MCbuffer *buff, char **errmsg) {
  struct nbt_byte_array ret;
  ret.data = NULL;

  ret.length = MCbuffer_unpack_int(buff, errmsg);
  ERR_CHECK;

  be2ne(&ret.length, sizeof(int32_t));

  ERR_IF(ret.length < 0, "byte array length less than zero???")

  ret.data = MCbuffer_unpack(buff, ret.length, errmsg);
  ERR_CHECK;

  return ret;
}

static struct nbt_int_array read_int_array(MCbuffer *buff, char **errmsg) {
  struct nbt_int_array ret;
  ret.data = NULL;

  ret.length = MCbuffer_unpack_int(buff, errmsg);
  ERR_CHECK;

  ERR_IF(ret.length < 0, "nbt int array len less than zero????")

  ret.data =
      (int32_t *)MCbuffer_unpack(buff, sizeof(int32_t) * ret.length, errmsg);
  ERR_CHECK;

  // Byteswap the whole array.
  for (int32_t i = 0; i < ret.length; i++)
    be2ne(ret.data + i, sizeof(int32_t));

  return ret;
}

nbt_node *parse_unnamed_tag(MCbuffer *buff, nbt_type type, char *name,
                            char **errmsg) {
  nbt_node *ret = NULL;
  ret = MALLOC(sizeof(nbt_node));

  ret->name = name;
  ret->type = type;

  switch (type) {
// Numbers
#define UNPACK_NUMBER(payload_attrib_name, type)                               \
  do {                                                                         \
    unsigned char *data = MCbuffer_unpack(buff, sizeof(type), errmsg);         \
    ERR_CHECK;                                                                 \
    type result = *((type *)data);                                             \
    FREE(data);                                                                \
    ret->payload.payload_attrib_name = result;                                \
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
    ret->payload.tag_compound = read_compound(buff, errmsg);
    break;
  case TAG_STRING:
    ret->payload.tag_string = nbt_read_string(buff, errmsg);
    break;
  case TAG_LIST:
    ret->payload.tag_list = read_list(buff, errmsg);
    break;
  case TAG_BYTE_ARRAY:
    ret->payload.tag_byte_array = read_byte_array(buff, errmsg);
    break;
  case TAG_INT_ARRAY:
    ret->payload.tag_int_array = read_int_array(buff, errmsg);
    break;
  case TAG_LONG_ARRAY:
    puts("TODO IMPLEMENT THIS");
  default:
    *errmsg = "invalid nbt tag type";
    goto err;
  }

  if (*errmsg != ((void *)0)) {
    char error_message[256];
    sprintf(error_message, "Error while parsing NBT of type %s tag \n%s",
            nbt_type_to_string(type), *errmsg);
    *errmsg = strdup(error_message);
    return ret;
  }

  return ret;
err:
  FREE(ret);
  return NULL;
}

nbt_node *nbt_parse_named_tag(MCbuffer *buff, char **errmsg) {
  nbt_node *ret = NULL;
  char *name = NULL;
  uint8_t type = MCbuffer_unpack_byte(buff, errmsg);
  ERR_CHECK;

  name = nbt_read_string(buff, errmsg);
  ERR_CHECK;

  ret = parse_unnamed_tag(buff, type, name, errmsg);
  ERR_CHECK;
  return ret;
}

// Nbt printing utils

struct text_buffer {
  unsigned char
      *data;  /* You can access the buffer's raw bytes through this pointer */
  size_t len; /* Only accesses in the interval [data, data + len) are defined */
  size_t cap; /* Internal use. The allocated size of the buffer. */
};

static int lazy_init(struct text_buffer *b) {
  assert(b->data == NULL);

  size_t cap = 1024;

  *b = (struct text_buffer){.data = malloc(cap), .len = 0, .cap = cap};

  if (unlikely(b->data == NULL))
    return 1;

  return 0;
}

void text_buffer_free(struct text_buffer *b) {
  assert(b);

  free(b->data);

  b->data = NULL;
  b->len = 0;
  b->cap = 0;
}

int buffer_reserve(struct text_buffer *b, size_t reserved_amount) {
  assert(b);

  if (unlikely(b->data == NULL) && unlikely(lazy_init(b)))
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

int buffer_append(struct text_buffer *b, const void *data, size_t n) {
  assert(b);

  if (unlikely(b->data == NULL) && unlikely(lazy_init(b)))
    return 1;

  if (unlikely(buffer_reserve(b, b->len + n)))
    return 1;

  memcpy(b->data + b->len, data, n);
  b->len += n;

  return 0;
}

static void bprintf(struct text_buffer *b, const char *restrict format, ...) {
  va_list args;
  int siz;

  va_start(args, format);
  siz = vsnprintf(NULL, 0, format, args);
  va_end(args);

  buffer_reserve(b, b->len + siz + 1);

  va_start(args, format);
  vsnprintf((char *)(b->data + b->len), siz + 1, format, args);
  va_end(args);

  b->len += siz; // remember - no null terminator!
}

static void indent(struct text_buffer *b, size_t amount) {
  size_t spaces = amount * 4; /* 4 spaces per indent */

  char temp[spaces + 1];

  for (size_t i = 0; i < spaces; ++i)
    temp[i] = ' ';
  temp[spaces] = '\0';

  bprintf(b, "%s", temp);
}

static void __nbt_dump_ascii(const nbt_node *tree, struct text_buffer *b,
                             size_t ident, char **errmsg);

/* prints the node's name, or (null) if it has none. */
#define SAFE_NAME(node) ((node)->name ? (node)->name : "<null>")

static void dump_byte_array(const struct nbt_byte_array ba,
                            struct text_buffer *b) {
  assert(ba.length >= 0);

  bprintf(b, "[ ");
  for (int32_t i = 0; i < ba.length; ++i)
    bprintf(b, "%u ", +ba.data[i]);
  bprintf(b, "]");
}

static void dump_int_array(const struct nbt_int_array ia,
                           struct text_buffer *b) {
  assert(ia.length >= 0);

  bprintf(b, "[ ");
  for (int32_t i = 0; i < ia.length; ++i)
    bprintf(b, "%u ", +ia.data[i]);
  bprintf(b, "]");
}

static void dump_long_array(const struct nbt_long_array la,
                            struct text_buffer *b) {
  assert(la.length >= 0);

  bprintf(b, "[ ");
  for (int32_t i = 0; i < la.length; ++i)
    bprintf(b, "%u ", +la.data[i]);
  bprintf(b, "]");
}

static void dump_list_contents_ascii(const struct nbt_list *list,
                                     struct text_buffer *b, size_t ident,
                                     char **errmsg) {
  const struct list_head *pos;

  list_for_each(pos, &list->entry) {
    const struct nbt_list *entry =
        list_entry(pos, const struct nbt_list, entry);

    __nbt_dump_ascii(entry->data, b, ident, errmsg);
    if (*errmsg != NULL)
      return;
  }

  return;
}

static void __nbt_dump_ascii(const nbt_node *tree, struct text_buffer *b,
                             size_t ident, char **errmsg) {
  if (tree == NULL)
    return;

  indent(b, ident);

  if (tree->type == TAG_BYTE)
    bprintf(b, "TAG_Byte(\"%s\"): %i\n", SAFE_NAME(tree),
            (int)tree->payload.tag_byte);
  else if (tree->type == TAG_SHORT)
    bprintf(b, "TAG_Short(\"%s\"): %i\n", SAFE_NAME(tree),
            (int)tree->payload.tag_short);
  else if (tree->type == TAG_INT)
    bprintf(b, "TAG_Int(\"%s\"): %i\n", SAFE_NAME(tree),
            (int)tree->payload.tag_int);
  else if (tree->type == TAG_LONG)
    bprintf(b, "TAG_Long(\"%s\"): %li\n", SAFE_NAME(tree),
            tree->payload.tag_long);
  else if (tree->type == TAG_FLOAT)
    bprintf(b, "TAG_Float(\"%s\"): %f\n", SAFE_NAME(tree),
            (double)tree->payload.tag_float);
  else if (tree->type == TAG_DOUBLE)
    bprintf(b, "TAG_Double(\"%s\"): %f\n", SAFE_NAME(tree),
            tree->payload.tag_double);
  else if (tree->type == TAG_BYTE_ARRAY) {
    bprintf(b, "TAG_Byte_Array(\"%s\"): ", SAFE_NAME(tree));
    dump_byte_array(tree->payload.tag_byte_array, b);
    bprintf(b, "\n");
  } else if (tree->type == TAG_INT_ARRAY) {
    bprintf(b, "Tag_Int_Array(\"%s\"): ", SAFE_NAME(tree));
    dump_int_array(tree->payload.tag_int_array, b);
    bprintf(b, "\n");
  } else if (tree->type == TAG_LONG_ARRAY) {
    bprintf(b, "Tag_Long_Array(\"%s\"): ", SAFE_NAME(tree));
    dump_long_array(tree->payload.tag_long_array, b);
    bprintf(b, "\n");
  } else if (tree->type == TAG_STRING) {
    if (tree->payload.tag_string == NULL) {
      *errmsg = "content of string that was null";
      return;
    }
    bprintf(b, "TAG_String(\"%s\"): %s\n", SAFE_NAME(tree),
            tree->payload.tag_string);
  } else if (tree->type == TAG_LIST) {
    bprintf(b, "TAG_List(\"%s\") [%s]\n", SAFE_NAME(tree),
            nbt_type_to_string(tree->payload.tag_list->data->type));
    indent(b, ident);
    bprintf(b, "{\n");

    dump_list_contents_ascii(tree->payload.tag_list, b, ident + 1, errmsg);
    if (*errmsg != NULL)
      return;

    indent(b, ident);
    bprintf(b, "}\n");

    if (*errmsg != NULL)
      return;
  } else if (tree->type == TAG_COMPOUND) {
    bprintf(b, "TAG_Compound(\"%s\")\n", SAFE_NAME(tree));
    indent(b, ident);
    bprintf(b, "{\n");

    dump_list_contents_ascii(tree->payload.tag_compound, b, ident + 1, errmsg);
    if (*errmsg != NULL)
      return;

    indent(b, ident);
    bprintf(b, "}\n");

    if (*errmsg != NULL)
      return;
  }

  else {
    *errmsg = "Invalid tag";
  }

  return;
}

char *nbt_dump_ascii(const nbt_node *tree, char **errmsg) {
  assert(tree);

  struct text_buffer b = BUFFER_INIT;

  __nbt_dump_ascii(tree, &b, 0, errmsg);
  if (*errmsg != NULL)
    goto err;
  buffer_reserve(&b, b.len + 1);
  if (*errmsg != NULL)
    goto err;

  b.data[b.len] = '\0'; /* null-terminate that biatch, since bprintf doesn't
                           do that for us. */

  return (char *)b.data;

err:

  text_buffer_free(&b);
  return NULL;
}