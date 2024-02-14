#include "platform.h"
#include "../file.h"

io_result_t os_open(const char *filename, int mode) {
    DWORD access = 0;
    DWORD disposition = 0;
    if(mode & READ_MODE) {
        access |= GENERIC_READ;
        disposition = OPEN_EXISTING;
    }

    if(mode & WRITE_MODE) {
        if(mode & APPEND_MODE) {
            access |= FILE_APPEND_DATA;
            disposition = OPEN_ALWAYS;
        } else {
            access |= GENERIC_WRITE;
            disposition = CREATE_ALWAYS;
        }
    }

    HANDLE handle = CreateFileA(
        filename, access, 0, NULL, disposition, FILE_ATTRIBUTE_NORMAL, NULL
    );
    if(handle != INVALID_HANDLE_VALUE) {
        return (io_result_t) { .res = (uintptr_t)handle, .error = false };
    }
    return (io_result_t) { .res = GetLastError(), .error = true };
}

io_result_t os_write(file_handle_t handle, const void *buf, uintptr_t size) {
    DWORD written = 0;
    if(WriteFile(handle, buf, size, &written, NULL)) {
        return (io_result_t) { .res = written, .error = false };
    }
    return (io_result_t) { .res = GetLastError(), .error = true };
}

io_result_t os_read(file_handle_t handle, void *buf, uintptr_t size) {
    DWORD read = 0;
    if(ReadFile(handle, buf, size, &read, NULL)) {
        return (io_result_t) { .res = read, .error = false };
    }
    return (io_result_t) { .res = GetLastError(), .error = true };
}

io_result_t os_file_size(file_handle_t handle) {
    LARGE_INTEGER size = {0};

    if(!GetFileSizeEx(handle, &size)) {
        return (io_result_t) { .res = GetLastError(), .error = true };
    }
    return (io_result_t) { .res = size.QuadPart, .error = false };
}

void os_close(file_handle_t handle) {
    CloseHandle(handle);
}
