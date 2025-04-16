#version 330

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec2 texCoords;

uniform mat4 u_WorldTransform;
uniform mat4 u_ViewProj;

out vec2 TexCoords;

void main()
{
	TexCoords = texCoords;
	
	vec4 pos = vec4(vertexPos, 1);
	pos = pos * u_WorldTransform * u_ViewProj;
	
	gl_Position = pos;
}