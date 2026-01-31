#pragma once

#include "Component.h"

namespace jLab
{
	class SpriteComponent : public Component
	{
	public:
		SpriteComponent(class Actor* owner, int drawOrder = 100);
		~SpriteComponent();

		void Draw(const class Shader* shader);
		void SetTexture(class Texture* texture);

		int GetWidth() const { return mWidth; }
		int GetHeight() const { return mHeight; }
		int GetDrawOrder() const { return mDrawOrder; }

		bool IsVisible() const { return mVisible; }
		void SetVisible(bool value) { mVisible = value; }

		Component::TypeID GetType() const override { return Component::TSpriteComponent; }

	private:
		class Texture* mTexture;
		int mWidth;
		int mHeight;
		int mDrawOrder;
		bool mVisible;
	};
}