#include "../time.h"
#include "platform.h"

void get_localtime(time_spec_t *time) {
    SYSTEMTIME t;
    GetLocalTime(&t);
    time->year = t.wYear;
    time->month = t.wMonth;
    time->day = t.wDay;
    time->hour = t.wHour;
    time->min = t.wMinute;
    time->sec = t.wSecond;
}
