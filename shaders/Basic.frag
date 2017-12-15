#version 400 core

out vec4 Frag_Data;

in vec2 pass_UV;

uniform sampler2D tex;

void main() {
    Frag_Data = vec4(0,0,1,1) * texture(tex, pass_UV);
}
