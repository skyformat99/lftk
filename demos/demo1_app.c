/**
 * File:   demo1_app.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  basic class of all widget
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
 * 2018-02-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/timer.h"
#include "base/button.h"
#include "base/check_button.h"
#include "base/dialog.h"
#include "base/image.h"
#include "base/image_manager.h"
#include "base/label.h"
#include "base/mem.h"
#include "base/progress_bar.h"
#include "base/utils.h"
#include "base/window.h"

static ret_t on_timer(const f_timer_t* timer) {
  widget_t* progress_bar = (widget_t*)timer->user_data;
  uint8_t value = (PROGRESS_BAR(progress_bar)->value + 10) % 100;
  progress_bar_set_value(progress_bar, value);

  return RET_REPEAT;
}

static ret_t on_inc(void* ctx, event_t* e) {
  widget_t* progress_bar = (widget_t*)ctx;
  uint8_t value = (PROGRESS_BAR(progress_bar)->value + 10) % 100;
  progress_bar_set_value(progress_bar, value);
  (void)e;
  return RET_OK;
}

static ret_t on_dec(void* ctx, event_t* e) {
  widget_t* progress_bar = (widget_t*)ctx;
  uint8_t value = (PROGRESS_BAR(progress_bar)->value + 90) % 100;
  progress_bar_set_value(progress_bar, value);
  (void)e;
  return RET_OK;
}

static ret_t on_ok(void* ctx, event_t* e) {
  widget_t* dialog = (widget_t*)ctx;

  dialog_quit(dialog, 0);
  (void)e;
  mem_info_dump();
  return RET_OK;
}

static ret_t on_cancel(void* ctx, event_t* e) {
  widget_t* dialog = (widget_t*)ctx;

  dialog_quit(dialog, 1);
  (void)e;
  mem_info_dump();
  return RET_OK;
}

static ret_t on_show_dialog(void* ctx, event_t* e) {
  uint32_t code = 0;
  widget_t* ok = NULL;
  widget_t* cancel = NULL;
  widget_t* win = dialog_create(NULL, 0, 0, 300, 240);
  dialog_t* dialog = DIALOG(win);

  widget_set_text(win, L"Dialog");
  dialog_set_icon(win, "info");

  ok = button_create(dialog->client, 60, 160, 80, 30);
  widget_set_text(ok, L"OK");

  cancel = button_create(dialog->client, 200, 160, 80, 30);
  widget_set_text(cancel, L"Cancel");

  widget_on(ok, EVT_CLICK, on_ok, dialog);
  widget_on(cancel, EVT_CLICK, on_cancel, dialog);

  code = dialog_modal(win);
  printf("code=%d\n", code);
  widget_destroy(win);

  mem_info_dump();
  (void)e;

  return RET_OK;
}

ret_t application_init() {
  widget_t* image = NULL;
  widget_t* ok = NULL;
  widget_t* label = NULL;
  widget_t* cancel = NULL;
  widget_t* progress_bar = NULL;
  widget_t* check_button = NULL;
  widget_t* radio_button = NULL;
  widget_t* show_dialog = NULL;
  widget_t* win = window_create(NULL, 0, 0, 0, 0);

  ok = button_create(win, 10, 5, 80, 30);
  widget_set_text(ok, L"Inc");

  cancel = button_create(win, 100, 5, 80, 30);
  widget_set_text(cancel, L"Dec");

  show_dialog = button_create(win, 190, 5, 80, 30);
  widget_set_text(show_dialog, L"Dialog");

  image = image_create(win, 10, 230, 100, 100);
  image_set_image_name(image, "earth");

  label = label_create(win, 10, 40, 80, 30);
  widget_set_text(label, L"Left");
  label_set_text_align_h(label, ALIGN_H_LEFT);

  label = label_create(win, 100, 40, 80, 30);
  widget_set_text(label, L"Center");
  label_set_text_align_h(label, ALIGN_H_CENTER);

  label = label_create(win, 200, 40, 80, 30);
  widget_set_text(label, L"Right");
  label_set_text_align_h(label, ALIGN_H_RIGHT);

  progress_bar = progress_bar_create(win, 10, 80, 168, 30);
  widget_set_value(progress_bar, 40);
  timer_add(on_timer, progress_bar, 1000);
  //  progress_bar_set_show_text(progress_bar, TRUE);

  widget_on(ok, EVT_CLICK, on_inc, progress_bar);
  widget_on(cancel, EVT_CLICK, on_dec, progress_bar);
  widget_on(show_dialog, EVT_CLICK, on_show_dialog, NULL);

  progress_bar = progress_bar_create(win, 260, 80, 30, 118);
  timer_add(on_timer, progress_bar, 1000);
  widget_set_value(progress_bar, 40);
  progress_bar_set_vertical(progress_bar, TRUE);
  widget_on(ok, EVT_CLICK, on_inc, progress_bar);
  widget_on(cancel, EVT_CLICK, on_dec, progress_bar);

  check_button = check_button_create(win, 10, 150, 80, 30);
  widget_set_text(check_button, L"Book");

  check_button = check_button_create(win, 100, 150, 80, 30);
  widget_set_text(check_button, L"Food");

  radio_button = radio_button_create(win, 10, 200, 80, 30);
  widget_set_text(radio_button, L"Book");

  radio_button = radio_button_create(win, 100, 200, 80, 30);
  widget_set_text(radio_button, L"Food");

  radio_button = radio_button_create(win, 190, 200, 80, 30);
  widget_set_text(radio_button, L"Pencil");
  widget_set_value(radio_button, TRUE);

  return RET_OK;
}
