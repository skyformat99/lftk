/**
 * File:   ui_loader.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  ui_loader interface
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

#ifndef LFTK_UI_LOADER_H
#define LFTK_UI_LOADER_H

#include "ui_loader/ui_builder.h"

BEGIN_C_DECLS

struct _ui_loader_t;
typedef struct _ui_loader_t ui_loader_t;

typedef ret_t (*ui_loader_load_t)(ui_loader_t* loader, const uint8_t* data, uint32_t size, ui_builder_t* b);

struct _ui_loader_t {
  ui_loader_load_t load;
};

ret_t ui_loader_load(ui_loader_t* loader, const uint8_t* data, uint32_t size, ui_builder_t* b);

END_C_DECLS

#endif/*LFTK_UI_LOADER_H*/

