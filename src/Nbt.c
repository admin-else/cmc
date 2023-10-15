#include "Nbt.h"
#include "MCbuffer.h"
#include "err.h"
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
    return return_val;                                                         \
  }

#define ERR_IF(condition, desctiption)                                         \
  if (condition) {                                                             \
    *errmsg = desctiption;                                                     \
    return return_val;                                                         \
  }

#define MALLOC(val, n)                                                         \
  if ((val = malloc(n)) == NULL) {                                             \
    *errmsg = "out of memory.";                                                \
    return return_val;                                                         \
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
static int little_endian()
{
    uint16_t t = 0x0001;
    char c[2];
    memcpy(c, &t, sizeof t);
    return c[0];
}

static void* swap_bytes(void* s, size_t len)
{
    for(char* b = s,
            * e = b + len - 1;
        b < e;
        b++, e--)
    {
        char t = *b;

        *b = *e;
        *e = t;
    }

    return s;
}

/* big endian to native endian. works in-place */
static void* be2ne(void* s, size_t len)
{
    return little_endian() ? swap_bytes(s, len) : s;
}

/* native endian to big endian. works the exact same as its inverse */
#define ne2be be2ne

static char *nbt_read_string(MCbuffer *buff, char **errmsg) {
  char *return_val = NULL;
  int16_t str_len = MCbuffer_unpack_short(buff, errmsg);

  be2ne(&str_len, sizeof(int16_t));
  ERR_IF(str_len < 0, "Nbt String len less than zero?")

  return_val = (char *)MCbuffer_unpack(buff, str_len, errmsg);
  ERR_CHECK("Err while unpacking nbt string: \n%s")

  return_val[str_len] = '\0'; //null terminate

  return return_val;
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
    free(entry);
  }

  free(list->data);
  free(list);
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
    free(tree->payload.tag_byte_array.data);

  else if (tree->type == TAG_INT_ARRAY)
    free(tree->payload.tag_int_array.data);

  else if (tree->type == TAG_LONG_ARRAY)
    free(tree->payload.tag_long_array.data);

  else if (tree->type == TAG_STRING)
    free(tree->payload.tag_string);

  free(tree->name);
  free(tree);
}

nbt_node *parse_unnamed_tag(MCbuffer *buff, nbt_type type, char *name,
                            char **errmsg);

static struct nbt_list *read_compound(MCbuffer *buff, char **errmsg) {
  struct nbt_list *return_val;

  MALLOC(return_val, sizeof(struct nbt_list));

  return_val->data = NULL;
  INIT_LIST_HEAD(&return_val->entry);

  for (;;) {
    uint8_t type;
    char *name = NULL;
    struct nbt_list *new_entry;

    type = MCbuffer_unpack_byte(buff, errmsg);
    if (*errmsg != NULL)
      goto err;

    if (type == TAG_END)
      break; /* TAG_END == 0. We've hit the end of the list when type ==
                TAG_END. */

    name = nbt_read_string(buff, errmsg);
    if (*errmsg != NULL)
      goto err;

    new_entry = malloc(sizeof(struct nbt_list));
    if (new_entry == NULL)
      goto err;

    new_entry->data = parse_unnamed_tag(buff, (nbt_type)type, name, errmsg);

    if (new_entry->data == NULL) {
      free(new_entry);
      free(name);
      goto err;
    }

    list_add_tail(&new_entry->entry, &return_val->entry);
  }

  return return_val;

err:
  nbt_free_list(return_val);

  return NULL;
}

static struct nbt_list *read_list(MCbuffer *buff, char **errmsg) {
  uint8_t type;
  int32_t elems;
  struct nbt_list *return_val = NULL;

  return_val = malloc(sizeof(struct nbt_list));
  if (return_val == NULL) {
    *errmsg = "out of memory";
    return NULL;
  }

  /* we allocate the data pointer to store the type of the list in the first
   * sentinel element */
  return_val->data = malloc(sizeof(struct nbt_list));
  if (return_val->data == NULL) {
    *errmsg = "out of memory";
    goto err;
  }

  INIT_LIST_HEAD(&return_val->entry);

  type = MCbuffer_unpack_byte(buff, errmsg);
  if (*errmsg != NULL)
    goto err;
  elems = MCbuffer_unpack_int(buff, errmsg);
  if (*errmsg != NULL)
    goto err;

  return_val->data->type = type == TAG_END ? TAG_COMPOUND : (nbt_type)type;

  for (int32_t i = 0; i < elems; i++) {
    struct nbt_list *new;

    new = malloc(sizeof(struct nbt_list));
    if (new == NULL) {
      *errmsg = "out of memory";
      goto err;
    }

    new->data = parse_unnamed_tag(buff, (nbt_type)type, NULL, errmsg);

    if (new->data == NULL) {
      free(new);
      goto err;
    }

    list_add_tail(&new->entry, &return_val->entry);
  }

  return return_val;

err:
  nbt_free_list(return_val);
  return NULL;
}

nbt_node *parse_unnamed_tag(MCbuffer *buff, nbt_type type, char *name,
                            char **errmsg) {
  nbt_node *return_val = NULL;
  MALLOC(return_val, sizeof(nbt_node))

  return_val->name = name;
  return_val->type = type;

  switch (type) {
// Numbers
#define UNPACK_NUMBER(payload_attrib_name, type)                               \
  do {                                                                         \
    type *pVal = (type *)MCbuffer_unpack(buff, sizeof(type), errmsg);          \
    if (*errmsg != NULL)                                                       \
      return NULL;                                                             \
    /* the pointer must be valid if there is no error. */                      \
                                                                               \
    return_val->payload.payload_attrib_name = *pVal;                           \
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
    return_val->payload.tag_compound = read_compound(buff, errmsg);
    break;
  case TAG_STRING:
    return_val->payload.tag_string = nbt_read_string(buff, errmsg);
    break;
  case TAG_LIST:

  case TAG_BYTE_ARRAY:

  case TAG_INT_ARRAY:

  case TAG_LONG_ARRAY:

  default:
    *errmsg = "invalid nbt tag type";
    goto err;
  }

err:
  free(return_val);
  return NULL;
}

nbt_node *nbt_parse_named_tag(MCbuffer *buff, char **errmsg) {
  nbt_node *return_val = NULL;
  char *name = NULL;
  uint8_t type = MCbuffer_unpack_byte(buff, errmsg);
  ERR_CHECK("Error while parsing named nbt tag: \n%s")

  name = nbt_read_string(buff, errmsg);
  ERR_CHECK("Err while parsing named tag \n%s");

  return_val = parse_unnamed_tag(buff, type, name, errmsg);
  ERR_CHECK("%s")

  return return_val;
}