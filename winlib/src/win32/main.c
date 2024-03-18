#include <windows.h>
#include <GL/gl.h>

#include "platform.h"
#include "../app.h"
#include "../file.h"
#include "../bufio.h"
#include "../log.h"

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB         0x2094
typedef HGLRC (*wglCreateContextAttribsARBProc)(HDC, HGLRC, const int*);

static HDC deviceCtx = NULL;

static inline void LogError(const char *str) {
    DWORD err = GetLastError();
    char *buf;
    DWORD msg_success = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, 
        err,
        MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
        (LPSTR)&buf,
        0,
        NULL
    );
    if(msg_success) {
        log("%s: %s", str, buf);
    } else {
        log("%s: error code %lu", str, err);
    }
    ExitProcess(err);
}

static inline void CreateContext(HWND hWnd) {
    deviceCtx = GetDC(hWnd);
    if(!deviceCtx)
        LogError("Failed to get device context");

    int num_bits = window_params.red_bits + window_params.green_bits +
                   window_params.blue_bits + window_params.alpha_bits;

    PIXELFORMATDESCRIPTOR pfd = {
        .nSize      = sizeof(PIXELFORMATDESCRIPTOR),
        .nVersion   = 1,
        .dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        .iPixelType = PFD_TYPE_RGBA,
        .cColorBits = num_bits,
        .cRedBits   = window_params.red_bits,
        .cGreenBits = window_params.green_bits,
        .cBlueBits  = window_params.blue_bits,
        .cAlphaBits = window_params.alpha_bits,
        .cDepthBits = window_params.depth_bits,
    };

    int pixelFmt = ChoosePixelFormat(deviceCtx, &pfd);
    if(!pixelFmt)
        LogError("Failed to choose pixel format");
    if(!SetPixelFormat(deviceCtx, pixelFmt, &pfd))
        LogError("Failed to set pixel format");

    HGLRC tmpGLCtx = wglCreateContext(deviceCtx);
    if(!tmpGLCtx)
        LogError("Failed to create legacy OpenGL context");
    if(!wglMakeCurrent(deviceCtx, tmpGLCtx))
        LogError("Failed to set legacy OpenGL as current");

    wglCreateContextAttribsARBProc wglCreateContextAttribsARB =
        (wglCreateContextAttribsARBProc)wglGetProcAddress("wglCreateContextAttribsARB");
    if(!wglCreateContextAttribsARB)
        LogError("wglCreateContextAttribsARB is not supported");

    int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, window_params.opengl_major,
        WGL_CONTEXT_MINOR_VERSION_ARB, window_params.opengl_minor,
        WGL_CONTEXT_FLAGS_ARB, 0,
        0
    };
    HGLRC GLCtx = wglCreateContextAttribsARB(deviceCtx, tmpGLCtx, attribs);
    if(!GLCtx)
        LogError("Failed to create OpenGL context");
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(tmpGLCtx);

    if(!wglMakeCurrent(deviceCtx, GLCtx))
        LogError("Failed to set current OpenGL context");
}

static uint8_t keymap[] = {
    KEY_BACK,
    KEY_TAB,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_RETURN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_SHIFT,
    KEY_CTRL,
    KEY_ALT,
    KEY_PAUSE,
    KEY_CAPS,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_ESC,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_SPACE,
    KEY_PAGEUP,
    KEY_PAGEDOWN,
    KEY_END,
    KEY_HOME,
    KEY_LEFT,
    KEY_UP,
    KEY_RIGHT,
    KEY_DOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_INSERT,
    KEY_DEL,
    KEY_UNKNOWN,
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
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
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
    KEY_LWIN,
    KEY_RWIN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
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
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
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
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_NUMLOCK,
    KEY_SCROLL,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_LSHIFT,
    KEY_RSHIFT,
    KEY_LCTRL,
    KEY_RCTRL,
    KEY_LALT,
    KEY_RALT,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_UNKNOWN,
    KEY_PLUS,
    KEY_COMMA,
    KEY_MINUS,
    KEY_PERIOD,
};

static inline key_t mapkey(WPARAM wParam) {
    wParam -= 8;
    if(wParam < 0 || wParam > sizeof(keymap) / sizeof(*keymap))
        return KEY_UNKNOWN;
    return keymap[wParam];
}

static LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    mouse_event_t mouse_event;

    switch(uMsg) {
    case WM_CREATE:
        CreateContext(hWnd);
        init_app();
        break;
    case WM_PAINT:
        draw();
        wglSwapLayerBuffers(deviceCtx, WGL_SWAP_MAIN_PLANE);
        break;
    case WM_DESTROY:
        ExitProcess(0);
        break;
    case WM_KEYDOWN:
        key_handler(mapkey(wParam), true);
        break;
    case WM_KEYUP:
        key_handler(mapkey(wParam), false);
        break;
    case WM_LBUTTONDOWN:
        mouse_event.btn = MOUSE_LEFT;
        mouse_event.is_down = true;
        goto mouse;
    case WM_LBUTTONUP:
        mouse_event.btn = MOUSE_LEFT;
        mouse_event.is_down = false;
        goto mouse;
    case WM_MBUTTONDOWN:
        mouse_event.btn = MOUSE_MIDDLE;
        mouse_event.is_down = true;
        goto mouse;
    case WM_MBUTTONUP:
        mouse_event.btn = MOUSE_MIDDLE;
        mouse_event.is_down = false;
        goto mouse;
    case WM_RBUTTONDOWN:
        mouse_event.btn = MOUSE_RIGHT;
        mouse_event.is_down = true;
        goto mouse;
    case WM_RBUTTONUP:
        mouse_event.btn = MOUSE_RIGHT;
        mouse_event.is_down = false;
        goto mouse;
    case WM_MOUSEMOVE:
        mouse_event.btn = MOUSE_NONE;
        mouse_event.is_down = false;
        goto mouse;
    case WM_SIZE:
        resize_handler(LOWORD(lParam), HIWORD(lParam));
        break;
    default:
        result = DefWindowProc(hWnd, uMsg, wParam, lParam);
        break;
    }

    return result;

mouse:
    mouse_event.x = LOWORD(lParam);
    mouse_event.y = HIWORD(lParam);
    mouse_handler(mouse_event);
    return result;
}

void mainCRTStartup(void) {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    stdout.handle = GetStdHandle(STD_OUTPUT_HANDLE);
    stderr.handle = GetStdHandle(STD_ERROR_HANDLE);

    WNDCLASS wc = {
        .lpfnWndProc   = WindowProc,
        .hInstance     = hInstance,
        .lpszClassName = window_params.title,
    };
    ATOM cls = RegisterClassA(&wc);
    if(!cls)
        LogError("Failed to register class");

    HWND hWnd = CreateWindowA(
        window_params.title,
        window_params.title,
        WS_VISIBLE | WS_SYSMENU | WS_CAPTION | WS_SIZEBOX | WS_MAXIMIZEBOX,
        window_params.x,
        window_params.y,
        window_params.width,
        window_params.height,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    if(!hWnd)
        LogError("Failed to create window");

    MSG msg;
    while(GetMessage(&msg, hWnd, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    ExitProcess(0);
}
