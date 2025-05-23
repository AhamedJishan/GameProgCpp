#pragma once

#include <GLEW/GL/glew.h>
#include <string>

namespace jLab
{
	class Texture
	{
	public:
		enum TextureType
		{
			DiffuseTexture,
			NormalTexture,
			SpecularTexture
		};

		Texture();
		Texture(const std::string& filename, TextureType type);
		~Texture();

		bool Load(const std::string& filename);
		void SetTextureType(TextureType type) { m_Type = type; }
		void SetActive(GLuint unit = 0);

		TextureType GetType() { return m_Type; }
		int GetWidth() { return m_Width; }
		int GetHeight() { return m_Height; }

	private:
		GLuint m_Id;
		int m_Width;
		int m_Height;

		TextureType m_Type;
	};
}