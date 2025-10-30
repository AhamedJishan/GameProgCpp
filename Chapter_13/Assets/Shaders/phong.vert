#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 uWorldTransform;
uniform mat4 uViewProjection;

out vec2 TexCoord;
out vec3 FragNormal;
out vec3 FragPos;

void main()
{
	TexCoord = aTexCoord;

	FragNormal = mat3(transpose(inverse(uWorldTransform))) * aNormal;

	vec4 pos =  uWorldTransform * vec4(aPos, 1);
	FragPos = pos.xyz;
	pos = uViewProjection * pos;

	gl_Position = pos;
}