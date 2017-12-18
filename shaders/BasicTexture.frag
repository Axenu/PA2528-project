#version 400 core

out vec4 Frag_Data;

in vec2 pass_UV;

uniform sampler2D tex;
uniform vec4 color;

void main() {
    Frag_Data = color * vec4(texture(tex, vec2(pass_UV.x, 1-pass_UV.y)).xyz, 1);
}
