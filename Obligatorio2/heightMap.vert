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
uniform float top;

varying vec4 vColor;
uniform sampler2D texture_height;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;

void main( void ){

float y = texture(texture_height, aTexCoords).g* top;
vec3 position = aPos;
position.y = y;

TexCoords = aTexCoords;
Normal = mat3(transpose(inverse(model))) * aNormal;  
FragPos = vec3(model * vec4(position, 1.f));
gl_Position = projection * view * vec4(FragPos, 1.0);

if(fract(sin(dot(FragPos.xy ,vec2(12.9898,78.233))) * 43758.5453)>(1-(FragPos.y/top)*1.5))
{
//vec4 result = vec4(0.5+(FragPos.y/top)/10,0.5+(FragPos.y/top)/10,0.5+(FragPos.y/top)/10, 1.f);
vColor = texture(texture_diffuse2, TexCoords);
}
else
{
vec4 result = vec4(texture(texture_diffuse1, TexCoords).r,texture(texture_diffuse1, TexCoords).g, texture(texture_diffuse1, TexCoords).b,1.0);
vColor = result;
}

}