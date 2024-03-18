#pragma once

#include <winlib/bool.h>
#include <winlib/platform.h>

typedef enum {
    WL_KEY_UNKNOWN,
    WL_KEY_BACK,
    WL_KEY_TAB,
    WL_KEY_RETURN,
    WL_KEY_LSHIFT,
    WL_KEY_RSHIFT,
    WL_KEY_PAGEUP,
    WL_KEY_PAGEDOWN,
    WL_KEY_LCTRL,
    WL_KEY_RCTRL,
    WL_KEY_LALT,
    WL_KEY_RALT,
    WL_KEY_ESC,
    WL_KEY_SPACE,
    WL_KEY_END,
    WL_KEY_HOME,
    WL_KEY_LEFT,
    WL_KEY_UP,
    WL_KEY_RIGHT,
    WL_KEY_DOWN,
    WL_KEY_DEL,
    WL_KEY_LWIN,
    WL_KEY_RWIN,
    WL_KEY_F1,
    WL_KEY_F2,
    WL_KEY_F3,
    WL_KEY_F4,
    WL_KEY_F5,
    WL_KEY_F6,
    WL_KEY_F7,
    WL_KEY_F8,
    WL_KEY_F9,
    WL_KEY_F10,
    WL_KEY_F11,
    WL_KEY_F12,
    WL_KEY_CAPS,
    WL_KEY_INSERT,
    WL_KEY_PAUSE,
    WL_KEY_NUMLOCK,
    WL_KEY_SCROLL,
    WL_KEY_PLUS,
    WL_KEY_COMMA,
    WL_KEY_MINUS,
    WL_KEY_PERIOD,
    WL_KEY_0,
    WL_KEY_1,
    WL_KEY_2,
    WL_KEY_3,
    WL_KEY_4,
    WL_KEY_5,
    WL_KEY_6,
    WL_KEY_7,
    WL_KEY_8,
    WL_KEY_9,
    WL_KEY_A,
    WL_KEY_B,
    WL_KEY_C,
    WL_KEY_D,
    WL_KEY_E,
    WL_KEY_F,
    WL_KEY_G,
    WL_KEY_H,
    WL_KEY_I,
    WL_KEY_J,
    WL_KEY_K,
    WL_KEY_L,
    WL_KEY_M,
    WL_KEY_N,
    WL_KEY_O,
    WL_KEY_P,
    WL_KEY_Q,
    WL_KEY_R,
    WL_KEY_S,
    WL_KEY_T,
    WL_KEY_U,
    WL_KEY_V,
    WL_KEY_W,
    WL_KEY_X,
    WL_KEY_Y,
    WL_KEY_Z,
    WL_KEY_COUNT
} wl_key_t;

typedef enum {
    WL_MOUSE_NONE   = 0,
    WL_MOUSE_LEFT   = 1,
    WL_MOUSE_MIDDLE = 2,
    WL_MOUSE_RIGHT  = 4,
} wl_mouse_btn_t;

typedef struct {
    uint8_t btn;
    int32_t x;
    int32_t y;
} wl_mouse_event_t;

typedef struct {
    char *title;
    int  width;
    int  height;
    int  x;
    int  y;
    int  opengl_major;
    int  opengl_minor;
    int  red_bits;
    int  green_bits;
    int  blue_bits;
    int  alpha_bits;
    int  depth_bits;
    bool vsync;
    bool warp_pointer;
} window_params_t;

void init_app(void);
void key_handler(wl_key_t key, bool is_down);
void resize_handler(int old_width, int old_height);
void mouse_handler(wl_mouse_event_t event);
void draw(uintptr_t dt_us);

extern window_params_t window_params;
