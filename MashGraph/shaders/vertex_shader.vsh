#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 TViewPos;
out vec3 TLightPos;
out vec3 TFragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;
uniform vec3 lightPos;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);

	TexCoord = aTexCoord;
	FragPos = vec3(model * vec4(aPos, 1.0));;
	
	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
	mat3 iTBN = transpose(mat3(T, B, N));

	TViewPos = iTBN * viewPos;
	TLightPos = iTBN * lightPos;
	TFragPos = iTBN * FragPos;
}