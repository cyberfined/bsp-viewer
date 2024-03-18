#include <winlib/camera.h>
#include <winlib/math.h>
#include <winlib/matrix.h>

static inline void set_dir(camera_t *camera) {
    float sin_pitch = wl_sinf(camera->pitch);

    camera->dir = (vec3_t) {
        .x = wl_cosf(camera->yaw) * sin_pitch,
        .y = wl_cosf(camera->pitch),
        .z = wl_sinf(camera->yaw) * sin_pitch,
    };
    camera->side = vec3_cross(camera->dir, camera->up);
}

void camera_init(camera_t *camera, float yaw, float pitch, vec3_t up, vec3_t pos) {
    camera->yaw = yaw;
    camera->pitch = pitch;
    camera->up = up;
    camera->pos = pos;
    set_dir(camera);
    camera->has_changed = true;
}

bool camera_calc_matrix(camera_t *camera) {
    if(!camera->has_changed)
        return false;
    set_dir(camera);
    vec3_t lookat = vec3_add(camera->pos, camera->dir);
    mat4_lookat(camera->view_mat, camera->pos, lookat, camera->up);
    camera->has_changed = false;
    return true;
}

void camera_add_yaw(camera_t *camera, float dyaw) {
    camera->yaw += dyaw;
    if(camera->yaw < 0)
        camera->yaw += 2.f * M_PI;
    else if(camera->yaw >= 2.f * M_PI)
        camera->yaw -= 2.f * M_PI;

    camera->has_changed = true;
}

void camera_add_pitch(camera_t *camera, float dpitch) {
    camera->pitch += dpitch;
    if(camera->pitch < 0)
        camera->pitch += 2.f * M_PI;
    else if(camera->pitch >= 2 * M_PI)
        camera->pitch -= 2.f * M_PI;

    camera->has_changed = true;
}

void camera_move_forward(camera_t *camera, float d) {
    camera->pos = vec3_add(camera->pos, vec3_smult(camera->dir, d));
    camera->has_changed = true;
}

void camera_move_side(camera_t *camera, float d) {
    camera->pos = vec3_add(camera->pos, vec3_smult(camera->side, d));
    camera->has_changed = true;
}
