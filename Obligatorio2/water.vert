#version 400 core

const float PI = 3.141592654;
const float G = 9.81;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float passedTime;

float clampTwoPI(float h)
{
	while (h < 0) h += 2 * PI;
	while (h > 6.283185) h -= 2 * PI;
	return h;
}

void main()
{
    vec3 position = aPos;
    vec2 direction = vec2(1.f, 0.f);
    float speed = 4;
    float frequency = sqrt(G * 2.0 * PI);
	//float phase = speed * frequency;
	float alpha = frequency * dot(vec3(1.f, 0.f, 0.f), vec3(position.x, 0.f, position.z)) - speed * passedTime;

    position.x += 0.3 * cos(position.x - speed * passedTime);
    position.y += 0.2 * sin(0.9 * position.x + 2.1 * position.z - speed * passedTime);
    position.z += 0.3 * cos(position.z - speed * passedTime);

    TexCoords = aTexCoords;
    FragPos = vec3(model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}