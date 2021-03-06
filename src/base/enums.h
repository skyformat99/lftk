/**
 * File:   enums.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  enumerations
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
 * 2018-02-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_ENUMS_H
#define LFTK_ENUMS_H

#include "base/types_def.h"

BEGIN_C_DECLS

typedef struct _key_type_value_t {
  const char* name;
  uint32_t type;
  uint32_t value;
} key_type_value_t;

enum { TYPE_INT = 0, TYPE_COLOR, TYPE_STRING, TYPE_WSTRING, TYPE_BOOL};

const key_type_value_t* widget_name_find(const char* name);
const key_type_value_t* style_name_find(const char* name);
const key_type_value_t* state_name_find(const char* name);
const key_type_value_t* align_v_name_find(const char* name);
const key_type_value_t* align_h_name_find(const char* name);

END_C_DECLS

#endif/*LFTK_ENUMS_H*/

