#pragma once

#include <string>
#include <functional>
#include <glm/vec2.hpp>

namespace jLab
{
	class Button
	{
	public:
		Button(const std::string& name, std::function<void()> onClick, class Font* font, glm::vec2 pos, glm::vec2 dimensions);
		~Button();

		void SetName(const std::string& name);

		class Texture* GetNameTexture() { return mNameTexture; }
		glm::vec2 GetPosition() { return mPosition; }
		void SetHighlighted(bool value) { mHighlighted = value; }
		bool GetHighlighted() const { return mHighlighted; }

		bool ContainsPoint(glm::vec2 point);
		void OnClick();

	private:
		std::string mName;
		class Font* mFont;
		class Texture* mNameTexture;
		std::function<void()> mOnClick;
		glm::vec2 mPosition;
		glm::vec2 mDimensions;
		bool mHighlighted;
	};
}