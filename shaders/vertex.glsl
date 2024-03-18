#version 330 core

layout (location = 0) in vec3 position;
out vec3 fragColor;
uniform mat4 proj;

void main() {
    gl_Position = proj * vec4((1.0 / 256.f) * position.xzy, 1.0);
    fragColor = vec3(1.0, 1.0, 1.0);
}
