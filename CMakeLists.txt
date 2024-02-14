cmake_minimum_required(VERSION 3.28)
project(bsp-viewer)

set(SOURCES
    bufio.c
    string.c
    log.c
    load_gl.c
    file.c
    shader.c
    app.c
)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3 -O0 -Wall -Wno-builtin-declaration-mismatch -fno-builtin -std=c11")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostartfiles -nostdlib -nodefaultlibs")

IF(WIN32)
    list(APPEND SOURCES
         win32/main.c
         win32/file.c
         win32/time.c
         win32/exit.c
         win32/error.c
    )
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-asynchronous-unwind-tables")
ENDIF()

add_executable(bsp-viewer ${SOURCES})

IF(WIN32)
    target_link_libraries(bsp-viewer kernel32 opengl32)
ENDIF()
