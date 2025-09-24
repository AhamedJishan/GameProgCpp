#pragma once

#include <string>
#include <SDL/SDL_surface.h>
#include <assimp/scene.h>

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

		bool Load(const std::string& filename, bool flipVertically = true, TextureType type = E_Diffuse);
		bool Load(const std::string& filename, const aiTexture* data, bool flipVertically = true, TextureType type = E_Diffuse);
		void Unload();

		void SetActive(int slot = 0);

		void CreateFromSurface(SDL_Surface* surface);

		TextureType GetTextureType() const { return m_Type; }
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }

	private:
		unsigned int m_Id;
		TextureType m_Type;

		int m_Width;
		int m_Height;
	};
}