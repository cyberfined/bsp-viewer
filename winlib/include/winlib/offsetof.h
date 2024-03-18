#pragma once

#define offsetof(st, m) \
    ((uintptr_t)&(((st*)0)->m))
