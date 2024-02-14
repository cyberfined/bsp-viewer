#include "platform.h"
#include "../string.h"
#include "../error.h"

#define UNKNOWN_ERROR "Unknown error"

char* os_error(uintptr_t error, char *buf, uintptr_t size) {
    DWORD msg_success = FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        error,
        MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
        buf,
        size,
        NULL
    );
    if(!msg_success) {
        if(size >= sizeof(UNKNOWN_ERROR))
            memcpy(buf, UNKNOWN_ERROR, sizeof(UNKNOWN_ERROR));
        else
            return NULL;
    }
    return buf;
}
