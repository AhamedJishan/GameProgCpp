#version 330 core

layout (location = 0) out vec4 OutColor;

struct PointLight
{
	vec3 mWorldPos;
	vec3 mDiffuseColor;
	float mInnerRadius;
	float mOuterRadius;
};

uniform PointLight uPointLight;
uniform vec2 uScreenDimensions;

uniform sampler2D uGDiffuse;
uniform sampler2D uGNormal;
uniform sampler2D uGWorldPos;

void main()
{
	vec2 fragCoords = gl_FragCoord.xy / uScreenDimensions;

	vec3 gDiffuse = texture(uGDiffuse, fragCoords).xyz;
	vec3 gNormal = texture(uGNormal, fragCoords).xyz;
	vec3 gWorldPos= texture(uGWorldPos, fragCoords).xyz;

	vec3 N = normalize(gNormal);
	vec3 L = normalize(uPointLight.mWorldPos - gWorldPos);
	float NdotL = dot(N, L);
	vec3 phong = vec3(0, 0, 0);

	if (NdotL > 0)
	{
		float dist = distance(uPointLight.mWorldPos, gWorldPos);
		float intensity = smoothstep(uPointLight.mInnerRadius, uPointLight.mOuterRadius, dist);

		if (dist > uPointLight.mOuterRadius)
			discard;
	
		vec3 diffuse = mix(uPointLight.mDiffuseColor, vec3(0, 0, 0), intensity);
		phong = diffuse * NdotL;
	}

	OutColor = vec4(phong * gDiffuse, 1);
}