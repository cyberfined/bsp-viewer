#pragma once

#include <GL/gl.h>
#include "khronos/glext.h"

#define GL_FUNC(type, name) extern type gl ## name;

#define GL_FUNCS_LIST \
    GL_FUNC(PFNGLGENBUFFERSPROC, GenBuffers) \
    GL_FUNC(PFNGLBINDBUFFERPROC, BindBuffer) \
    GL_FUNC(PFNGLBUFFERDATAPROC, BufferData) \
    GL_FUNC(PFNGLGENVERTEXARRAYSPROC, GenVertexArrays) \
    GL_FUNC(PFNGLBINDVERTEXARRAYPROC, BindVertexArray) \
    GL_FUNC(PFNGLENABLEVERTEXATTRIBARRAYPROC, EnableVertexAttribArray) \
    GL_FUNC(PFNGLVERTEXATTRIBPOINTERPROC, VertexAttribPointer) \
    GL_FUNC(PFNGLCREATEPROGRAMPROC, CreateProgram) \
    GL_FUNC(PFNGLUSEPROGRAMPROC, UseProgram) \
    GL_FUNC(PFNGLLINKPROGRAMPROC, LinkProgram) \
    GL_FUNC(PFNGLGETPROGRAMIVPROC, GetProgramiv) \
    GL_FUNC(PFNGLGETPROGRAMINFOLOGPROC, GetProgramInfoLog) \
    GL_FUNC(PFNGLCREATESHADERPROC, CreateShader) \
    GL_FUNC(PFNGLSHADERSOURCEPROC, ShaderSource) \
    GL_FUNC(PFNGLCOMPILESHADERPROC, CompileShader) \
    GL_FUNC(PFNGLGETSHADERIVPROC, GetShaderiv) \
    GL_FUNC(PFNGLGETSHADERINFOLOGPROC, GetShaderInfoLog) \
    GL_FUNC(PFNGLATTACHSHADERPROC, AttachShader) \
    GL_FUNC(PFNGLDELETESHADERPROC, DeleteShader)

GL_FUNCS_LIST

static inline const char *gl_error_message(GLenum error) {
    switch(error) {
    case GL_NO_ERROR:
        return "GL_NO_ERROR";
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";
    default:
        return "Unknown error";
    }
}

void load_gl(void);
