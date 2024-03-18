#pragma once

#include <winlib/platform.h>
#include <winlib/bool.h>

#define S8(str) (s8) { str, sizeof(str) - 1 }

typedef struct {
    char      *buf;
    uintptr_t len;
} s8;

void wl_memcpy(void *dst, const void *src, uintptr_t size);
void wl_memset(void *dst, int c, uintptr_t size);
uintptr_t wl_strlen(const char *src);
intptr_t s8cmp(s8 a, s8 b);
s8 s8find(s8 a, s8 b);

static inline s8 s8fromc(const char *str) {
    return (s8) { .buf = (char*)str, .len = wl_strlen(str) };
}

static inline s8 s8slice(s8 a, uintptr_t offset, uintptr_t size) {
    return (s8) { .buf = a.buf + offset, .len = size };
}

static inline s8 s8null(void) {
    return (s8) { .buf = NULL, .len = 0 };
}

static inline bool s8isnull(s8 s) {
    return s.buf == NULL;
}

typedef struct {
    intptr_t res;
    bool     error;
} s82i_result;
s82i_result s82i(s8 a);
