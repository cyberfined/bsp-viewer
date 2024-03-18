cmake_minimum_required(VERSION 3.13)
project(winlib C ASM)

set(SOURCES
    src/bufio.c
    src/string.c
    src/load_gl.c
    src/file.c
    src/shader.c
    src/matrix.c
    src/camera.c
)

IF(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "x86_64")
    list(APPEND SOURCES
         src/amd64/sin.s
         src/amd64/cos.s
         src/amd64/tan.s
         src/amd64/sqrt.s
         src/amd64/memcpy.s
         src/amd64/memset.s
    )
ELSE()
    message(FATAL_ERROR "Your arch is not supported")
ENDIF()

set(CMAKE_CONFIGURATION_TYPES "Debug" "Release")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS_RELEASE "-O2 -Wall -Werror -Wno-builtin-declaration-mismatch")
set(CMAKE_C_FLAGS_DEBUG "-g3 -O0 -DWINLIB_DEBUG -Wall -Wno-builtin-declaration-mismatch")

IF(WIN32)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-builtin -fno-asynchronous-unwind-tables")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-builtin -fno-asynchronous-unwind-tables")

    list(APPEND SOURCES
         src/win32/main.c
         src/win32/file.c
         src/win32/time.c
         src/win32/exit.c
         src/win32/error.c
    )
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND SOURCES
        src/linux/main.c
        src/linux/file.c
        src/linux/time.c
        src/linux/exit.c
        src/linux/error.c
    )
ENDIF()

add_library(winlib STATIC ${SOURCES})
target_include_directories(winlib PRIVATE include)

find_package(OpenGL REQUIRED)
target_link_libraries(winlib OpenGL::OpenGL)

IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(X11 REQUIRED)
    target_link_libraries(winlib ${X11_X11_LIB} OpenGL::GLX)
ENDIF()
