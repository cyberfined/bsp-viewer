#pragma once

#include <winlib/bool.h>
#include <winlib/matrix.h>

typedef struct {
    float  yaw;
    float  pitch;
    vec3_t up;
    vec3_t pos;
    vec3_t dir;
    vec3_t side;
    mat4_t view_mat;
    bool   has_changed;
} camera_t;

void camera_init(camera_t *camera, float yaw, float pitch, vec3_t up, vec3_t pos);
bool camera_calc_matrix(camera_t *camera);
void camera_add_yaw(camera_t *camera, float dyaw);
void camera_add_pitch(camera_t *camera, float dpitch);
void camera_move_forward(camera_t *camera, float d);
void camera_move_side(camera_t *camera, float d);
