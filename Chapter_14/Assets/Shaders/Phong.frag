#version 330 core

out vec4 OutColor;

in vec2 TexCoord;
in vec3 FragNormal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform vec3 uCameraPos;
uniform vec3 uLightDir;
uniform vec3 uLightColor;
uniform vec3 uAmbientColor;
uniform vec3 uSpecularColor;
uniform float uSpecularPower;
uniform bool uHasSpecular;

void main()
{
	vec4 baseColor = texture(texture_diffuse1, TexCoord);

	// Ambient
	vec3 phong = uAmbientColor * baseColor.rgb;

	// Diffuse
	vec3 lightDir = normalize(-uLightDir);
	vec3 normal = normalize(FragNormal);
	float diffuseFactor = max(dot(lightDir, normal), 0);
	phong += diffuseFactor * baseColor.rgb;

	// Specular
	vec3 reflectedDir = reflect(normalize(uLightDir), normal);
	vec3 viewDir = normalize(uCameraPos - FragPos);
	float specularFactor = pow(max(dot(viewDir, reflectedDir), 0.0), uSpecularPower);
	vec3 specularColor = uSpecularColor * specularFactor;
	
	if (uHasSpecular == true) phong += specularColor * texture(texture_specular1, TexCoord).r;
	else phong += specularColor;

	OutColor = vec4(phong, baseColor.a);
}