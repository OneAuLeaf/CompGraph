#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screen;

void main()
{
    float step = 0.003f;
    
    // 0 1 2
    // 3 4 5
    // 6 7 8 

    float kernel[9] = float[](
        1, 1, 1,
        1,-8, 1,
        1, 1, 1
    );

    vec2 steps_cell[9] = vec2[](
        vec2(-step, step), vec2(0.0f, step), vec2(step, step), 
        vec2(-step, 0.0f), vec2(0.0f, 0.0f), vec2(step, 0.0f),   
        vec2(-step, -step), vec2(0.0f, -step),vec2(step, -step)    
    );
    
    vec3 convolution = vec3(0.0f);
    for(int i = 0; i < 9; ++i) {
        vec2 sample;
        sample.s = max(step, min(1.0f - step, TexCoord.s + steps_cell[i].s));
        sample.t = max(step, min(1.0f - step, TexCoord.t + steps_cell[i].t));
        convolution += texture(screen, sample).rgb * kernel[i];
    }
    FragColor = vec4(convolution, 1.0f);
}