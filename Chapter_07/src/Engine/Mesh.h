#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace jLab
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<class Texture*> textures);
		~Mesh();

		void Draw(class Shader* shader);

	private:
		void SetupMesh();

	private:
		unsigned int m_Id;
		unsigned int m_VBO, m_EBO;

		std::vector<Vertex> m_Vertices;
		std::vector<unsigned int> m_Indices;
		std::vector<class Texture*> m_Textures;
	};
}