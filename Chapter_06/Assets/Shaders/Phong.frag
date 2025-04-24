#version 330

out vec4 OutColor;

in vec2 TexCoord;
in vec3 FragNormal;
in vec3 FragWorldPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform bool uHasSpecular;

uniform vec3 uLightDir;
uniform vec3 uLightColor;
uniform vec3 uCameraPos;
uniform vec3 uAmbientColor;
uniform vec3 uSpecularColor;
uniform float uSpecularPower;

void main()
{
	vec4 baseColor = texture(texture_diffuse1, TexCoord);

	// Ambient
	vec3 phong = uAmbientColor * baseColor.rgb;

	// Diffuse
	vec3 lightDir = normalize(-uLightDir);
	vec3 normal = normalize(FragNormal);
	vec3 diffuseColor = uLightColor * max(dot(normal, lightDir), 0) * baseColor.rgb;
	phong += diffuseColor;

	// Specular
	vec3 reflectedDir = reflect(normalize(uLightDir), normal);
	vec3 viewDir = normalize(uCameraPos - FragWorldPos);
	vec3 specularColor = uSpecularColor * pow(max(dot(reflectedDir, viewDir), 0), uSpecularPower);
	
	if (uHasSpecular == true) phong += specularColor * texture(texture_specular1, TexCoord).r;
	else phong += specularColor;

	OutColor = vec4(phong, 1);
}