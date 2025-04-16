#version 330

layout (location = 0) in vec3 vertexPos;

uniform mat4 u_WorldTransform;
uniform mat4 u_ViewProj;

void main()
{
	vec4 pos = vec4(vertexPos, 1);
	gl_Position = pos * u_WorldTransform * u_ViewProj;
}