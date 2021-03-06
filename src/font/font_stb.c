/**
 * File:   font_stb.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  stb truetype font loader
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
 * 2018-01-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "font/font_stb.h"
#include "base/mem.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"

typedef struct _font_stb_t {
  font_t base;
  stbtt_fontinfo stb_font;
} font_stb_t;

static ret_t font_stb_find_glyph(font_t* f, wchar_t c, glyph_t* g) {
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;
  int s = f->size;
  font_stb_t* font = (font_stb_t*)f;
  stbtt_fontinfo* sf = &(font->stb_font);

  g->data = stbtt_GetCodepointBitmap(sf, 0, stbtt_ScaleForPixelHeight(sf, s), c, &w, &h, &x, &y);
  g->x = x;
  g->y = y;
  g->w = w;
  g->h = h;

  return g->data != NULL ? RET_OK : RET_NOT_FOUND;
}

static ret_t font_stb_destroy(font_t* f) {
  MEM_FREE(f);

  return RET_OK;
}

font_t* font_stb_create(const char* name, uint16_t font_size, const uint8_t* buff) {
  font_stb_t* f = NULL;
  return_value_if_fail(buff != NULL, NULL);

  f = MEM_ZALLOC(font_stb_t);
  return_value_if_fail(f != NULL, NULL);

  f->base.name = name;
  f->base.size = font_size;
  f->base.scalable = TRUE;
  f->base.find_glyph = font_stb_find_glyph;
  f->base.destroy = font_stb_destroy;

  stbtt_InitFont(&(f->stb_font), buff, stbtt_GetFontOffsetForIndex(buff, 0));

  return &(f->base);
}
