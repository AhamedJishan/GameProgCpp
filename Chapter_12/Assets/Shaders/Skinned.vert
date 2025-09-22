#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in uvec4 a_SkinBones;
layout (location = 4) in vec4 a_SkinWeights;

uniform mat4 u_WorldTransform;
uniform mat4 u_ViewProjection;
uniform mat4 u_MatrixPalette[96];

out vec2 TexCoord;
out vec3 FragNormal;
out vec3 FragPos;

void main()
{
	TexCoord = a_TexCoord;

	vec4 pos = vec4(a_Pos, 1.0f);

	pos =
      (u_MatrixPalette[a_SkinBones.x] * pos) * a_SkinWeights.x +
      (u_MatrixPalette[a_SkinBones.y] * pos) * a_SkinWeights.y +
      (u_MatrixPalette[a_SkinBones.z] * pos) * a_SkinWeights.z +
      (u_MatrixPalette[a_SkinBones.w] * pos) * a_SkinWeights.w;

	vec3 normal = a_Normal;
	normal = mat3(u_MatrixPalette[a_SkinBones.x]) * normal * a_SkinWeights.x;
	normal += mat3(u_MatrixPalette[a_SkinBones.y]) * normal * a_SkinWeights.y;
	normal += mat3(u_MatrixPalette[a_SkinBones.z]) * normal * a_SkinWeights.z;
	normal += mat3(u_MatrixPalette[a_SkinBones.w]) * normal * a_SkinWeights.w;

	FragNormal = mat3(transpose(inverse(u_WorldTransform))) * normal;
	
	pos =  u_WorldTransform * pos;
	FragPos = pos.xyz;
	pos = u_ViewProjection * pos;

	gl_Position = pos;
}