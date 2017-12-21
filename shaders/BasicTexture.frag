#version 400 core

out vec4 Frag_Data;

in vec2 pass_UV;
in vec3 pass_normal;

uniform sampler2D tex;
uniform vec4 color;

void main() {

	vec3 lightDir = normalize(vec3(1, 1, 1));

	float NdotL = max(dot(pass_normal, lightDir), 0.0);
	NdotL += 0.3;

    Frag_Data = vec4((color * vec4(texture(tex, vec2(pass_UV.x, 1-pass_UV.y)).xyz, 1)).xyz * NdotL, 1);
	//Frag_Data = vec4(pass_normal, 1);
}
