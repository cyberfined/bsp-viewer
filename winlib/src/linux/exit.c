#include <winlib/exit.h>
#include <winlib/linux/syscall.h>

void os_exit(int status) {
    syscall1(__NR_EXIT, status);
    __builtin_unreachable();
}
