#pragma once

#include <GLEW/GL/glew.h>
#include <string>

namespace jLab
{
	class Texture
	{
	public:
		Texture();
		~Texture();

		bool Load(const std::string& filename);

		void Bind(unsigned int slot = 0);
		void Unbind();

		int GetWidth() { return m_Width; }
		int GetHeight() { return m_Height; }

	private:
		GLuint m_Texture;
		int m_Width;
		int m_Height;
	};
}