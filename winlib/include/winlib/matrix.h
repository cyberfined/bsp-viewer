#pragma once

typedef union {
    float data[3];
    struct {
        float x, y, z;
    };
} vec3_t;

vec3_t vec3_smult(vec3_t a, float s);
vec3_t vec3_add(vec3_t a, vec3_t b);
vec3_t vec3_sub(vec3_t a, vec3_t b);
vec3_t vec3_normalize(vec3_t a);
void vec3_normalize_inplace(vec3_t *a);
float vec3_dot(vec3_t a, vec3_t b);
vec3_t vec3_cross(vec3_t a, vec3_t b);

typedef union {
    float data[4];
    struct {
        float x, y, z, w;
    };
} vec4_t;

typedef float mat4_t[16];

void mat4_mm(mat4_t res, mat4_t a, mat4_t b);
void mat4_lookat(mat4_t res, vec3_t pos, vec3_t lookat, vec3_t up);
void mat4_perspective(mat4_t proj, float aspect, float fovy, float zn, float zf);
