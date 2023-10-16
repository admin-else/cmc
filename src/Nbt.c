#include "Nbt.h"
#include "MCbuffer.h"
#include "err.h"
#include "heap-utils.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERR_CHECK(description)                                                 \
  if (*errmsg != NULL) {                                                       \
    char error_message[256];                                                   \
    sprintf(error_message, description, *errmsg);                              \
    *errmsg = strdup(error_message);                                           \
    return ret;                                                                \
  }

#define ERR_IF(condition, desctiption)                                         \
  if (condition) {                                                             \
    *errmsg = desctiption;                                                     \
    return ret;                                                                \
  }

#define READ_GENERIC(dest, n, scanner, on_failure)                             \
  do {                                                                         \
    if (*length < (n)) {                                                       \
      on_failure;                                                              \
    }                                                                          \
    *memory = scanner((dest), *memory, (n));                                   \
    *length -= (n);                                                            \
  } while (0)

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

static char *nbt_read_string(MCbuffer *buff, char **errmsg) {
  char *ret = NULL;
  int16_t str_len = MCbuffer_unpack_short(buff, errmsg);
  ERR_CHECK("Error while unpacking nbt string len \n%s")

  be2ne(&str_len, sizeof(int16_t));
  ERR_IF(str_len < 0, "Nbt String len less than zero?")

  char *string = MALLOC(str_len + 1); // null terminator
  char *heap_data = (char *)MCbuffer_unpack(buff, str_len, errmsg);

  memcpy(string, heap_data, str_len);
  FREE(heap_data);
  string[str_len] = '\0';

  ERR_CHECK("Err while unpacking nbt string: \n%s");

  return string;
}

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
  if (*errmsg != NULL)
    goto err;
  elems = MCbuffer_unpack_int(buff, errmsg);
  if (*errmsg != NULL)
    goto err;

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
  ERR_CHECK("error while reading nbt byte arr: \n%s");
  ERR_IF(ret.length < 0, "byte array length less than zero???")

  ret.data = MCbuffer_unpack(buff, ret.length, errmsg);
  ERR_CHECK("error while reading nbt byte arr: \n%s");

  return ret;
}

static struct nbt_int_array read_int_array(MCbuffer *buff, char **errmsg) {
  struct nbt_int_array ret;
  ret.data = NULL;

  ret.length = MCbuffer_unpack_int(buff, errmsg);
  ERR_CHECK("Error while unpacking nbt int array len %s\n")

  ERR_IF(ret.length < 0, "nbt int array len less than zero????")

  ret.data =
      (int32_t *)MCbuffer_unpack(buff, sizeof(int32_t) * ret.length, errmsg);
  ERR_CHECK("Error while unpacking nbt int array %s\n")

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
    type *pVal = (type *)MCbuffer_unpack(buff, sizeof(type), errmsg);          \
    if (*errmsg != NULL)                                                       \
      return NULL;                                                             \
    /* the pointer must be valid if there is no error. */                      \
                                                                               \
    ret->payload.payload_attrib_name = *pVal;                                  \
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

  default:
    *errmsg = "invalid nbt tag type";
    goto err;
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
  ERR_CHECK("Error while parsing named nbt tag: \n%s")

  puts("read string");
  name = nbt_read_string(buff, errmsg);
  ERR_CHECK("Err while parsing named tag \n%s");
  puts("end read string");

  ret = parse_unnamed_tag(buff, type, name, errmsg);
  ERR_CHECK("%s")

  return ret;
}