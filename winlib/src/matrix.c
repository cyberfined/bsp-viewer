#include <winlib/matrix.h>
#include <winlib/math.h>
#include <winlib/string.h>

vec3_t vec3_smult(vec3_t a, float s) {
    vec3_t result;
    for(int i = 0; i < 3; i++)
        result.data[i] = a.data[i] * s;
    return result;
}

vec3_t vec3_add(vec3_t a, vec3_t b) {
    vec3_t res;
    for(int i = 0; i < 3; i++)
        res.data[i] = a.data[i] + b.data[i];
    return res;
}

vec3_t vec3_sub(vec3_t a, vec3_t b) {
    vec3_t res;
    for(int i = 0; i < 3; i++)
        res.data[i] = a.data[i] - b.data[i];
    return res;
}

vec3_t vec3_normalize(vec3_t a) {
    vec3_t res;

    float len = 0;
    for(int i = 0; i < 3; i++)
        len += a.data[i] * a.data[i];

    float norm = 1.0 / wl_sqrtf(len);
    for(int i = 0; i < 3; i++)
        res.data[i] = a.data[i] * norm;

    return res;
}

void vec3_normalize_inplace(vec3_t *a) {
    float len = 0;
    for(int i = 0; i < 3; i++)
        len += a->data[i] * a->data[i];

    float norm = 1.0 / wl_sqrtf(len);
    for(int i = 0; i < 3; i++)
        a->data[i] *= norm;
}

float vec3_dot(vec3_t a, vec3_t b) {
    float res = 0;
    for(int i = 0; i < 3; i++)
        res += a.data[i] * b.data[i];
    return res;
}

vec3_t vec3_cross(vec3_t a, vec3_t b) {
    return (vec3_t) {
        .x = a.y * b.z - a.z * b.y,
        .y = a.z * b.x - a.x * b.z,
        .z = a.x * b.y - a.y * b.x,
    };
}

void mat4_mm(mat4_t res, mat4_t a, mat4_t b) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            res[4 * j + i] = 0;
            for(int k = 0; k < 4; k++) {
                res[4 * j + i] += a[4 * k + i] * b[4 * j + k];
            }
        }
    }
}

void mat4_lookat(mat4_t res, vec3_t pos, vec3_t lookat, vec3_t up) {
    vec3_t dir = vec3_sub(pos, lookat);
    vec3_normalize_inplace(&dir);
    vec3_t side = vec3_cross(up, dir);
    vec3_normalize_inplace(&side);
    up = vec3_cross(dir, side);
    vec3_normalize_inplace(&up);

    res[0 * 4 + 0] = side.x;
    res[1 * 4 + 0] = side.y;
    res[2 * 4 + 0] = side.z;

    res[0 * 4 + 1] = up.x;
    res[1 * 4 + 1] = up.y;
    res[2 * 4 + 1] = up.z;

    res[0 * 4 + 2] = dir.x;
    res[1 * 4 + 2] = dir.y;
    res[2 * 4 + 2] = dir.z;

    res[3 * 4 + 0] = -vec3_dot(side, pos);
    res[3 * 4 + 1] = -vec3_dot(up, pos);
    res[3 * 4 + 2] = -vec3_dot(dir, pos);

    res[0 * 4 + 3] = 0.f;
    res[1 * 4 + 3] = 0.f;
    res[2 * 4 + 3] = 0.f;
    res[3 * 4 + 3] = 1.f;
}

void mat4_perspective(mat4_t proj, float aspect, float fovy, float zn, float zf) {
    wl_memset(proj, 0, sizeof(mat4_t));

    float k_y = wl_tanf(fovy * 0.5f);
    float k_x = k_y * aspect;
    proj[0] = 1.0f / k_x;
    proj[5] = 1.0f / k_y;
    proj[10] = -(zf + zn) / (zf - zn);
    proj[11] = -1.0f;
    proj[14] = -2.0f * zf * zn / (zf - zn);
}
