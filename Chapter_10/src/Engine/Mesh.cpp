#include "Mesh.h"

#include <GLEW/GL/glew.h>
#include <string>
#include "Shader.h"
#include "Texture.h"

namespace jLab
{
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<class Texture*> textures)
	{
		m_Vertices = vertices;
		m_Indices = indices;
		m_Textures = textures;

		SetupMesh();
	}
	
	Mesh::~Mesh()
	{
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);
		glDeleteVertexArrays(1, &m_Id);
	}
	
	void Mesh::Draw(const Shader* shader) const
	{
		int diffuseNr = 0;
		int specularNr = 0;
		int normalNr = 0;

		for (int i = 0; i < m_Textures.size(); i++)
		{
			std::string name;
			if (m_Textures[i]->GetTextureType() == Texture::E_Diffuse)
			{
				name = "texture_diffuse" + std::to_string(++diffuseNr);
			}
			else if (m_Textures[i]->GetTextureType() == Texture::E_Specular)
			{
				name = "texture_specular" + std::to_string(++specularNr);
			}
			else if (m_Textures[i]->GetTextureType() == Texture::E_Normal)
			{
				name = "texture_normal" + std::to_string(++normalNr);
			}

			shader->SetInt(name, i);
			m_Textures[i]->SetActive(i);
		}

		glBindVertexArray(m_Id);
		glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	
	void Mesh::SetupMesh()
	{
		glGenVertexArrays(1, &m_Id);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		glBindVertexArray(m_Id);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Vertex::Normal)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Vertex::TexCoord)));

		glBindVertexArray(0);
	}
}