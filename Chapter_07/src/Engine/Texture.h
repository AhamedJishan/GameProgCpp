#pragma once

#include <string>

namespace jLab
{
	class Texture
	{
	public:
		enum TextureType
		{
			E_DIFFUSE,
			E_SPECULAR,
			E_NORMAL
		};

		Texture();
		~Texture();

		bool Load(const std::string& filename, TextureType type = E_DIFFUSE);
		void SetActive(int slot = 0);

		TextureType GetType() { return m_Type; }

		int GetWidth() { return m_Width; }
		int GetHeight() { return m_Height; }

	private:
		unsigned int m_Id;
		TextureType m_Type;

		int m_Width;
		int m_Height;
	};
}