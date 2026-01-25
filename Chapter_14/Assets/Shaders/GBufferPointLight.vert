#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 uWorldTransform;
uniform mat4 uViewProjection;

void main()
{
	vec4 pos = uViewProjection * uWorldTransform * vec4(aPos, 1);

	gl_Position = pos;
}