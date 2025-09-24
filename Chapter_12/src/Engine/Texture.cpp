#include "Texture.h"

#include <fstream>
#include <sstream>
#include <GLEW/GL/glew.h>
#include <stbi/stb_image.h>
#include <SDL/SDL.h>

namespace jLab
{
	Texture::Texture()
	{
		m_Id = -1;
		m_Type = E_Diffuse;
		m_Height = -1;
		m_Width = -1;
	}

	Texture::~Texture()
	{
	}

	bool Texture::Load(const std::string& filename, bool flipVertically, TextureType type)
	{
		m_Type = type;

		int nrChannels;
		stbi_set_flip_vertically_on_load(flipVertically);
		unsigned char* data = stbi_load(filename.c_str(), &m_Width, &m_Height, &nrChannels, 0);
		if (!data)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image '%s'", filename.c_str());
			return false;
		}

		GLenum format;
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

	bool Texture::Load(const std::string& filename, const aiTexture* data, bool flipVertically, TextureType type)
	{
		m_Type = type;

		unsigned char* pixels = nullptr;
		int channels=0;

		if (data->mHeight == 0)
		{
			// Compressed texture: aTex->pcData is a blob (size = mWidth)
			int sizeInBytes = static_cast<int>(data->mWidth);
			unsigned char* blob = reinterpret_cast<unsigned char*>(data->pcData);

			stbi_set_flip_vertically_on_load(flipVertically); // optional, based on your needs
			pixels = stbi_load_from_memory(blob, sizeInBytes, &m_Width, &m_Height, &channels, STBI_rgb_alpha);
			if (!pixels)
			{
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to decode embedded texture '%s': %s",
					filename.c_str(), stbi_failure_reason());
				return false;
			}
		}
		else
		{
			// Raw BGRA data
			m_Width = data->mWidth;
			m_Height = data->mHeight;
			channels = 4; // aiTexel = 4 bytes (BGRA)

			size_t dataSize = static_cast<size_t>(m_Width) * static_cast<size_t>(m_Height) * 4u;
			pixels = new unsigned char[dataSize];
			memcpy(pixels, data->pcData, dataSize);

			// NOTE: data is BGRA, if colors look wrong, swap R<->B or use GL_BGRA
		}

		// Upload to OpenGL
		GLenum format = GL_RGBA; // safest default

		glGenTextures(1, &m_Id);
		glBindTexture(GL_TEXTURE_2D, m_Id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Free CPU pixels
		if (data->mHeight == 0)
			stbi_image_free(pixels);
		else
			delete[] pixels;

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

	void Texture::CreateFromSurface(SDL_Surface* surface)
	{
		m_Width = surface->w;
		m_Height = surface->h;
		glGenTextures(1, &m_Id);
		
		glBindTexture(GL_TEXTURE_2D, m_Id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
}