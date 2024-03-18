#pragma once

#include <winlib/bool.h>
#include <winlib/platform.h>
#include <winlib/io_result.h>

typedef enum {
    READ_MODE   = 1,
    WRITE_MODE  = 2,
    APPEND_MODE = 4,
} io_mode_t;

typedef enum {
    WL_SEEK_SET,
    WL_SEEK_CUR,
    WL_SEEK_END,
} io_seek_t;

io_result_t os_open(const char *filename, int mode);
io_result_t os_write(file_handle_t handle, const void *buf, uintptr_t size);
io_result_t os_read(file_handle_t handle, void *buf, uintptr_t size);
io_result_t os_lseek(file_handle_t handle, uintptr_t offset, io_seek_t seek);
io_result_t os_pread(file_handle_t handle, void *buf, uintptr_t size, uintptr_t offset);
io_result_t os_file_size(file_handle_t handle);
void os_close(file_handle_t handle);

io_result_t read_all(file_handle_t handle, void *buf, uintptr_t size);
io_result_t pread_all(file_handle_t handle, void *buf, uintptr_t size, uintptr_t offset);
io_result_t write_all(file_handle_t handle, void *buf, uintptr_t size);
