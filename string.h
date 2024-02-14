#pragma once

#include <stdint.h>

#define S8(str) (s8) { (uint8_t*)str, sizeof(str) - 1 }

typedef struct {
    uint8_t   *buf;
    uintptr_t len;
} s8;

void* memcpy(void *dst, const void *src, uintptr_t size);
void* memset(void *dst, int c, uintptr_t size);
uintptr_t strlen(const char *src);
