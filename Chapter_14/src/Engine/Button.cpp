#include "Button.h"

#include <cstdio>
#include "Font.h"

namespace jLab
{
	Button::Button(const std::string& name, std::function<void()> onClick, Font* font, glm::vec2 pos, glm::vec2 dimensions)
		:mName(name)
		,mOnClick(onClick)
		,mFont(font)
		,mPosition(pos)
		,mDimensions(dimensions)
		,mNameTexture(nullptr)
		,mHighlighted(false)
	{
		SetName(mName);
	}

	Button::~Button()
	{
		if (mNameTexture)
			delete mNameTexture;
	}
	
	void Button::SetName(const std::string& name)
	{
		if (mNameTexture)
			delete mNameTexture;
		mNameTexture = mFont->RenderText(name);
	}
	
	bool Button::ContainsPoint(glm::vec2 point)
	{
		bool no =	point.x < (mPosition.x - mDimensions.x / 2.0f) ||
					point.x > (mPosition.x + mDimensions.x / 2.0f) ||
					point.y < (mPosition.y - mDimensions.y / 2.0f) ||
					point.y > (mPosition.y + mDimensions.y / 2.0f);
		return !no;
	}
	
	void Button::OnClick()
	{
		if (mOnClick)
			mOnClick();
	}
}