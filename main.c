#include "log.h"
#include "exit.h"

void _start(void) {
    log("Hello World");
    exit(EXIT_SUCCESS);
}
