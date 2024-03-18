#include <winlib/app.h>
#include <winlib/string.h>
#include <winlib/log.h>
#include <winlib/bufio.h>
#include <winlib/gl.h>
#include <winlib/shader.h>
#include <winlib/exit.h>
#include <winlib/matrix.h>
#include <winlib/camera.h>
#include "arena.h"
#include "bsp.h"

// TODO: rewrite with cmd args
#ifdef _WIN32
#define BSP_PATH "C:/Program Files (x86)/Steam/steamapps/common/Portal/portal/maps/testchmb_a_00.bsp"
#elif defined(__linux__)
#define BSP_PATH "/mnt/windows/Program Files (x86)/Steam/steamapps/common/Portal/portal/maps/testchmb_a_00.bsp"
#else
#error "your os is not suported"
#endif

#define CAMERA_MOVE_SPEED 20.f
#define CAMERA_ROTATE_SPEED 0.1f

static uint8_t arena_buf[2 * 1024 * 1024] __attribute__((aligned(16)));

window_params_t window_params = {
    .title           = "OpenGL example",
    .width           = 500,
    .height          = 500,
    .x               = 100,
    .y               = 100,
    .opengl_major    = 4,
    .opengl_minor    = 6,
    .red_bits        = 8,
    .green_bits      = 8,
    .blue_bits       = 8,
    .alpha_bits      = 8,
    .depth_bits      = 24,
    .vsync           = true,
    .warp_pointer    = true,
};

static struct {
    GLuint vbo, ebo, vao;
    GLuint prog, proj_uniform;

    mat4_t proj, proj_view;
    camera_t cam;

    bool keys[WL_KEY_COUNT];
    int32_t mouse_dx, mouse_dy;
    arena_t arena;
    bsp_t   bsp;
} g;

#ifdef WINLIB_DEBUG
static void gl_message_callback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    GLchar const* message,
    void const* user_param
) {
    char *source_str;
    switch(source) {
        case GL_DEBUG_SOURCE_API:             source_str = "API";             break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   source_str = "Window system";   break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: source_str = "Shader compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     source_str = "Third party";     break;
        case GL_DEBUG_SOURCE_APPLICATION:     source_str = "Application";     break;
        case GL_DEBUG_SOURCE_OTHER:           source_str = "Other";           break;
        default:                              source_str = "Unknown";         break;
    }

    char *type_str;
    switch(type) {
        case GL_DEBUG_TYPE_ERROR:               type_str = "Error";               break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_str = "Deprecated behavior"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  type_str = "Undefined behavior";  break;
        case GL_DEBUG_TYPE_PORTABILITY:         type_str = "Portability";         break;
        case GL_DEBUG_TYPE_PERFORMANCE:         type_str = "Performance";         break;
        case GL_DEBUG_TYPE_MARKER:              type_str = "Marker";              break;
        case GL_DEBUG_TYPE_OTHER:               type_str = "Other";               break;
        default:                                type_str = "Uknown";              break;
    }

    char *severity_str;
    switch(severity) {
        case GL_DEBUG_SEVERITY_NOTIFICATION: severity_str = "Notification"; break;
        case GL_DEBUG_SEVERITY_LOW:          severity_str = "Low";          break;
        case GL_DEBUG_SEVERITY_MEDIUM:       severity_str = "Medium";       break;
        case GL_DEBUG_SEVERITY_HIGH:         severity_str = "High";         break;
        default:                             severity_str = "Unknown";      break;
    }

    wl_log("%s, %s, %s, %u: %s", source_str, type_str, severity_str, id, message);
}
#endif

void init_app(void) {
    g.arena = ARENA(arena_buf);
    g.bsp.arena = &g.arena;

    load_gl();

#ifdef WINLIB_DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(gl_message_callback, NULL);
#endif

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);  
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glViewport(0, 0, window_params.width, window_params.height);

    bsp_open(S8(BSP_PATH), &g.bsp);
    glBindVertexArray(g.bsp.vao);

    g.prog = create_simple_shader_program(
        "shaders/vertex.glsl",
        "shaders/fragment.glsl"
    );
    g.proj_uniform = glGetUniformLocation(g.prog, "proj");
    glUseProgram(g.prog);

    camera_init(
        &g.cam,
        -1.663499f,
        1.57f,
        (vec3_t){.x = 0.f, .y = 1.f, .z = 0.f},
        (vec3_t){.x = 0.f, .y = 2.f, .z = 10.f}
    );
    camera_calc_matrix(&g.cam);
}

void key_handler(wl_key_t key, bool is_down) {
    if(key == WL_KEY_ESC)
        os_exit(EXIT_SUCCESS);

    g.keys[key] = is_down;
}

void resize_handler(int old_width, int old_height) {
    glViewport(0, 0, window_params.width, window_params.height);

    float aspect = (float)window_params.width / (float)window_params.height;
    mat4_perspective(g.proj, aspect, 1.0472f, 0.01f, 250.0f);
    mat4_mm(g.proj_view, g.proj, g.cam.view_mat);
    glUniformMatrix4fv(g.proj_uniform, 1, GL_FALSE, g.proj_view);
}

void mouse_handler(wl_mouse_event_t event) {
    g.mouse_dx = event.x;
    g.mouse_dy = event.y;
}

void handle_input(float delta) {
    if(g.keys[WL_KEY_W])
        camera_move_forward(&g.cam, CAMERA_MOVE_SPEED * delta);
    else if(g.keys[WL_KEY_S])
        camera_move_forward(&g.cam, -CAMERA_MOVE_SPEED * delta);

    if(g.keys[WL_KEY_A])
        camera_move_side(&g.cam, -CAMERA_MOVE_SPEED * delta);
    else if(g.keys[WL_KEY_D])
        camera_move_side(&g.cam, CAMERA_MOVE_SPEED * delta);

    if(g.mouse_dx) {
        camera_add_yaw(&g.cam, g.mouse_dx * delta * CAMERA_ROTATE_SPEED);
        g.mouse_dx = 0;
    }

    if(g.mouse_dy) {
        camera_add_pitch(&g.cam, g.mouse_dy * delta * CAMERA_ROTATE_SPEED);
        g.mouse_dy = 0;
    }
}

void draw(uintptr_t dt_us) {
    float delta = dt_us * 1e-6;

    handle_input(delta);

    if(camera_calc_matrix(&g.cam)) {
        mat4_mm(g.proj_view, g.proj, g.cam.view_mat);
        glUniformMatrix4fv(g.proj_uniform, 1, GL_FALSE, g.proj_view);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, g.bsp.num_indices, GL_UNSIGNED_SHORT, NULL);
}
