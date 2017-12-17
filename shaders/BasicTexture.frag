#version 400 core

out vec4 Frag_Data;

in vec2 pass_UV;

uniform sampler2D tex;
uniform vec4 color;

void main() {
    Frag_Data = color * vec4(texture(tex, pass_UV).xyz, 1);
}
