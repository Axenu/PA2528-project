#version 400 core


layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_UV;
layout(location = 2) in vec3 in_normal;
uniform mat4 mvp;

out vec2 pass_UV;
out vec3 pass_normal;

void main() {
    pass_UV = in_UV;
	pass_normal = in_normal;
    gl_Position = mvp * vec4(in_Position.xyz, 1.0);
}
