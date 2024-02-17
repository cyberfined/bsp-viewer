#pragma once

#include "platform.h"

typedef struct {
    uint32_t year;
    uint32_t month;
    uint32_t day;
    uint32_t hour;
    uint32_t min;
    uint32_t sec;
} time_spec_t;

void get_localtime(time_spec_t *time);
