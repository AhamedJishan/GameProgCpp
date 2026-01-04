#version 330 core

out vec4 OutColor;

uniform sampler2D uGDiffuse;
uniform sampler2D uGNormal;
uniform sampler2D uGWorldPos;

uniform vec3 uCameraPos;
uniform vec3 uLightDir;
uniform vec3 uLightColor;
uniform vec3 uAmbientColor;

in vec2 TexCoords;

void main()
{
	vec3 diffuseColor = texture(uGDiffuse, TexCoords).rgb;
	vec3 normal = normalize(texture(uGNormal, TexCoords).xyz);
	vec3 worldPos = texture(uGWorldPos, TexCoords).xyz;
	
	// AMBIENT
	vec3 phong = diffuseColor * uAmbientColor;

	// DIFFUSE
	vec3 lightDir = normalize(-uLightDir);
	float diffuseFactor = max(dot(lightDir, normal), 0);
	phong += diffuseFactor * diffuseColor;

	OutColor = vec4(phong, 1);
}