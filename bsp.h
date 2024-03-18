#pragma once

#include <winlib/platform.h>
#include <winlib/string.h>
#include <winlib/gl.h>
#include "arena.h"

typedef enum {
    BSP_LUMP_TEXDATA              = 2,
    BSP_LUMP_VERTICES             = 3,
    BSP_LUMP_TEXINFO              = 6,
    BSP_LUMP_FACES                = 7,
    BSP_LUMP_EDGES                = 12,
    BSP_LUMP_SURFEDGES            = 13,
    BSP_LUMP_TEXDATA_STRING_DATA  = 43,
    BSP_LUMP_TEXDATA_STRING_TABLE = 44,
    BSP_LUMP_COUNT                = 64,
} bsp_lump_type_t;

typedef struct {
    uint32_t offset;
    uint32_t size;
    uint32_t version;
    uint32_t four_cc;
} bsp_lump_t;

typedef struct {
    uint32_t   ident;
    uint32_t   version;
    bsp_lump_t lumps[BSP_LUMP_COUNT];
    uint32_t   revision;
} bsp_header_t;

typedef struct {
    float x, y, z;
} bsp_vec_t;

typedef struct {
    uint16_t v[2];
} bsp_edge_t;

typedef int32_t bsp_surfedge_t;

typedef struct {
    uint16_t planenum;
    uint8_t  side;
    uint8_t  on_node;
    int32_t  first_edge;
    int16_t  num_edges;
    int16_t  texinfo;
    int16_t  dispinfo;
    int16_t  fog_volume_id;
    uint8_t  styles[4];
    int32_t  light_offset;
    float    area;
    int32_t  min_luxels[2];
    int32_t  size_luxels[2];
    int32_t  orig_face;
    uint16_t num_prims;
    uint16_t first_prim_id;
    uint32_t smoothing_groups;
} bsp_face_t;

typedef struct {
    float   tex_v[2][4];
    float   light_v[2][4];
    int32_t flags;
    int32_t data;
} bsp_texinfo_t;

typedef struct {
    bsp_vec_t refl;
    int32_t   name;
    int32_t   width;
    int32_t   height;
    int32_t   view_width;
    int32_t   view_height;
} bsp_texdata_t;

typedef struct {
    s8             filename;
    uintptr_t      filesize;
    uintptr_t      num_indices;
    arena_t        *arena;
    bsp_header_t   header;
    bsp_texdata_t  *tex_data;
    bsp_vec_t      *vertices;
    bsp_texinfo_t  *tex_info;
    bsp_face_t     *faces;
    bsp_edge_t     *edges;
    bsp_surfedge_t *surfedges;
    char           *tex_names;
    int32_t        *tex_name_offsets;
    GLuint         vao;
} bsp_t;

void bsp_open(const s8 filename, bsp_t *bsp);
