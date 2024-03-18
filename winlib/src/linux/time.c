#include <winlib/time.h>
#include <winlib/linux/syscall.h>

#define CLOCK_MONOTONIC 1

typedef struct {
    uint64_t tv_sec;
    uint64_t tv_nsec;
} timespec_t;

uintptr_t get_time_us(void) {
    timespec_t t;
    syscall2(__NR_CLOCK_GETTIME, CLOCK_MONOTONIC, (long)&t);
    return t.tv_sec * 1000000 + t.tv_nsec / 1000;
}
