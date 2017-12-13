#version 400 core


layout(location = 0) in vec3 in_Position;
uniform mat4 mvp;
// layout(location = 1) in vec3 in_Normal;

void main() {
    gl_Position = mvp * vec4(in_Position.xyz, 1.0);
}
