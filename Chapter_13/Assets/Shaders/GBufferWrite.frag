#version 330 core

in vec2 TexCoord;
in vec3 FragNormal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;

layout (location = 0) out vec3 outDiffuse;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec3 outWorldPos;

void main()
{
	outDiffuse = texture(texture_diffuse1, TexCoord).rgb;
	outNormal = FragNormal;
	outWorldPos = FragPos;
}