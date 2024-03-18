#include "bsp.h"
#include <winlib/file.h>
#include <winlib/error.h>
#include <winlib/exit.h>
#include <winlib/string.h>
#include <winlib/log.h>
#include <winlib/offsetof.h>

#define BSP_IDENT (('P'<<24)+('S'<<16)+('B'<<8)+'V')

#define READ_LUMP(bsp, handle, type, member) \
    read_lump(bsp, type, handle, # type, ((void*)(bsp)) + offsetof(bsp_t, member), sizeof(*(((bsp_t*)0)->member)))


static void read_lump(
    bsp_t *bsp,
    bsp_lump_type_t type,
    file_handle_t handle,
    const char *lump_name,
    void **ptr,
    uintptr_t entity_size
) {
    bsp_lump_t *lump = &bsp->header.lumps[type];
    if(lump->four_cc != 0) {
        wl_log("%s read error: %s lump is compressed", bsp->filename.buf, lump_name);
        goto exit;
    }

    if(lump->offset + lump->size > bsp->filesize ||
       (lump->size / entity_size) * entity_size != lump->size) {
        wl_log("%s read error: lump %s is corrupted", bsp->filename.buf, lump_name);
        goto exit;
    }

    *ptr = arena_alloc(bsp->arena, lump->size);
    if(!*ptr) {
        wl_log("Out of memory");
        goto exit;
    }

    io_result_t io_res = pread_all(handle, *ptr, lump->size, lump->offset);
    if(io_res.error) {
        wl_log(
            "Failed to read %s lump from %s: %s",
            lump_name,
            bsp->filename.buf,
            os_error(io_res.res)
        );
        goto exit;
    }

    WL_DEBUG("%s, offset = %d, size = %d was read", lump_name, lump->offset, lump->size);

    return;
exit:
    os_exit(EXIT_FAILURE);
}

static void bsp_load_vertices(bsp_t *bsp) {
    uintptr_t num_vertices = 0, num_indices = 0;
    uint32_t num_faces = bsp->header.lumps[BSP_LUMP_FACES].size / sizeof(bsp_face_t);

    for(uint32_t i = 0; i < num_faces; i++) {
        bsp_face_t *face = &bsp->faces[i];
        // TODO: check docs about face skipping
        if(face->texinfo < 0 || face->light_offset == 0xffffffffu || face->light_offset < 4)
            continue;

        num_vertices += face->num_edges;
        num_indices += face->num_edges - 2;
    }
    num_indices *= 3;
    bsp->num_indices = num_indices;

    uintptr_t vertices_size = num_vertices * sizeof(GLfloat) * 3;
    GLfloat *vertices = arena_alloc(bsp->arena, vertices_size);
    if(!vertices) {
        wl_log("Out of memory");
        goto exit;
    }

    uintptr_t indices_size = num_indices * sizeof(GLushort);
    GLushort *indices = arena_alloc(bsp->arena, indices_size);
    if(!indices) {
        wl_log("Out of memory");
        goto exit;
    }

    uintptr_t vertex = 0, index = 0;
    for(uint32_t i = 0; i < num_faces; i++) {
        bsp_face_t *face = &bsp->faces[i];

        // TODO: check docs about face skipping
        if(face->texinfo < 0 || face->light_offset == 0xffffffffu || face->light_offset < 4)
            continue;

        int32_t sedge;
        uint16_t first_index = index;
        for(int16_t edge_idx = 0; edge_idx < face->num_edges; edge_idx++) {
            sedge = bsp->surfedges[face->first_edge + edge_idx];

            uint16_t from;
            if(sedge < 0) {
                from = bsp->edges[-sedge].v[1];
            } else {
                from = bsp->edges[sedge].v[0];
            }

            if(edge_idx > 2) {
                indices[index++] = indices[first_index];
                indices[index] = indices[index - 2];
                index++;
            }
            indices[index++] = vertex;

            wl_memcpy(&vertices[vertex * 3], &bsp->vertices[from], sizeof(bsp_vec_t));
            vertex++;
        }
    }

    assert(vertex == num_vertices);
    assert(index == num_indices);

    GLint align;
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &align);

    uintptr_t indices_size_aligned = roundup(indices_size, align);
    uintptr_t vertices_size_aligned = roundup(vertices_size, align);
    uintptr_t buf_size = indices_size_aligned + vertices_size_aligned;

    GLuint buf;
    glCreateBuffers(1, &buf);
    glNamedBufferStorage(buf, buf_size, NULL, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferSubData(buf, 0, indices_size, indices);
    glNamedBufferSubData(buf, indices_size_aligned, vertices_size, vertices);

    glCreateVertexArrays(1, &bsp->vao);
    glVertexArrayVertexBuffer(bsp->vao, 0, buf, indices_size_aligned, sizeof(bsp_vec_t));
    glVertexArrayElementBuffer(bsp->vao, buf);
    glVertexArrayAttribBinding(bsp->vao, 0, 0);
    glVertexArrayAttribFormat(bsp->vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(bsp->vao, 0);

    return;
exit:
    os_exit(EXIT_FAILURE);
}

void bsp_open(const s8 filename, bsp_t *bsp) {
    io_result_t io_res = os_open((char*)filename.buf, READ_MODE);

    if(io_res.error) {
        wl_log("Failed to open file %s: %s", filename.buf, os_error(io_res.res));
        goto exit;
    }

    file_handle_t handle = io_res.res;
    io_res = os_file_size(handle);
    if(io_res.error) {
        wl_log("Failed to get %s file size: %s", filename.buf, os_error(io_res.res));
        goto exit;
    }
    bsp->filename = filename;
    bsp->filesize = io_res.res;

    io_res = read_all(handle, &bsp->header, sizeof(bsp->header));
    if(io_res.error) {
        wl_log("Failed to read %s header: %s", filename.buf, os_error(io_res.res));
        goto exit;
    }

    if(bsp->header.ident != BSP_IDENT) {
        wl_log("%s has wrong BSP header", filename.buf);
        goto exit;
    }

    if(bsp->header.version != 20) {
        wl_log(
            "%s has %d version, but only 20 version is supported",
            filename.buf,
            bsp->header.version
        );
        goto exit;
    }

    WL_DEBUG(
        "%s: version = %d, revision = %d was opened",
        filename.buf,
        bsp->header.version,
        bsp->header.revision
    );

    READ_LUMP(bsp, handle, BSP_LUMP_TEXDATA, tex_data);
    READ_LUMP(bsp, handle, BSP_LUMP_VERTICES, vertices);
    READ_LUMP(bsp, handle, BSP_LUMP_TEXINFO, tex_info);
    READ_LUMP(bsp, handle, BSP_LUMP_FACES, faces);
    READ_LUMP(bsp, handle, BSP_LUMP_EDGES, edges);
    READ_LUMP(bsp, handle, BSP_LUMP_SURFEDGES, surfedges);
    READ_LUMP(bsp, handle, BSP_LUMP_TEXDATA_STRING_DATA, tex_names);
    READ_LUMP(bsp, handle, BSP_LUMP_TEXDATA_STRING_TABLE, tex_name_offsets);
    
    bsp_load_vertices(bsp);

    return;
exit:
    os_exit(EXIT_FAILURE);
}
