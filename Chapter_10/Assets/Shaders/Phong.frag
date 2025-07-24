#version 330 core

out vec4 OutColor;

in vec2 TexCoord;
in vec3 FragNormal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform vec3 u_CameraPos;
uniform vec3 u_LightDir;
uniform vec3 u_LightColor;
uniform vec3 u_AmbientColor;
uniform vec3 u_SpecularColor;
uniform float u_SpecularPower;
uniform bool u_HasSpecular;

void main()
{
	vec4 baseColor = texture(texture_diffuse1, TexCoord);

	// Ambient
	vec3 phong = u_AmbientColor * baseColor.rgb;

	// Diffuse
	vec3 lightDir = normalize(-u_LightDir);
	vec3 normal = normalize(FragNormal);
	float diffuseFactor = max(dot(lightDir, normal), 0);
	phong += diffuseFactor * baseColor.rgb;

	// Specular
	vec3 reflectedDir = reflect(normalize(u_LightDir), normal);
	vec3 viewDir = normalize(u_CameraPos - FragPos);
	float specularFactor = pow(max(dot(viewDir, reflectedDir), 0.0), u_SpecularPower);
	vec3 specularColor = u_SpecularColor * specularFactor;
	
	if (u_HasSpecular == true) phong += specularColor * texture(texture_specular1, TexCoord).r;
	else phong += specularColor;

	OutColor = vec4(phong, baseColor.a);
}