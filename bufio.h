#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "string.h"
#include "platform.h"

typedef struct {
    uint8_t       *buf;
    uintptr_t     cap;
    uintptr_t     size;
    file_handle_t handle;
    int           error;
} buf_t;

extern buf_t stdout;
extern buf_t stderr;

void append_str(buf_t *buf, s8 str);
void append_char(buf_t *buf, uint8_t c);
void append_long(buf_t *buf, long a, unsigned int padding);
void append_ulong(buf_t *buf, unsigned long a, unsigned int padding);
void flush(buf_t *buf);

uintptr_t vfprintf(buf_t *buf, const char *fmt, va_list ap);

__attribute__((format(printf, 2, 3)))
uintptr_t fprintf(buf_t *buf, const char *fmt, ...);
