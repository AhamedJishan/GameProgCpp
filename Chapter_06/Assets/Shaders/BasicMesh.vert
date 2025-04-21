#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 uViewProjection;
uniform mat4 uWorldTransform;

out vec2 TexCoord;

void main()
{
	TexCoord = aTexCoord;
	vec4 position = vec4(aPos, 1);
	position = uViewProjection * uWorldTransform * position ;

	gl_Position = position;
}