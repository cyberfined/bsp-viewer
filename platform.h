#pragma once

#if defined(_WIN32)
  #include "win32/platform.h"
#elif defined(__linux__)
  #include "linux/platform.h"
#else
  #error "your os is not suported"
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif
