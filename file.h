#pragma once

#include "bool.h"
#include "platform.h"

typedef struct {
    uintptr_t res;
    bool      error;
} io_result_t;

typedef enum {
    READ_MODE   = 1,
    WRITE_MODE  = 2,
    APPEND_MODE = 4,
} io_mode_t;

io_result_t os_open(const char *filename, int mode);
io_result_t os_write(file_handle_t handle, const void *buf, uintptr_t size);
io_result_t os_read(file_handle_t handle, void *buf, uintptr_t size);
io_result_t os_file_size(file_handle_t handle);
void os_close(file_handle_t handle);

io_result_t read_all(file_handle_t handle, void *buf, uintptr_t size);
