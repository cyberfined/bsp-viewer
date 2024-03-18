#pragma once

#include <winlib/platform.h>
#include <winlib/assert.h>

#define ARENA(buf) (arena_t) { buf, sizeof(buf), 0 }

typedef struct {
    void      *buf;
    uintptr_t size;
    uintptr_t offset;
} arena_t;

static inline uint32_t roundup(uint32_t x, uint32_t align) {
    return (x + align - 1) & (~(align - 1));
}

__attribute__((assume_aligned (16)))
static inline void* arena_alloc(arena_t *arena, uint32_t size) {
    size = roundup(size, 16);
    uintptr_t next_offset = arena->offset + size;
    if(next_offset > arena->size)
        return NULL;
    void *result = arena->buf + arena->offset;
    assert(((uintptr_t)result & 15) == 0);
    arena->offset = next_offset;
    return result;
}

static inline void arena_free(arena_t *arena, uintptr_t size) {
    size = roundup(size, 16);
    assert(arena->offset >= size);
    arena->offset -= size;
}
