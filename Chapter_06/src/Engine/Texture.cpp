#include "Texture.h"

#include <SDL/SDL.h>
#include "util/stb_image.h"

namespace jLab
{
	Texture::Texture()
		:m_Type(TextureType::DiffuseTexture),
		m_Height(0),
		m_Width(0),
		m_Id(0)
	{
	}
	
	Texture::Texture(const std::string& filename, TextureType type)
		:m_Type(TextureType::DiffuseTexture),
		m_Height(0),
		m_Width(0),
		m_Id(0)
	{
		Load(filename);
		SetTextureType(type);
	}
	
	Texture::~Texture()
	{
		glDeleteTextures(1, &m_Id);
	}
	
	bool Texture::Load(const std::string& filename)
	{
		int nrChannels;
		GLenum format;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filename.c_str(), &m_Width, &m_Height, &nrChannels, 0);
		
		if (!data)
		{
			SDL_Log("ERROR: Failed to load image: %s", filename.c_str());
			stbi_image_free(data);
			return false;
		}

		if		(nrChannels == 1) format = GL_RED;
		else if (nrChannels == 3) format = GL_RGB;
		else if (nrChannels == 4) format = GL_RGBA;
		else
		{
			SDL_Log("ERROR: %d channeled image are not supported.", nrChannels);
			stbi_image_free(data);
			return false;
		}

		glGenTextures(1, &m_Id);
		glBindTexture(GL_TEXTURE_2D, m_Id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		return true;
	}
	
	void Texture::SetActive(GLuint unit)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_Id);
	}
}