#include <winlib/gl.h>
#include <winlib/log.h>
#include <winlib/exit.h>

#if defined(_WIN32)
#include <windows.h>

static inline void* load_gl_proc(const char *func_name) {
    return (void*)wglGetProcAddress(func_name);
}
#elif defined(__linux__)
#include <GL/glx.h>

static inline void* load_gl_proc(const char *func_name) {
    return (void*)glXGetProcAddressARB((const GLubyte*)func_name);
}
#else
#error "your os is not supported"
#endif

#undef GL_FUNC
#define GL_FUNC(type, name) type gl ## name = NULL;
GL_FUNCS_LIST

void load_gl(void) {
    #undef GL_FUNC
    #define GL_FUNC(type, name) \
        gl ## name = (type)load_gl_proc("gl" # name); \
        if(!gl ## name) { \
            wl_log("Failed to load gl" # name " function"); \
            os_exit(EXIT_FAILURE); \
        }

    GL_FUNCS_LIST
}
