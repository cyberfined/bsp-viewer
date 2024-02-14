#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    KEY_UNKNOWN,
    KEY_BACK,
    KEY_TAB,
    KEY_RETURN,
    KEY_SHIFT,
    KEY_LSHIFT,
    KEY_RSHIFT,
    KEY_PAGEUP,
    KEY_PAGEDOWN,
    KEY_CTRL,
    KEY_LCTRL,
    KEY_RCTRL,
    KEY_ALT,
    KEY_LALT,
    KEY_RALT,
    KEY_ESC,
    KEY_SPACE,
    KEY_END,
    KEY_HOME,
    KEY_LEFT,
    KEY_UP,
    KEY_RIGHT,
    KEY_DOWN,
    KEY_DEL,
    KEY_WIN,
    KEY_LWIN,
    KEY_RWIN,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_CAPS,
    KEY_INSERT,
    KEY_PAUSE,
    KEY_NUMLOCK,
    KEY_SCROLL,
    KEY_PLUS,
    KEY_COMMA,
    KEY_MINUS,
    KEY_PERIOD,
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
} key_t;

typedef enum {
    MOUSE_NONE,
    MOUSE_LEFT,
    MOUSE_MIDDLE,
    MOUSE_RIGHT,
} mouse_btn_t;

typedef struct {
    uint8_t  btn;
    bool     is_down;
    uint32_t x;
    uint32_t y;
} mouse_event_t;

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
} window_params_t;

void init_app(void);
void key_handler(key_t key, bool is_down);
void resize_handler(int width, int height);
void mouse_handler(mouse_event_t event);
void draw(void);

extern window_params_t window_params;
