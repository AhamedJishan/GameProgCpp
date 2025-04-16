#include "Texture.h"

#include "util/stb_image.h"
#include <SDL/SDL.h>

namespace jLab
{
	Texture::Texture()
		:m_Texture(0),
		m_Width(0),
		m_Height(0)
	{
	}
	
	Texture::~Texture()
	{
		glDeleteTextures(1, &m_Texture);
	}

	bool Texture::Load(const std::string& filename)
	{
		int nrChannels;
		GLenum format;

		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filename.c_str(), &m_Width, &m_Height, &nrChannels, 0);
		if (!data)
		{
			SDL_Log("Failed to load image: %s", filename);
			return false;
		}

		if (nrChannels == 3)	format = GL_RGB;
		else if (nrChannels == 4)	format = GL_RGBA;
		else
		{
			SDL_Log("Unsupported number of channels for image: %s, Channels: %d", filename.c_str(), nrChannels);
			stbi_image_free(data);
			return false;
		}

		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D, m_Texture);

		glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
		return true;
	}
	
	void Texture::Bind(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
	}
	
	void Texture::Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}