#include "Texture.h"

#include <fstream>
#include <sstream>
#include <GLEW/GL/glew.h>
#include <SDL/SDL.h>
#include "stbi/stb_image.h"

namespace jLab
{
	Texture::Texture()
	{
		m_Id = -1;
		m_Width = 0;
		m_Height = 0;
		m_Type = TextureType::E_Diffuse;
	}
	
	Texture::~Texture()
	{
	}
	
	bool Texture::Load(const std::string& filename, TextureType type)
	{
		m_Type = type;

		int nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filename.c_str(), &m_Width, &m_Height, &nrChannels, 0);
		if (!data)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: %s", filename.c_str());
			return false;
		}

		GLenum format = 0;
		if (nrChannels == 1) format = GL_RED;
		else if (nrChannels == 3) format = GL_RGB;
		else if (nrChannels == 4) format = GL_RGBA;
		else
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%d channel images are not supported", nrChannels);
			stbi_image_free(data);
			return false;
		}

		glGenTextures(1, &m_Id);
		glBindTexture(GL_TEXTURE_2D, m_Id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
		return true;
	}
	
	void Texture::Unload()
	{
		glDeleteTextures(1, &m_Id);
	}
	
	void Texture::SetActive(int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_Id);
	}
}