cmake_minimum_required(VERSION 3.13)
project(bsp-viewer)

add_subdirectory(winlib)

set(SOURCES
    app.c
    bsp.c
)

set(CMAKE_CONFIGURATION_TYPES "Debug" "Release")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS_RELEASE "-O2 -Wall -Wno-builtin-declaration-mismatch -fno-builtin")
set(CMAKE_C_FLAGS_DEBUG "-g3 -O0 -DWINLIB_DEBUG -Wall -Wno-builtin-declaration-mismatch -fno-builtin")

IF(WIN32)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-asynchronous-unwind-tables")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-asynchronous-unwind-tables")
ENDIF()

add_executable(bsp-viewer ${SOURCES})

IF(WIN32)
    target_link_options(bsp-viewer PRIVATE "-nostdlib" PRIVATE "-nodefaultlibs")
ENDIF()

target_include_directories(bsp-viewer PRIVATE winlib/include)
target_link_libraries(bsp-viewer winlib)
