#pragma once

#if defined(__x86_64__) || defined(_M_X64)
#include <winlib/linux/amd64/syscall.h>
#else
#error "Your architecture is not supported"
#endif
