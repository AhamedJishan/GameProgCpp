#version 330

in vec2 TexCoords;
uniform sampler2D u_Texture;

out vec4 fragColor;

void main()
{
	fragColor = texture(u_Texture, TexCoords);
}