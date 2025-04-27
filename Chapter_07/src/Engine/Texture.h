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

		Texture(const std::string& filename, TextureType type = E_DIFFUSE);
		~Texture();

		void SetActive();

		TextureType GetType() { return m_Type; }

	private:
		void Load(const std::string& filename);

	private:
		unsigned int m_Id;
		TextureType m_Type;

		int m_Width;
		int m_Height;
	};
}