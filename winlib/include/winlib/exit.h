#pragma once

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

__attribute__((noreturn))
void os_exit(int status);
