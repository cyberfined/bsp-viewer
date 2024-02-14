#include "shader.h"
#include "log.h"
#include "file.h"
#include "error.h"
#include "exit.h"

#define SHADER_BUF_SIZE 8192

static char shader_buf[SHADER_BUF_SIZE];

GLuint compile_shader(const char *filename, GLenum type) {
    io_result_t io_result = os_open(filename, READ_MODE);
    if(io_result.error) {
        os_error(io_result.res, shader_buf, sizeof(shader_buf));
        log("Failed to open file %s: %s", filename, shader_buf);
        goto error;
    }
    file_handle_t handle = (file_handle_t)io_result.res;

    io_result = os_file_size(handle);
    if(io_result.error) {
        os_error(io_result.res, shader_buf, sizeof(shader_buf));
        log("Failed to get file size %s: %s", filename, shader_buf);
        goto error;
    }
    uintptr_t file_size = io_result.res;
    if(file_size > sizeof(shader_buf)) {
        log("Shader %s is too big", filename);
        goto error;
    }

    io_result = read_all(handle, shader_buf, file_size);
    if(io_result.error) {
        os_error(io_result.res, shader_buf, sizeof(shader_buf));
        log("Failed to read sahder source from %s: %s", filename, shader_buf);
        goto error;
    }
    os_close(handle);

    GLuint shader = glCreateShader(type);
    if(!shader) {
        log("Failed to create shader: %s", gl_error_message(glGetError()));
        goto error;
    }
    const char *src = shader_buf;
    GLint length = file_size;
    glShaderSource(shader, 1, &src, &length);
    GLenum gl_error = glGetError();
    if(gl_error != GL_NO_ERROR) {
        log("%s glShaderSource error: %s", filename, gl_error_message(gl_error));
        goto error;
    }

    GLint status = 0;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status != GL_TRUE) {
        GLsizei log_size;
        glGetShaderInfoLog(shader, sizeof(shader_buf), &log_size, shader_buf);
        log("%s: Compile error: %s", filename, shader_buf);
        goto error;
    }

    return shader;
error:
    exit(EXIT_FAILURE);
}

GLuint create_simple_shader_program(
    const char *vertex_filename,
    const char *fragment_filename
) {
    GLuint vertex = compile_shader(vertex_filename, GL_VERTEX_SHADER);
    GLuint fragment = compile_shader(fragment_filename, GL_FRAGMENT_SHADER);
    GLuint program = glCreateProgram();
    if(!program) {
        log("Failed to create program: %s", gl_error_message(glGetError()));
        goto error;
    }

    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    GLint status = 0;
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status != GL_TRUE) {
        GLsizei log_size;
        glGetProgramInfoLog(program, sizeof(shader_buf), &log_size, shader_buf);
        log("Failed to link program: %s", shader_buf);
        goto error;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    
    return program;
error:
    exit(EXIT_FAILURE);
}
