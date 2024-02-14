#include "string.h"

// TODO: rewrite in assembly for amd64
void* memcpy(void *dst, const void *src, uintptr_t size) {
    while(size--) *(char*)dst++ = *(char*)src++;
    return dst;
}

void* memset(void *dst, int c, uintptr_t size) {
    while(size--) *(char*)dst++ = c;
    return dst;
}

uintptr_t strlen(const char *src) {
    uintptr_t len = 0;
    while  (*src++) len++;
    return len;
}
