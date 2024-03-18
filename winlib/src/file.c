#include <winlib/file.h>

io_result_t read_all(file_handle_t handle, void *buf, uintptr_t size) {
    uintptr_t orig_size = size;
    for(;;) {
        io_result_t res = os_read(handle, buf, size);
        if(res.error)
            return res;
        if(size <= res.res)
            return (io_result_t) { .res = orig_size, .error = false };

        size -= res.res;
        buf += res.res;
    }
}

io_result_t pread_all(file_handle_t handle, void *buf, uintptr_t size, uintptr_t offset) {
    uintptr_t orig_size = size;
    for(;;) {
        io_result_t res = os_pread(handle, buf, size, offset);
        if(res.error)
            return res;
        if(size <= res.res)
            return (io_result_t) { .res = orig_size, .error = false };

        offset += res.res;
        size -= res.res;
        buf += res.res;
    }
}

io_result_t write_all(file_handle_t handle, void *buf, uintptr_t size) {
    uintptr_t orig_size = size;
    for(;;) {
        io_result_t res = os_write(handle, buf, size);
        if(res.error)
            return res;
        if(size <= res.res)
            return (io_result_t) { .res = orig_size, .error = false };

        size -= res.res;
        buf += res.res;
    }
}
