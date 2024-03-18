#pragma once

#include <winlib/gl.h>

GLuint compile_shader(const char *filename, GLenum type);
GLuint create_simple_shader_program(const char *vertex_filename, const char *fragment_filename);
