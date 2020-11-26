#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
flat out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

varying vec4 vColor;
uniform sampler2D texture_height;
uniform sampler2D texture_diffuse1;

void main( void ){

float y = texture(texture_height, aTexCoords).g* 70.f;
vec3 position = aPos;
position.y = y;

TexCoords = aTexCoords;
Normal = mat3(transpose(inverse(model))) * aNormal;  
FragPos = vec3(model * vec4(position, 1.f));
gl_Position = projection * view * vec4(FragPos, 1.0);

if(FragPos.y<28)
{
    vec4 result = vec4(texture(texture_height, TexCoords).r+0.2, texture(texture_height, TexCoords).g+0.2, texture(texture_height, TexCoords).b+0.2, 1.f);
    vColor = result;
}
else
{
    vec4 result = vec4(1.0, 1.0, 1.0, 1.f);
    vColor = result;
}

}