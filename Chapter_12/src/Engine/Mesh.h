#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <cstdint>

namespace jLab
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoord;
		uint8_t BoneIds[4] = { 0 };
		uint8_t BoneWeights[4] = { 0 };

		bool AddBone(uint8_t boneId, uint8_t boneWeight)
		{
			for (int i = 0; i < 4; i++)
			{
				if (BoneWeights[i] == 0)
				{
					BoneIds[i] = boneId;
					BoneWeights[i] = boneWeight;
					return true;
				}
			}
			return false;
		}
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<class Texture*> textures, bool isSkinned = false);
		~Mesh();

		void Draw(const class Shader* shader) const;

		std::vector<Vertex> GetVertices() const { return m_Vertices; }

	private:
		void SetupMesh();

	private:
		unsigned int m_Id;
		unsigned int m_VBO, m_EBO;

		bool m_IsSkinned;

		std::vector<Vertex> m_Vertices;
		std::vector<unsigned int> m_Indices;
		std::vector<class Texture*> m_Textures;
	};
}