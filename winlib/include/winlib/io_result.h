#pragma once

#include <winlib/bool.h>
#include <winlib/platform.h>

typedef struct {
    uintptr_t res;
    bool      error;
} io_result_t;
