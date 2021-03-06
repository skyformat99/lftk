/**
 * File:   array.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  dynamic array.
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/array.h"
#include "base/mem.h"

array_t* array_create(uint16_t capacity) {
  array_t* array = MEM_ZALLOC(array_t);

  if (array_init(array, capacity)) {
    return array;
  } else {
    MEM_FREE(array);

    return NULL;
  }
}

array_t* array_init(array_t* array, uint16_t capacity) {
  return_value_if_fail(array != NULL, NULL);

  array->size = 0;
  array->capacity = capacity;
  if (capacity > 0) {
    array->elms = MEM_ZALLOCN(void*, capacity);
    return_value_if_fail(array->elms != NULL, NULL);
  } else {
    array->elms = NULL;
  }

  return array;
}

static bool_t array_extend(array_t* array) {
  if (array->elms != NULL && array->size < array->capacity) {
    return TRUE;
  } else {
    void* elms = NULL;
    uint16_t capacity = (array->capacity >> 1) + array->capacity + 1;

    elms = MEM_REALLOC(void*, array->elms, capacity);
    if (elms) {
      array->elms = elms;
      array->capacity = capacity;

      return TRUE;
    } else {
      return FALSE;
    }
  }
}

int array_find_index(array_t* array, compare_t cmp, void* data) {
  int32_t i = 0;
  int32_t size = 0;
  void** elms = NULL;
  return_value_if_fail(array != NULL, -1);
  elms = array->elms;
  size = array->size;

  if (cmp) {
    for (i = 0; i < size; i++) {
      void* iter = elms[i];
      if (cmp(iter, data) == 0) {
        return i;
      }
    }
  } else {
    for (i = 0; i < size; i++) {
      void* iter = elms[i];
      if (iter == data) {
        return i;
      }
    }
  }

  return -1;
}

bool_t array_remove(array_t* array, compare_t cmp, void* data) {
  int32_t i = 0;
  int32_t pos = 0;
  int32_t size = 0;
  void** elms = NULL;

  return_value_if_fail(array != NULL, FALSE);
  pos = array_find_index(array, cmp, data);
  if (pos < 0) {
    return FALSE;
  }

  elms = array->elms;
  for (size = array->size - 1, i = pos; i < size; i++) {
    elms[i] = elms[i + 1];
  }

  elms[i] = NULL;
  array->size--;

  return TRUE;
}

bool_t array_foreach(array_t* array, visit_t visit, void* ctx) {
  uint16_t i = 0;
  uint16_t size = 0;
  void** elms = NULL;
  return_value_if_fail(array != NULL && visit != NULL, FALSE);

  size = array->size;
  elms = array->elms;
  for (i = 0; i < size; i++) {
    if (!visit(ctx, elms[i])) {
      return FALSE;
    }
  }

  return TRUE;
}

void* array_find(array_t* array, compare_t cmp, void* data) {
  int pos = array_find_index(array, cmp, data);
  if (pos >= 0) {
    return array->elms[pos];
  } else {
    return NULL;
  }
}

void* array_pop(array_t* array) {
  return_value_if_fail(array != NULL && array->size > 0, NULL);

  return array->elms[--array->size];
}

bool_t array_push(array_t* array, void* data) {
  return_value_if_fail(array != NULL, FALSE);
  return_value_if_fail(array_extend(array), FALSE);

  array->elms[array->size++] = data;

  return TRUE;
}

void array_deinit(array_t* array) {
  return_if_fail(array != NULL && array->elms != NULL);
  MEM_FREE(array->elms);
  memset(array, 0x00, sizeof(array_t));

  return;
}

void array_destroy(array_t* array) {
  return_if_fail(array != NULL && array->elms != NULL);
  array_deinit(array);
  MEM_FREE(array);

  return;
}
