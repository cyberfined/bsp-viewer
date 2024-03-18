#include <winlib/exit.h>
#include <winlib/platform.h>
#include <winlib/log.h>
#include <winlib/time.h>
#include <winlib/app.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>

typedef GLXContext (*glXCreateContextAttribsARBProc)(
    Display*,
    GLXFBConfig,
    GLXContext,
    Bool,
    const int*
);

typedef void (*glXSwapIntervalEXTProc) (Display*, GLXDrawable, int);

typedef enum {
    XKEY_ESC      = 9,
    XKEY_1        = 10,
    XKEY_2        = 11,
    XKEY_3        = 12,
    XKEY_4        = 13,
    XKEY_5        = 14,
    XKEY_6        = 15,
    XKEY_7        = 16,
    XKEY_8        = 17,
    XKEY_9        = 18,
    XKEY_0        = 19,
    XKEY_MINUS    = 20,
    XKEY_PLUS     = 21,
    XKEY_BACK     = 22,
    XKEY_TAB      = 23,
    XKEY_Q        = 24,
    XKEY_W        = 25,
    XKEY_E        = 26,
    XKEY_R        = 27,
    XKEY_T        = 28,
    XKEY_Y        = 29,
    XKEY_U        = 30,
    XKEY_I        = 31,
    XKEY_O        = 32,
    XKEY_P        = 33,
    XKEY_RETURN   = 36,
    XKEY_LCTRL    = 37,
    XKEY_A        = 38,
    XKEY_S        = 39,
    XKEY_D        = 40,
    XKEY_F        = 41,
    XKEY_G        = 42,
    XKEY_H        = 43,
    XKEY_J        = 44,
    XKEY_K        = 45,
    XKEY_L        = 46,
    XKEY_LSHIFT   = 50,
    XKEY_Z        = 52,
    XKEY_X        = 53,
    XKEY_C        = 54,
    XKEY_V        = 55,
    XKEY_B        = 56,
    XKEY_N        = 57,
    XKEY_M        = 58,
    XKEY_COMMA    = 59,
    XKEY_PERIOD   = 60,
    XKEY_RSHIFT   = 62,
    XKEY_LALT     = 64,
    XKEY_SPACE    = 65,
    XKEY_CAPS     = 66,
    XKEY_F1       = 67,
    XKEY_F2       = 68,
    XKEY_F3       = 69,
    XKEY_F4       = 70,
    XKEY_F5       = 71,
    XKEY_F6       = 72,
    XKEY_F7       = 73,
    XKEY_F8       = 74,
    XKEY_F9       = 75,
    XKEY_F10      = 76,
    XKEY_NUMLOCK  = 77,
    XKEY_SCROLL   = 78,
    XKEY_F11      = 95,
    XKEY_F12      = 96,
    XKEY_RCTRL    = 105,
    XKEY_RALT     = 108,
    XKEY_HOME     = 110,
    XKEY_UP       = 111,
    XKEY_PAGEUP   = 112,
    XKEY_LEFT     = 113,
    XKEY_RIGHT    = 114,
    XKEY_END      = 115,
    XKEY_DOWN     = 116,
    XKEY_PAGEDOWN = 117,
    XKEY_INSERT   = 118,
    XKEY_DEL      = 119,
    XKEY_PAUSE    = 127,
    XKEY_LWIN     = 133,
} xkeycode_t;

