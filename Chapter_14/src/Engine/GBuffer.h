#pragma once

#include <vector>

namespace jLab
{
	class GBuffer
	{
	public:
		enum class Type : int
		{
			Diffuse = 0,
			Normal,
			WorldPosition,
			NUM_GBUFFER_TEXTURES
		};

		GBuffer();
		~GBuffer();

		bool Create(int width, int height);
		void Destroy();

		unsigned int GetId() const { return mId; }
		class Texture* GetTexture(Type type);

		void SetTexturesActive();

	private:
		unsigned int mId;
		std::vector<class Texture*> mTextures;
	};
}