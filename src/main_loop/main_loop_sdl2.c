/**
 * file:   main_loop_sdl2.c
 * author: li xianjing <xianjimli@hotmail.com>
 * brief:  sdl2 implemented main_loop interface
 *
 * copyright (c) 2018 - 2018  li xianjing <xianjimli@hotmail.com>
 *
 * this program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * license file for more details.
 *
 */

/**
 * history:
 * ================================================================
 * 2018-01-13 li xianjing <xianjimli@hotmail.com> created
 *
 */

#include "main_loop/main_loop_sdl2.h"
#include "base/font_manager.h"
#include "base/window_manager.h"
#include "lcd/lcd_sdl2.h"

typedef struct _main_loop_sdl2_t {
  main_loop_t base;

  int w;
  int h;
  widget_t* wm;
  canvas_t canvas;
  SDL_Window* sdl_window;
  SDL_Renderer* sdl_render;
  uint16_t ctrl : 1;
  uint16_t alt : 1;
  uint16_t shift : 1;
  uint16_t pressed : 1;
} main_loop_sdl2_t;

static ret_t main_loop_sdl2_dispatch_key_event(main_loop_sdl2_t* loop, SDL_Event* sdl_event) {
  key_event_t event;
  int type = sdl_event->type;
  widget_t* widget = loop->wm;

  event.key = sdl_event->key.keysym.sym;
  switch (type) {
    case SDL_KEYDOWN: {
      event.e.type = EVT_KEY_DOWN;
      widget_on_keydown(widget, &event);
      break;
    }
    case SDL_KEYUP: {
      event.e.type = EVT_KEY_UP;
      widget_on_keyup(widget, &event);
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

static ret_t main_loop_sdl2_dispatch_mouse_event(main_loop_sdl2_t* loop, SDL_Event* sdl_event) {
  pointer_event_t event;
  int type = sdl_event->type;
  widget_t* widget = loop->wm;

  switch (type) {
    case SDL_MOUSEBUTTONDOWN: {
      loop->pressed = 1;
      event.e.type = EVT_POINTER_DOWN;
      event.x = sdl_event->button.x;
      event.y = sdl_event->button.y;
      event.button = sdl_event->button.button;
      event.pressed = loop->pressed;

      widget_on_pointer_down(widget, &event);
      break;
    }
    case SDL_MOUSEBUTTONUP: {
      event.e.type = EVT_POINTER_UP;
      event.x = sdl_event->button.x;
      event.y = sdl_event->button.y;
      event.button = sdl_event->button.button;
      event.pressed = loop->pressed;

      widget_on_pointer_up(widget, &event);
      loop->pressed = 0;
      break;
    }
    case SDL_MOUSEMOTION: {
      event.e.type = EVT_POINTER_MOVE;
      event.x = sdl_event->motion.x;
      event.y = sdl_event->motion.y;
      event.button = 0;
      event.pressed = loop->pressed;

      widget_on_pointer_move(widget, &event);
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

static ret_t main_loop_sdl2_dispatch(main_loop_sdl2_t* loop) {
  SDL_Event event;
  ret_t ret = RET_OK;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_KEYDOWN:
      case SDL_KEYUP: {
        ret = main_loop_sdl2_dispatch_key_event(loop, &event);
        break;
      }
      case SDL_MOUSEMOTION:
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP: {
        ret = main_loop_sdl2_dispatch_mouse_event(loop, &event);
        break;
      }
      case SDL_QUIT: {
        ret = main_loop_quit(&(loop->base));
        break;
      }
    }
  }

  return ret;
}

static ret_t main_loop_sdl2_paint(main_loop_sdl2_t* loop) {
  ret_t ret = window_manager_paint(loop->wm, &(loop->canvas));

  return ret;
}

static ret_t main_loop_sdl2_run(main_loop_t* l) {
  main_loop_sdl2_t* loop = (main_loop_sdl2_t*)l;

  while (l->running) {
    main_loop_sdl2_dispatch(loop);
    main_loop_sdl2_paint(loop);
  }

  return RET_OK;
}

static ret_t main_loop_sdl2_quit(main_loop_t* l) { return RET_OK; }

static ret_t main_loop_sdl2_destroy(main_loop_t* l) {
  const char* errMsg = SDL_GetError();
  main_loop_sdl2_t* loop = (main_loop_sdl2_t*)l;

  if (errMsg && strlen(errMsg)) {
    SDL_Log("Error : %s\n", errMsg);
  }

  if (loop->sdl_window) {
    SDL_DestroyWindow(loop->sdl_window);
    loop->sdl_window = NULL;
  }

  if (loop->sdl_render) {
    SDL_DestroyRenderer(loop->sdl_render);
  }

  SDL_Quit();

  return RET_OK;
}

static ret_t main_loop_sdl2_remove_timer(main_loop_t* l, id_t id) {
  /*TODO*/
  return RET_OK;
}

static id_t main_loop_sdl2_add_timer(main_loop_t* l, uint32_t time_ms, event_handler on_event,
                                     void* ctx) {
  /*TODO*/
  return RET_OK;
}

static ret_t main_loop_sdl2_create_window(main_loop_sdl2_t* l, font_manager_t* fm, int w, int h) {
  l->w = w;
  l->h = h;
  SDL_Init(SDL_INIT_VIDEO);

  l->sdl_window = SDL_CreateWindow("LFTK Simulator", SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_RESIZABLE);
  return_value_if_fail(l->sdl_window != NULL, RET_FAIL);

  l->sdl_render = SDL_CreateRenderer(l->sdl_window, -1, 0);
  return_value_if_fail(l->sdl_render != NULL, RET_FAIL);

  canvas_init(&(l->canvas), lcd_sdl2_init(l->sdl_render), fm);

  return RET_OK;
}

static main_loop_sdl2_t loop;
main_loop_t* main_loop_sdl2_init(int w, int h) {
  widget_t* wm = default_wm();
  font_manager_t* fm = default_fm();
  main_loop_t* base = &(loop.base);

  memset(&loop, 0x00, sizeof(loop));

  base->run = main_loop_sdl2_run;
  base->quit = main_loop_sdl2_quit;
  base->destroy = main_loop_sdl2_destroy;
  base->add_timer = main_loop_sdl2_add_timer;
  base->remove_timer = main_loop_sdl2_remove_timer;

  loop.wm = wm;
  window_manager_resize(wm, w, h);
  main_loop_sdl2_create_window(&loop, fm, w, h);

  return base;
}

main_loop_t* default_main_loop() { return &loop.base; }