#if defined(__x86_64__) || defined(_M_X64)
#include "amd64/error.c"
#else
#error "Your architecture is not supported"
#endif
