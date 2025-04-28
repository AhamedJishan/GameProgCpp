#version 330 core

out vec4 OutColor;

uniform sampler2D texture_diffuse1;

in vec2 TexCoord;

void main()
{
	OutColor = texture(texture_diffuse1, TexCoord);
}