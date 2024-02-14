#include "bufio.h"
#include "file.h"
#include "varg.h"

#define STD_BUF_SIZE 1024

static char stdout_buf[STD_BUF_SIZE];
buf_t stdout = {
    .buf    = (uint8_t*)stdout_buf,
    .cap    = STD_BUF_SIZE,
    .size   = 0,
    .error  = false,
};

static char stderr_buf[STD_BUF_SIZE];
buf_t stderr = {
    .buf   = (uint8_t*)stderr_buf,
    .cap   = STD_BUF_SIZE,
    .size  = 0,
    .error = 0,
};

void append_str(buf_t *buf, s8 str) {
    if(buf->error)
        return;

    for(;;) {
        uintptr_t rem_size = buf->cap - buf->size;
        if(str.len <= rem_size) {
            memcpy(&buf->buf[buf->size], str.buf, str.len);
            buf->size += str.len;
            break;
        }

        if(rem_size != 0) {
            memcpy(&buf->buf[buf->size], str.buf, rem_size);
            str.len -= rem_size;
            str.buf += rem_size;
        }
        io_result_t res = os_write(buf->handle, buf->buf, buf->cap);
        if(res.error) {
            buf->error = res.res;
            break;
        }

        buf->size = 0;
    }
}

void append_char(buf_t *buf, uint8_t c) {
    append_str(buf, (s8) { .buf = &c, .len = 1 });
}

void append_long(buf_t *buf, long a, unsigned int padding) {
    unsigned char numbuf[64];
    bool sign;
    if(a < 0) {
        a = -a;
        if(padding > 0)
            padding--;
        sign = true;
    } else {
        sign = false;
    }
    uintptr_t offset = sizeof(numbuf) / sizeof(*numbuf);

    do {
        numbuf[--offset] = a % 10 + '0';
        a /= 10;
    } while(a);

    uintptr_t len = sizeof(numbuf) / sizeof(*numbuf) - offset;
    if(len < padding) {
        padding -= len;
        uintptr_t cpy = padding < offset ? padding : offset - 1;
        offset -= cpy;
        memset(&numbuf[offset], '0', cpy);

        while(cpy < padding) {
            append_char(buf, '0');
            cpy++;
        }
    }

    if(sign)
        numbuf[--offset] = '-';
    len = sizeof(numbuf) / sizeof(*numbuf) - offset;
    append_str(buf, (s8) { .buf = &numbuf[offset], .len = len });
}

void append_ulong(buf_t *buf, unsigned long a, unsigned int padding) {
    unsigned char numbuf[64];
    uintptr_t offset = sizeof(numbuf) / sizeof(*numbuf);

    do {
        numbuf[--offset] = a % 10 + '0';
        a /= 10;
    } while(a);

    uintptr_t len = sizeof(numbuf) / sizeof(*numbuf) - offset;
    if(len < padding) {
        padding -= len;
        uintptr_t cpy = padding <= offset ? padding : offset;
        offset -= cpy;
        len += cpy;
        memset(&numbuf[offset], '0', cpy);

        while(cpy < padding) {
            append_char(buf, '0');
            cpy++;
        }
    }

    append_str(buf, (s8) { .buf = &numbuf[offset], .len = len });
}

void flush(buf_t *buf) {
    if(buf->error || !buf->size)
        return;

    io_result_t res = os_write(buf->handle, buf->buf, buf->size);
    if(res.error) {
        buf->error = res.res;
        return;
    }

    buf->size = 0;
}

typedef struct {
    uintptr_t    len;
    unsigned int result;
} atoi_result_t;

static inline atoi_result_t a2i(const uint8_t *buf) {
    uintptr_t result = 0;
    const uint8_t *end = buf;

    while(*end >= '0' && *end <= '9') {
        result = result * 10 + (*end - '0');
        end++;
    }

    return (atoi_result_t) {
        .len    = end - buf,
        .result = result,
    };
}

uintptr_t vfprintf(buf_t *buf, const char *_fmt, va_list ap) {
    bool is_format = false;
    const uint8_t *fmt = (uint8_t*)_fmt;
    uint8_t *copy_from = NULL;
    uintptr_t written = 0;
    atoi_result_t atoi_res = {0};

    while(*fmt) {
        if(*fmt == '%') {
            is_format = true;
            if(copy_from) {
                uintptr_t len = fmt - copy_from;
                append_str(buf, (s8) { .buf = copy_from, .len = len });
                if(buf->error)
                    goto exit;
                written += len;
                copy_from = NULL;
            }
            fmt++;
        } else if(is_format) {
            int fmt_size = 1;
            unsigned long num;
            uint8_t *str;
            char next_fmt;

            switch(*fmt) {
            case 'l':
                fmt_size = 2;
                next_fmt = *(fmt + 1);
                switch(next_fmt) {
                case 'd':
                    num = va_arg(ap, long);
                    append_long(buf, num, atoi_res.result);
                    break;
                case 'u':
                    num = va_arg(ap, unsigned long);
                    append_ulong(buf, num, atoi_res.result);
                    break;
                default:
                    append_str(buf, S8("%l"));
                    append_char(buf, next_fmt);
                    break;
                }
                break;
            case 'd':
                num = va_arg(ap, int);
                append_long(buf, num, atoi_res.result);
                break;
            case 'u':
                num = va_arg(ap, unsigned int);
                append_ulong(buf, num, atoi_res.result);
                break;
            case 's':
                str = va_arg(ap, uint8_t*);
                append_str(buf, (s8) { .buf = str, .len = strlen((char*)str) });
                break;
            case '0':
                fmt++;
                atoi_res = a2i(fmt);
                fmt += atoi_res.len;
                continue;
            case '%':
                append_char(buf, '%');
                break;
            default:
                append_char(buf, '%');
                append_char(buf, *fmt);
                break;
            }
            if(buf->error)
                goto exit;
            atoi_res.result = 0;
            is_format = false;
            fmt += fmt_size;
        } else {
            if(!copy_from) {
                copy_from = (uint8_t*)fmt;
            }
            fmt++;
        }
    }

    if(copy_from) {
        uintptr_t len = fmt - copy_from;
        append_str(buf, (s8) { .buf = copy_from, .len = len });
        if(!buf->error)
            written += len;
    }

exit:
    return written;
}

uintptr_t fprintf(buf_t *buf, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    uintptr_t res = vfprintf(buf, fmt, ap);
    va_end(ap);
    return res;
}
