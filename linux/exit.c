#include "../exit.h"
#include "syscall.h"

void exit(int status) {
    syscall1(__NR_EXIT, status);
}
