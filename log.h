#pragma once

#ifdef DEBUG
    #define DEBUG(str)       log(str)
    #define DEBUGF(fmt, ...) log(fmt, __VA_ARGS__)
#else
    #define DEBUG(str)
    #define DEBUGF(fmt, ...)
#endif

__attribute__((format(printf, 1, 2)))
void log(const char *fmt, ...);
