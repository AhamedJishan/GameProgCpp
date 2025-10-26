#pragma once

#include <string>
#include <SDL/SDL.h>
#include <assimp/scene.h>

namespace jLab
{
	class Texture
	{
	public:
		enum class Type
		{
			Diffuse,
			Normal,
			Specular
		};

		Texture();
		~Texture();

		bool Load(const std::string& filename, bool flipVertically = false, Type type = Type::Diffuse);
		bool Load(const std::string& filename, const aiTexture* textureData, bool flipVertically = false, Type type = Type::Diffuse);

		void CreateFromTexture(SDL_Surface* surface);

		void Unload();

		void SetActive(int slot = 0);

		const std::string& GetName() const { return mName; }
		int GetWidth() const { return mWidth; }
		int GetHeight() const { return mHeight; }
		Type GetType() const { return mType; }

	private:
		std::string mName;

		unsigned int mId;

		int mWidth;
		int mHeight;

		Type mType;
	};
}