/**
 * File:   emitter.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  emitter dispatcher
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
 * 2018-01-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/emitter.h"
#include "base/mem.h"

emitter_t* emitter_create() {
  emitter_t* emitter = (emitter_t*)MEM_ZALLOC(emitter_t);

  return emitter_init(emitter);
}

emitter_t* emitter_init(emitter_t* emitter) {
  return_value_if_fail(emitter, NULL);
  memset(emitter, 0x00, sizeof(emitter_t));
  emitter->enable = TRUE;

  return emitter;
}

ret_t emitter_dispatch(emitter_t* emitter, event_t* e) {
  return_value_if_fail(emitter != NULL && e != NULL, RET_BAD_PARAMS);

  if (emitter->enable && emitter->items) {
    uint32_t i = 0;
    uint32_t size = emitter->size;
    emitter_item_t* items = emitter->items;

    emitter->stop = FALSE;
    for (i = 0; i < size; i++) {
      emitter_item_t* iter = items + i;
      if (iter->type == e->type) {
        iter->handler(iter->ctx, e);
      }

      if (emitter->stop) {
        break;
      }
    }
  }

  return RET_OK;
}

static ret_t emitter_extends(emitter_t* emitter, uint32_t nr) {
  uint32_t capacity = 0;
  emitter_item_t* items = NULL;
  if (emitter->items && emitter->size < emitter->capacity) {
    return RET_OK;
  }

  if (emitter->items == NULL) {
    emitter->items = MEM_ZALLOC(emitter_item_t);
    return_value_if_fail(emitter->items != NULL, RET_FAIL);

    emitter->size = 0;
    emitter->capacity = 1;

    return RET_OK;
  }

  capacity = emitter->capacity + 5;
  items = MEM_REALLOC(emitter_item_t, emitter->items, capacity);
  return_value_if_fail(items != NULL, RET_FAIL);

  emitter->items = items;
  emitter->capacity = capacity;

  return RET_OK;
}

ret_t emitter_on(emitter_t* emitter, uint16_t etype, event_handler handler, void* ctx) {
  emitter_item_t* iter = NULL;
  return_value_if_fail(emitter != NULL && handler != NULL, RET_BAD_PARAMS);
  return_value_if_fail(emitter_extends(emitter, 1) == RET_OK, RET_FAIL);

  iter = emitter->items + emitter->size;
  iter->type = etype;
  iter->ctx = ctx;
  iter->handler = handler;
  emitter->size++;

  return RET_OK;
}

ret_t emitter_off(emitter_t* emitter, uint16_t etype, event_handler handler, void* ctx) {
  return_value_if_fail(emitter != NULL && handler != NULL, RET_BAD_PARAMS);

  if (emitter->items) {
    uint32_t i = 0;
    uint32_t size = emitter->size;
    emitter_item_t* items = emitter->items;

    for (i = 0; i < size; i++) {
      emitter_item_t* iter = items + i;
      if (iter->type == etype && iter->ctx == ctx && iter->handler == handler) {
        for (; i < (size - 1); i++) {
          items[i] = items[i + 1];
        }
        emitter->size--;

        return RET_OK;
      }
    }
  }

  return RET_FAIL;
}

ret_t emitter_stop(emitter_t* emitter) {
  return_value_if_fail(emitter != NULL, RET_BAD_PARAMS);
  emitter->stop = TRUE;

  return RET_OK;
}

ret_t emitter_enable(emitter_t* emitter) {
  return_value_if_fail(emitter != NULL, RET_BAD_PARAMS);
  emitter->enable = TRUE;

  return RET_OK;
}

ret_t emitter_disable(emitter_t* emitter) {
  return_value_if_fail(emitter != NULL, RET_BAD_PARAMS);
  emitter->enable = FALSE;

  return RET_OK;
}

ret_t emitter_deinit(emitter_t* emitter) {
  return_value_if_fail(emitter != NULL, RET_BAD_PARAMS);
  if (emitter->items) {
    MEM_FREE(emitter->items);
  }

  return RET_OK;
}

ret_t emitter_destroy(emitter_t* emitter) {
  return_value_if_fail(emitter != NULL, RET_BAD_PARAMS);
  emitter_deinit(emitter);
  MEM_FREE(emitter);

  return RET_OK;
}
