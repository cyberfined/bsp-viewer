#pragma once

#include <winlib/bufio.h>

#ifdef WINLIB_DEBUG
    #define WL_DEBUG(fmt, ...) wl_log(fmt, __VA_ARGS__)
#else
    #define WL_DEBUG(fmt, ...)
#endif

#define _STR(x) #x
#define STR(x) _STR(x)

#define wl_log(fmt, ...) \
    do { \
        appendf(&stdout_buf,__FILE__ ":" STR(__LINE__) ": " fmt "\n", ## __VA_ARGS__); \
        flush_buf(&stdout_buf); \
    } while(0)
