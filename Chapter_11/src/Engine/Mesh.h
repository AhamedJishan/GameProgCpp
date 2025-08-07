#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <string>
#include <vector>

namespace jLab
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoord;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<class Texture*> textures);
		~Mesh();

		// TODO: void Draw(const class Shader& shader);

		const std::vector<Vertex>& GetVertices() const { return m_Vertices; }

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