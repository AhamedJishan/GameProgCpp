#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

uniform mat4 u_WorldTransform;
uniform mat4 u_ViewProjection;

out vec2 TexCoords;

void main()
{
	vec4 pos = vec4(a_Pos, 1);
	pos = u_ViewProjection * u_WorldTransform * pos;

	gl_Position = pos;

	TexCoords = a_TexCoords;
}