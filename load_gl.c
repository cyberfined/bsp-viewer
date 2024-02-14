#include "gl.h"
#include "log.h"
#include "exit.h"

#if defined(_WIN32)
    #include <windows.h>

    static inline void* load_gl_proc(const char *func_name) {
        return (void*)wglGetProcAddress(func_name);
    }
#endif

#undef GL_FUNC
#define GL_FUNC(type, name) type gl ## name = NULL;
GL_FUNCS_LIST

void load_gl(void) {
    #undef GL_FUNC
    #define GL_FUNC(type, name) \
        gl ## name = (type)load_gl_proc("gl" # name); \
        if(!gl ## name) { \
            log("Failed to load gl" # name " function"); \
            exit(EXIT_FAILURE); \
        }

    GL_FUNCS_LIST
}
