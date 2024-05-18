#include <cmc/nbt.h>

#include <cmc/err.h>
#include <stdlib.h>

#include "err_macros.h"
#include <cmc/heap_utils.h>

static unsigned char cmc_nbt_unpack_byte(cmc_buff *buff) {
  unsigned char *heap_data =
      CMC_ERRB_ABLE(cmc_buff_unpack(buff, sizeof(unsigned char)), return 0;);
  unsigned char data = *heap_data;
  free(heap_data);
  return data;
}

static int16_t cmc_nbt_unpack_short(cmc_buff *buff) {
  int16_t *heap_data =
      CMC_ERRB_ABLE(cmc_buff_unpack(buff, sizeof(int16_t)), return 0;);
  int16_t data = *heap_data;
  free(heap_data);
  data = be16toh(data);
  return data;
}

static int32_t cmc_nbt_unpack_int(cmc_buff *buff) {
  int32_t *heap_data =
      CMC_ERRB_ABLE(cmc_buff_unpack(buff, sizeof(int32_t)), return 0;);
  int32_t data = *heap_data;
  free(heap_data);
  data = be32toh(data);
  return data;
}

static int64_t cmc_nbt_unpack_long(cmc_buff *buff) {
  int64_t *heap_data =
      CMC_ERRB_ABLE(cmc_buff_unpack(buff, sizeof(int64_t)), return 0;);
  int64_t data = *heap_data;
  free(heap_data);
  data = be64toh(data);
  return data;
}

static char *cmc_nbt_unpack_string(cmc_buff *buff) {
  int16_t strlen = CMC_ERRB_ABLE(cmc_nbt_unpack_short(buff), return NULL;);
  if (strlen < 0)
    CMC_ERRB(CMC_ERR_NEGATIVE_STRING_LENGTH, return NULL;);
  char *str = CMC_ERRB_ABLE(cmc_malloc(strlen + 1, &buff->err), return NULL;);
  CMC_ERRB_ABLE(cmc_buff_unpack_into(buff, strlen, str), goto err_free_str;);
  str[strlen] = '\0';
  return str;

err_free_str:
  free(str);
  return NULL;
}

cmc_nbt *cmc_nbt_init(cmc_err_extra *err) {
  cmc_nbt *tree = CMC_ERR_ABLE(cmc_malloc(sizeof(cmc_nbt), err), return NULL;);
  tree->name = NULL;
  tree->type = CMC_NBT_TAG_END;
  return tree;
}

cmc_nbt *cmc_nbt_parse(cmc_buff *buff) {
  assert(buff);
  cmc_nbt *tree = cmc_nbt_init(&buff->err);

  tree->type = CMC_ERRB_ABLE(cmc_nbt_unpack_byte(buff), goto err_free_nbt;);

  tree->name = CMC_ERRB_ABLE(cmc_nbt_unpack_string(buff), goto err_free_nbt;);

  switch (tree->type) {
  case CMC_NBT_TAG_END:
    CMC_ERRB(CMC_ERR_NBT_UNEXPECTED_TAG_END, return 0;);
    break;
  case CMC_NBT_TAG_BYTE:
    tree->payload.tag_byte =
        CMC_ERRB_ABLE(cmc_nbt_unpack_byte(buff), goto err_free_nbt;);
    break;
  case CMC_NBT_TAG_SHORT:
    tree->payload.tag_short =
        CMC_ERRB_ABLE(cmc_nbt_unpack_short(buff), goto err_free_nbt;);
    break;
  case CMC_NBT_TAG_INT:
    tree->payload.tag_int =
        CMC_ERRB_ABLE(cmc_nbt_unpack_int(buff), goto err_free_nbt;);
    break;
  case CMC_NBT_TAG_LONG:
    tree->payload.tag_long =
        CMC_ERRB_ABLE(cmc_nbt_unpack_long(buff), goto err_free_nbt;);
    break;
  case CMC_NBT_TAG_FLOAT:
    tree->payload.tag_float =
        CMC_ERRB_ABLE(cmc_buff_unpack_float(buff), goto err_free_nbt;);
    break;
  case CMC_NBT_TAG_DOUBLE:
    tree->payload.tag_double =
        CMC_ERRB_ABLE(cmc_buff_unpack_double(buff), goto err_free_nbt;);
    break;
  case CMC_NBT_TAG_BYTE_ARRAY:
    tree->payload.tag_byte_array.length =
        CMC_ERRB_ABLE(cmc_nbt_unpack_int(buff), goto err_free_nbt;);
    tree->payload.tag_byte_array.data = CMC_ERRB_ABLE(
        cmc_buff_unpack(buff, tree->payload.tag_byte_array.length *
                                  sizeof(unsigned char)),
        goto err_free_nbt;);
    break;
  case CMC_NBT_TAG_STRING: {
    uint16_t str_len =
        CMC_ERRB_ABLE(cmc_buff_unpack_ushort(buff), goto err_free_nbt;);
    char *str =
        CMC_ERRB_ABLE(cmc_malloc(str_len + 1, &buff->err), goto err_free_nbt;)
            CMC_ERRB_ABLE(cmc_buff_unpack_into(buff, str_len, str), {
              free(str);
              goto err_free_nbt;
            });
    str[str_len] = '\0';
    tree->payload.tag_string = str;
    break;
  }
  case CMC_NBT_TAG_LIST: {
    cmc_nbt_type list_type =
        CMC_ERRB_ABLE(cmc_nbt_unpack_byte(buff), goto err_free_nbt;);
    int32_t len = CMC_ERRB_ABLE(cmc_nbt_unpack_int(buff), goto err_free_nbt;);
    if (len < 0)
      CMC_ERRB(CMC_ERR_NBT_NEGATIVE_LIST_LEN, goto err_free_nbt;);
    if (list_type != CMC_NBT_TAG_END && len == 0)
      CMC_ERRB(CMC_ERR_NBT_BAD_LIST, goto err_free_nbt;);
    break;
  }
  case CMC_NBT_TAG_COMPOUND:
  case CMC_NBT_TAG_INT_ARRAY:
    tree->payload.tag_int_array.length =
        CMC_ERRB_ABLE(cmc_nbt_unpack_int(buff), goto err_free_nbt;);
    tree->payload.tag_int_array.data =
        CMC_ERRB_ABLE(cmc_buff_unpack(buff, tree->payload.tag_int_array.length *
                                                sizeof(int32_t)),
                      goto err_free_nbt;);
    break;
  case CMC_NBT_TAG_LONG_ARRAY:
    tree->payload.tag_long_array.length =
        CMC_ERRB_ABLE(cmc_nbt_unpack_int(buff), goto err_free_nbt;);
    tree->payload.tag_long_array.data = CMC_ERRB_ABLE(
        cmc_buff_unpack(buff,
                        tree->payload.tag_long_array.length * sizeof(int64_t)),
        goto err_free_nbt;);
    break;
  }

  return tree;
err_free_nbt:
  cmc_nbt_free(tree);
  return NULL;
}

cmc_buff *cmc_nbt_text(const cmc_nbt *tree, cmc_err_extra *err) {
  (void)tree;
  CMC_ERR(CMC_ERR_NOT_IMPLEMENTED_YET, return NULL;);
}

cmc_buff *cmc_nbt_dump(const cmc_nbt *tree, cmc_err_extra *err) {
  (void)tree;
  CMC_ERR(CMC_ERR_NOT_IMPLEMENTED_YET, return NULL;);
}

void cmc_nbt_free(cmc_nbt *tree) {
  (void)tree;
  abort(); // unimplemented
}
