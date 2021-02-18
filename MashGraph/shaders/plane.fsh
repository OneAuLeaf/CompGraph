#version 330 core

out vec4 FragColor;

in vec2 UV;

uniform float iTime;
uniform sampler2D iTex;

vec3 noise()
{
    vec2 vel = vec2(-1.0f, -1.0f);
    vec2 tex_coord = 1.5f*UV+vel*iTime*0.01f;
    vec3 finalNoise = vec3(0.0f);
    
    finalNoise += texture(iTex,tex_coord).rgb / 2.0f;
	finalNoise -= texture(iTex,tex_coord * 2.0f).rgb / 4.0f;
    finalNoise += texture(iTex,tex_coord * 4.0f).rgb / 8.0f;

    return finalNoise;

}


void main() {
	float eps = 0.01;

    vec3 cloud = noise();

    vec3 color = vec3(0.9, 0.6, 0.6);
    vec3 base = vec3(72., 80, 83.) / 255;
    vec3 light = mix(base, color, cloud);

    FragColor = vec4(light, 1.0);
}