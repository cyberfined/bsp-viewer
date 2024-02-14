#include "log.h"
#include "bufio.h"
#include "time.h"
#include "varg.h"

void log(const char *fmt, ...) {
    time_spec_t time;
    get_localtime(&time);

    fprintf(
        &stdout,
        "[%02d/%02d/%d %02d:%02d:%02d] ",
        time.day,
        time.month,
        time.year,
        time.hour,
        time.min,
        time.sec
    );

    va_list ap;
    va_start(ap, fmt);
    vfprintf(&stdout, fmt, ap);
    va_end(ap);

    append_char(&stdout, '\n');
    flush(&stdout);
}
