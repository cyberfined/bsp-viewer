#pragma once

#include "platform.h"

#define O_RDONLY 00
#define O_WRONLY 01
#define O_RDWR   02
#define O_CREAT  0100
#define O_TRUNC	 01000
#define O_APPEND 02000

typedef struct {
    int64_t tv_sec;
    int64_t tv_nsec;
} timespec_t;

typedef struct {
    uint64_t   st_dev;
    uint64_t   st_ino;
    uint64_t   st_nlink;
    int32_t    st_mode;
    int32_t    st_uid;
    int32_t    st_gid;
    int32_t    _pad0;
    uint64_t   st_rdev;
    uint64_t   st_size;
    uint64_t   st_blksize;
    uint64_t   st_blocks;
    timespec_t st_atim;
    timespec_t st_mtim;
    timespec_t st_ctim;
    int64_t    __unused[3];
} stat_t;
