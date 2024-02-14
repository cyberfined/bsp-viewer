#include <windows.h>
#include "../exit.h"

void exit(int status) {
    ExitProcess(status);
}
