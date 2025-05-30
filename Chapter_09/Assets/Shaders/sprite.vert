#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;


uniform mat4 u_WorldTransform;
uniform mat4 u_OrthoProjection;

out vec2 TexCoord;

void main()
{
	TexCoord = a_TexCoord;

	vec4 pos = u_OrthoProjection * u_WorldTransform * vec4(a_Pos, 1);

	gl_Position = pos;
}