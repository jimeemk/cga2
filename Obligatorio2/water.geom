#version 400 core

// A triangle with three points come in
layout(triangles) in;

layout(triangle_strip, max_vertices=3) out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 g_Normal[3];
in vec3 g_FragPos[3];

out vec3 v_Normal;
out vec3 v_FragPos;

void main (void)
{
	vec3 vector1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 vector2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 new_normal = normalize(cross(vector1, vector2));

	for (int i = 0; i < 3; i++)
	{
		gl_Position = gl_in[i].gl_Position;
		v_FragPos = g_FragPos[i];
		v_Normal = normalize(new_normal + g_Normal[i]);
		EmitVertex();
	}

	EndPrimitive();
}