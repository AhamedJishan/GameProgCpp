#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in uvec4 aSkinBones;
layout (location = 4) in vec4 aSkinWeights;

uniform mat4 uWorldTransform;
uniform mat4 uViewProjection;
uniform mat4 uMatrixPalette[96];

out vec2 TexCoord;
out vec3 FragNormal;
out vec3 FragPos;

void main()
{
	TexCoord = aTexCoord;

	vec4 pos = vec4(aPos, 1.0f);

	pos =
      (uMatrixPalette[aSkinBones.x] * pos) * aSkinWeights.x +
      (uMatrixPalette[aSkinBones.y] * pos) * aSkinWeights.y +
      (uMatrixPalette[aSkinBones.z] * pos) * aSkinWeights.z +
      (uMatrixPalette[aSkinBones.w] * pos) * aSkinWeights.w;

	vec3 normal = aNormal;
	normal = mat3(uMatrixPalette[aSkinBones.x]) * normal * aSkinWeights.x;
	normal += mat3(uMatrixPalette[aSkinBones.y]) * normal * aSkinWeights.y;
	normal += mat3(uMatrixPalette[aSkinBones.z]) * normal * aSkinWeights.z;
	normal += mat3(uMatrixPalette[aSkinBones.w]) * normal * aSkinWeights.w;

	FragNormal = mat3(transpose(inverse(uWorldTransform))) * normal;
	
	pos =  uWorldTransform * pos;
	FragPos = pos.xyz;
	pos = uViewProjection * pos;

	gl_Position = pos;
}