static uint8_t keymap[] = {
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_ESC,
    WL_KEY_1,
    WL_KEY_2,
    WL_KEY_3,
    WL_KEY_4,
    WL_KEY_5,
    WL_KEY_6,
    WL_KEY_7,
    WL_KEY_8,
    WL_KEY_9,
    WL_KEY_0,
    WL_KEY_MINUS,
    WL_KEY_PLUS,
    WL_KEY_BACK,
    WL_KEY_TAB,
    WL_KEY_Q,
    WL_KEY_W,
    WL_KEY_E,
    WL_KEY_R,
    WL_KEY_T,
    WL_KEY_Y,
    WL_KEY_U,
    WL_KEY_I,
    WL_KEY_O,
    WL_KEY_P,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_RETURN,
    WL_KEY_LCTRL,
    WL_KEY_A,
    WL_KEY_S,
    WL_KEY_D,
    WL_KEY_F,
    WL_KEY_G,
    WL_KEY_H,
    WL_KEY_J,
    WL_KEY_K,
    WL_KEY_L,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_LSHIFT,
    WL_KEY_UNKNOWN,
    WL_KEY_Z,
    WL_KEY_X,
    WL_KEY_C,
    WL_KEY_V,
    WL_KEY_B,
    WL_KEY_N,
    WL_KEY_M,
    WL_KEY_COMMA,
    WL_KEY_PERIOD,
    WL_KEY_UNKNOWN,
    WL_KEY_RSHIFT,
    WL_KEY_UNKNOWN,
    WL_KEY_LALT,
    WL_KEY_SPACE,
    WL_KEY_CAPS,
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
    WL_KEY_NUMLOCK,
    WL_KEY_SCROLL,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_F11,
    WL_KEY_F12,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_RCTRL,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_RALT,
    WL_KEY_UNKNOWN,
    WL_KEY_HOME,
    WL_KEY_UP,
    WL_KEY_PAGEUP,
    WL_KEY_LEFT,
    WL_KEY_RIGHT,
    WL_KEY_END,
    WL_KEY_DOWN,
    WL_KEY_PAGEDOWN,
    WL_KEY_INSERT,
    WL_KEY_DEL,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_PAUSE,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_UNKNOWN,
    WL_KEY_LWIN,
};

static inline bool is_ext_unsupported(s8 exts, s8 ext) {
    return s8isnull(s8find(exts, ext));
}

static inline void key_process(XKeyEvent *event, bool is_down) {
    if(event->keycode >= 0 && event->keycode < sizeof(keymap) / sizeof(*keymap))
        key_handler(keymap[event->keycode], is_down);
}

static inline uint8_t x_btn_map(int state) {
    uint8_t btn = WL_MOUSE_NONE;
    if(state & Button1Mask)
        btn |= WL_MOUSE_LEFT;
    if(state & Button2Mask)
        btn |= WL_MOUSE_MIDDLE;
    if(state & Button3Mask)
        btn |= WL_MOUSE_RIGHT;
    return btn;
}

