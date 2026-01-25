#include "Mesh.h"

#include <GLEW/GL/glew.h>
#include <string>
#include "Texture.h"
#include "Shader.h"

namespace jLab
{
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<class Texture*> textures, bool isSkinned)
	{
		mVertices = vertices;
		mIndices = indices;
		mTextures = textures;
		mIsSkinned = isSkinned;

		SetupMesh();
	}

	Mesh::~Mesh()
	{
		glDeleteBuffers(1, &mEBO);
		glDeleteBuffers(1, &mVBO);
		glDeleteVertexArrays(1, &mId);
	}
	
	void Mesh::Draw(const Shader* shader) const
	{
		int diffuseNr = 0;
		int specularNr = 0;
		int normalNr = 0;

		for (int i = 0; i < mTextures.size(); i++)
		{
			std::string name;
			if (mTextures[i]->GetType() == Texture::Type::Diffuse)
				name = "texture_diffuse" + std::to_string(++diffuseNr);
			if (mTextures[i]->GetType() == Texture::Type::Specular)
				name = "texture_specular" + std::to_string(++specularNr);
			if (mTextures[i]->GetType() == Texture::Type::Normal)
				name = "texture_normal" + std::to_string(++normalNr);

			shader->SetInt(name, i);
			mTextures[i]->SetActive(i);
		}

		if (specularNr != 0)
			shader->SetBool("uHasSpecular", true);
		else
			shader->SetBool("uHasSpecular", false);

		glBindVertexArray(mId);
		glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	
	void Mesh::SetupMesh()
	{
		glGenVertexArrays(1, &mId);
		glGenBuffers(1, &mVBO);
		glGenBuffers(1, &mEBO);

		glBindVertexArray(mId);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Vertex::Normal)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Vertex::TexCoord)));

		if (mIsSkinned)
		{
			glEnableVertexAttribArray(3);
			glVertexAttribIPointer(3, 4, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)(offsetof(Vertex, Vertex::BoneIds)));
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)(offsetof(Vertex, Vertex::BoneWeights)));
		}

		glBindVertexArray(0);
	}
}