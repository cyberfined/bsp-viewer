#pragma once

#include <winlib/platform.h>
#include <winlib/bool.h>
#include <winlib/string.h>
#include <winlib/platform.h>
#include <winlib/varg.h>

typedef struct {
    uint8_t       *buf;
    uintptr_t     cap;
    uintptr_t     size;
    file_handle_t handle;
    uintptr_t     error;
} buf_t;

extern buf_t stdout_buf;
extern buf_t stderr_buf;

void append_str(buf_t *buf, s8 str);
void append_char(buf_t *buf, uint8_t c);
void append_long(buf_t *buf, long a, unsigned int padding);
void append_ulong(buf_t *buf, unsigned long a, unsigned int padding);
void append_double(buf_t *buf, double a);
void flush_buf(buf_t *buf);

uintptr_t vappendf(buf_t *buf, const char *fmt, va_list ap);

__attribute__((format(printf, 2, 3)))
uintptr_t appendf(buf_t *buf, const char *fmt, ...);