int main(void) {
    int result = EXIT_FAILURE;

    Display *display = XOpenDisplay(NULL);
    if(display == NULL) {
        wl_log("Failed to open display");
        goto exit;
    }

    int screen_id = DefaultScreen(display);
    int glx_major = 0, glx_minor = 0;
    glXQueryVersion(display, &glx_major, &glx_minor);
    if(glx_major <= 1 && glx_minor < 2) {
        wl_log("GLX 1.2 or greater is required, current version is %d.%d",
               glx_major,
               glx_minor);
        goto exit;
    }
    WL_DEBUG("GLX version: %d.%d", glx_major, glx_minor);

    GLint fb_config_attributes[] = {
        GLX_X_RENDERABLE,  True,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE,   GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
        GLX_RED_SIZE,      window_params.red_bits,
        GLX_GREEN_SIZE,    window_params.green_bits,
        GLX_BLUE_SIZE,     window_params.blue_bits,
        GLX_ALPHA_SIZE,    window_params.alpha_bits,
        GLX_DEPTH_SIZE,    window_params.depth_bits,
        GLX_DOUBLEBUFFER,  1,
        None
    };

    int fb_count;
    GLXFBConfig *fb_configs = glXChooseFBConfig(
        display,
        screen_id,
        fb_config_attributes,
        &fb_count
    );
    if(fb_count == 0) {
        wl_log("Failed to retrieve frame buffers configs");
        goto exit;
    }
    GLXFBConfig fb_config = fb_configs[0];
    XFree(fb_configs);

    XVisualInfo *visual = glXGetVisualFromFBConfig(display, fb_config);
    if(visual == NULL) {
        wl_log("Failed to get visual id");
        goto exit;
    }

    XSetWindowAttributes window_attributes = {
        .colormap = XCreateColormap(
            display,
            RootWindow(display, screen_id),
            visual->visual,
            AllocNone
        ),
        .event_mask = KeyPressMask | KeyReleaseMask |
                      PointerMotionMask |
                      ButtonPressMask | ButtonReleaseMask | StructureNotifyMask,
        .override_redirect = false,
    };
    Window window = XCreateWindow(
        display,
        RootWindow(display, screen_id),
        window_params.x, window_params.y,
        window_params.width, window_params.height,
        0,
        window_params.depth_bits,
        InputOutput,
        visual->visual,
        CWEventMask | CWColormap,
        &window_attributes
    );
    XStoreName(display, window, window_params.title);

    s8 glx_extensions = s8fromc(glXQueryExtensionsString(display, screen_id));
    if(is_ext_unsupported(glx_extensions, S8("GLX_ARB_create_context"))) {
        wl_log("GLX_ARB_create_context is not supported");
        goto exit;
    }

    if(is_ext_unsupported(glx_extensions, S8("GLX_EXT_swap_control"))) {
        wl_log("GLX_EXT_swap_control is not supported");
        goto exit;
    }

    glXCreateContextAttribsARBProc glXCreateContextAttribsARB;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB(
        (const GLubyte*)"glXCreateContextAttribsARB"
    );

    glXSwapIntervalEXTProc glXSwapIntervalEXT;
    glXSwapIntervalEXT = (glXSwapIntervalEXTProc)glXGetProcAddressARB(
        (const GLubyte*)"glXSwapIntervalEXT"
    );

    int context_attributes[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, window_params.opengl_major,
        GLX_CONTEXT_MINOR_VERSION_ARB, window_params.opengl_minor,
        GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
        None
    };

    GLXContext gl_context;
    gl_context = glXCreateContextAttribsARB(
        display, fb_config, 0, true, context_attributes
    );
    XSync(display, false);
    glXMakeCurrent(display, window, gl_context);

    Atom atomWmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, &atomWmDeleteWindow, 1);

    XClearWindow(display, window);
    XMapWindow(display, window);

    init_app();
    glXSwapIntervalEXT(display, window, window_params.vsync);

    XEvent event;
    wl_mouse_event_t wl_mouse_event;
    uintptr_t current_time, elapsed_time = get_time_us();
    uintptr_t old_width, old_height;

    for(;;) {
        while(XPending(display)) {
            XNextEvent(display, &event);
            switch(event.type) {
            case ConfigureNotify:
                old_width = window_params.width;
                old_height = window_params.height;
                window_params.width = event.xconfigure.width;
                window_params.height = event.xconfigure.height;
                resize_handler(old_width, old_height);
                break;
            case MotionNotify:
                wl_mouse_event = (wl_mouse_event_t) {
                    .btn = x_btn_map(event.xmotion.state),
                    .x   = event.xmotion.x,
                    .y   = event.xmotion.y,
                };

                if(window_params.warp_pointer) {
                    wl_mouse_event.x -= window_params.width / 2;
                    wl_mouse_event.y -= window_params.height / 2;
                }

                mouse_handler(wl_mouse_event);

                if(window_params.warp_pointer &&
                  (wl_mouse_event.x || wl_mouse_event.y))
                {
                    XWarpPointer(
                        display,
                        None, window,
                        0, 0, 0, 0,
                        window_params.width / 2,
                        window_params.height / 2
                    );
                }

                break;
            case ButtonPress:
            case ButtonRelease:
                wl_mouse_event = (wl_mouse_event_t) {
                    .btn = x_btn_map(event.xbutton.state),
                    .x   = event.xbutton.x,
                    .y   = event.xbutton.y,
                };

                if(window_params.warp_pointer) {
                    wl_mouse_event.x -= window_params.width / 2;
                    wl_mouse_event.y -= window_params.height / 2;
                }

                mouse_handler(wl_mouse_event);

                if(window_params.warp_pointer &&
                  (wl_mouse_event.x || wl_mouse_event.y))
                {
                    XWarpPointer(
                        display,
                        None, window,
                        0, 0, 0, 0,
                        window_params.width / 2,
                        window_params.height / 2
                    );
                }
                break;
            case KeyPress:
            case KeyRelease:
                key_process(&event.xkey, event.type == KeyPress);
                break;
            case ClientMessage:
            case DestroyNotify:
            case UnmapNotify:
                goto exit;
                break;
            }
        }

        current_time = get_time_us();
        draw(current_time - elapsed_time);
        elapsed_time = current_time;

        glXSwapBuffers(display, window);
    }

    result = EXIT_SUCCESS;
exit:
    os_exit(result);
}
