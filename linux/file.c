#include "../file.h"
#include "syscall.h"
#include "file.h"

io_result_t os_open(const char *filename, int mode) {
    int flags = 0;

    if(mode & WRITE_MODE) {
        if(mode & READ_MODE)
            flags = O_RDWR;
        else
            flags = O_WRONLY;

        flags |= O_CREAT;
        if(mode & APPEND_MODE)
            flags |= O_APPEND;
        else
            flags |= O_TRUNC;
    }

    int res = syscall3(__NR_OPEN, (long)filename, flags, 0644);
    if(res < 0)
        return (io_result_t) { .res = -res, .error = true };
    return (io_result_t) { .res = res, .error = false };
}

io_result_t os_write(file_handle_t handle, const void *buf, uintptr_t size) {
    int res = syscall3(__NR_WRITE, handle, (long)buf, size);
    if(res < 0)
        return (io_result_t) { .res = -res, .error = true };
    return (io_result_t) { .res = res, .error = false };
}

io_result_t os_read(file_handle_t handle, void *buf, uintptr_t size) {
    int res = syscall3(__NR_READ, handle, (long)buf, size);
    if(res < 0)
        return (io_result_t) { .res = -res, .error = true };
    return (io_result_t) { .res = res, .error = false };
}

io_result_t os_file_size(file_handle_t handle) {
    stat_t stat;
    int res = syscall2(__NR_FSTAT, handle, (long)&stat);
    if(res < 0)
        return (io_result_t) { .res = -res, .error = true };
    return (io_result_t) { .res = stat.st_size, .error = false };
}

void os_close(file_handle_t handle) {
    syscall1(__NR_CLOSE, handle);
}
