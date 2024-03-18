#include <winlib/shader.h>
#include <winlib/log.h>
#include <winlib/file.h>
#include <winlib/error.h>
#include <winlib/exit.h>

#define SHADER_BUF_SIZE 8192

static char shader_buf[SHADER_BUF_SIZE];

GLuint compile_shader(const char *filename, GLenum type) {
    io_result_t io_result = os_open(filename, READ_MODE);
    if(io_result.error) {
        wl_log("Failed to open file %s: %s", filename, os_error(io_result.res));
        goto error;
    }
    file_handle_t handle = (file_handle_t)io_result.res;

    io_result = os_file_size(handle);
    if(io_result.error) {
        wl_log("Failed to get file size %s: %s", filename, os_error(io_result.res));
        goto error;
    }
    uintptr_t file_size = io_result.res;
    if(file_size > sizeof(shader_buf)) {
        wl_log("Shader %s is too big", filename);
        goto error;
    }

    io_result = read_all(handle, shader_buf, file_size);
    if(io_result.error) {
        wl_log(
            "Failed to read sahder source from %s: %s",
            filename,
            os_error(io_result.res)
        );
        goto error;
    }
    os_close(handle);

    GLuint shader = glCreateShader(type);
    if(!shader) {
        wl_log("Failed to create shader: %s", gl_error_message(glGetError()));
        goto error;
    }
    const char *src = shader_buf;
    GLint length = file_size;
    glShaderSource(shader, 1, &src, &length);
    GLenum gl_error = glGetError();
    if(gl_error != GL_NO_ERROR) {
        wl_log("%s glShaderSource error: %s", filename, gl_error_message(gl_error));
        goto error;
    }

    GLint status = 0;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status != GL_TRUE) {
        GLsizei log_size;
        glGetShaderInfoLog(shader, sizeof(shader_buf), &log_size, shader_buf);
        wl_log("%s: Compile error: %s", filename, shader_buf);
        goto error;
    }

    return shader;
error:
    os_exit(EXIT_FAILURE);
}

GLuint create_simple_shader_program(
    const char *vertex_filename,
    const char *fragment_filename
) {
    GLuint vertex = compile_shader(vertex_filename, GL_VERTEX_SHADER);
    GLuint fragment = compile_shader(fragment_filename, GL_FRAGMENT_SHADER);
    GLuint program = glCreateProgram();
    if(!program) {
        wl_log("Failed to create program: %s", gl_error_message(glGetError()));
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
        wl_log("Failed to link program: %s", shader_buf);
        goto error;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    
    return program;
error:
    os_exit(EXIT_FAILURE);
}
