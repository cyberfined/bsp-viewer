#include <winlib/string.h>

#if !defined(__x86_64__) && !defined(_M_X64)
void* wl_memcpy(void *dst, const void *src, uintptr_t size) {
    while(size--) *(char*)dst++ = *(char*)src++;
    return dst;
}

void* wl_memset(void *dst, int c, uintptr_t size) {
    while(size--) *(char*)dst++ = c;
    return dst;
}
#endif

uintptr_t wl_strlen(const char *src) {
    uintptr_t len = 0;
    while  (*src++) len++;
    return len;
}

intptr_t s8cmp(s8 a, s8 b) {
    intptr_t len_diff = a.len - b.len;
    if(len_diff != 0)
        return len_diff;

    for(uintptr_t i = 0; i < a.len; i++) {
        int8_t diff = a.buf[i] - b.buf[i];
        if(diff != 0)
            return diff;
    }

    return 0;
}

s8 s8find(s8 a, s8 b) {
    if(a.len < b.len)
        return s8null();

    for(uintptr_t i = 0; i < a.len - b.len + 1; i++) {
        s8 slice = s8slice(a, i, b.len);
        if(!s8cmp(slice, b))
            return slice;
    }

    return s8null();
}

s82i_result s82i(s8 a) {
    intptr_t res = 0;
    char *from = a.buf;
    if(*from == '-')
        from++;

    while(from < a.buf + a.len) {
        if(*from >= '0' && *from <= '9') {
            res = res * 10 + *from - '0';
        } else {
            return (s82i_result) { .res = 0, .error = true };
        }
        from++;
    }

    if(a.buf[0] == '-')
        res = -res;

    return (s82i_result) { .res = res, .error = false };
}
