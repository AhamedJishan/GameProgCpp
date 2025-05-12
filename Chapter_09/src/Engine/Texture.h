#pragma once

#include <string>

namespace jLab
{
	class Texture
	{
	public:
		enum TextureType
		{
			E_Diffuse,
			E_Normal,
			E_Specular
		};

		Texture();
		~Texture();

		bool Load(const std::string& filename, TextureType type = E_Diffuse);
		void Unload();

		void SetActive(int slot = 0);

		TextureType GetType() const { return m_Type; }
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }

	private:
		unsigned int m_Id;
		TextureType m_Type;

		int m_Width;
		int m_Height;
	};
}