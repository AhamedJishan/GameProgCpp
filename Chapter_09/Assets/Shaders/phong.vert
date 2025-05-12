#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

uniform mat4 u_WorldTransform;
uniform mat4 u_ViewProjection;

out vec2 TexCoord;
out vec3 FragNormal;
out vec3 FragPos;

void main()
{
	TexCoord = a_TexCoord;

	FragNormal = mat3(transpose(inverse(u_WorldTransform))) * a_Normal;

	vec4 pos = u_WorldTransform * vec4(a_Pos, 1);
	FragPos = pos.xyz;
	pos = u_ViewProjection * pos;

	gl_Position = pos;
}