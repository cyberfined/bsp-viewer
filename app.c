#include "app.h"
#include "gl.h"
#include "log.h"
#include "shader.h"

window_params_t window_params = {
    .title           = "OpenGL example",
    .width           = 500,
    .height          = 500,
    .x               = 100,
    .y               = 100,
    .opengl_major    = 3,
    .opengl_minor    = 3,
    .red_bits        = 8,
    .green_bits      = 8,
    .blue_bits       = 8,
    .alpha_bits      = 8,
    .depth_bits      = 24,
};

static struct {
    GLuint program;
    GLuint vao, vbo;
} state;

static GLfloat vertices[] = {
    -0.5, -0.5, 1.0, 0.0, 0.0,
     0.5, -0.5, 0.0, 1.0, 0.0,
     0.0,  0.5, 0.0, 0.0, 1.0,
};

void init_app(void) {
    const GLubyte *GLVersionString = glGetString(GL_VERSION);
    log("OpenGL version: %s", GLVersionString);

    load_gl();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glViewport(0, 0, window_params.width, window_params.height);

    state.program = create_simple_shader_program("shaders/vertex.glsl", "shaders/fragment.glsl");
    glUseProgram(state.program);
    glGenVertexArrays(1, &state.vao);
    glBindVertexArray(state.vao);
    glGenBuffers(1, &state.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, state.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), NULL);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(2 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void key_handler(key_t key, bool is_down) {
}

void resize_handler(int width, int height) {
    window_params.width = width;
    window_params.height = height;
    glViewport(0, 0, window_params.width, window_params.height);
}

void mouse_handler(mouse_event_t event) {
}

void draw(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 15);
}
