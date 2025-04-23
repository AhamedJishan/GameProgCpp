#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 uWorldTransform;
uniform mat4 uViewProjection;

out vec2 TexCoord;

void main()
{
	vec4 pos = vec4(aPos, 1);
	pos = uViewProjection * uWorldTransform * pos;

	gl_Position = pos;
	
	TexCoord = aTexCoord;
}