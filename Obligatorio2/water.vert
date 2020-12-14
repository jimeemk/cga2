#version 400 core

#define NUMBERWAVES 4

const float PI = 3.141592654;
const float G = 9.81;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 g_Normal;
out vec3 g_FragPos;
out vec2 v_TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float passedTime;

uniform vec4 waveParameters[NUMBERWAVES];
uniform vec2 waveDirections[NUMBERWAVES];

vec3 CalcGerstnerWaveNormal(vec3 point);
vec3 CalcGerstnerWaveOffset(vec3 v);

void main(void) 
{
	v_TexCoords = aTexCoords;

	vec4 finalVertex;

	finalVertex.x = aPos.x;
	finalVertex.y = aPos.y;
	finalVertex.z = aPos.z;
	finalVertex.w = 1.0;

	vec3 finalNormal;

	finalVertex += vec4(CalcGerstnerWaveOffset(aPos), 0.f);
				
	gl_Position = projection * view * model * finalVertex;

	g_FragPos = gl_Position.xyz;
	g_Normal = CalcGerstnerWaveNormal(g_FragPos);
}

vec3 CalcGerstnerWaveNormal(vec3 point)
{
	vec3 normal = vec3(0, 1.f, 0);
	
	for (int i = 0; i < NUMBERWAVES; i++)
	{
		vec2 direction = normalize(waveDirections[i]);
		float speed = waveParameters[i].x;
		float amplitude = waveParameters[i].y;
		float wavelength = waveParameters[i].z;
		float steepness = waveParameters[i].w;

		float wi = 2 / wavelength;
		float WA = wi * amplitude;
		float phi = speed * wi;
		float rad = wi * dot(direction, point.xz) + phi * passedTime;
		float Qi = steepness / (wavelength * wi * NUMBERWAVES);
		normal.xz -= direction * WA * cos(rad);
		normal.y -= Qi * WA * sin(rad);
	}
	return normalize(normal);
}

vec3 CalcGerstnerWaveOffset(vec3 v)
{
	vec3 sum = vec3(0, 0, 0);

	for (int i = 0; i < NUMBERWAVES; i++)
	{
		vec2 direction = normalize(waveDirections[i]);
		float speed = waveParameters[i].x;
		float amplitude = waveParameters[i].y;
		float wavelength = waveParameters[i].z;
		float steepness = waveParameters[i].w;
		float wi = 2 / wavelength;
		float Qi = steepness / (amplitude * wi * NUMBERWAVES);
		float phi = speed * wi;
		float rad = dot(direction, v.xz) * wi + passedTime * phi;
		sum.y += sin(rad) * amplitude;
		sum.xz += cos(rad) * amplitude * Qi * direction;
	}
	return sum;
}