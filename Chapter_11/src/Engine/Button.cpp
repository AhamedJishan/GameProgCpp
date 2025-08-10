#include "Button.h"

#include "Font.h"
#include "Texture.h"

namespace jLab
{
	Button::Button(const std::string& name, Font* font, std::function<void()> onClick, glm::vec2 position, glm::vec2 dimensions)
		:m_Name(name)
		,m_Font(font)
		,m_OnClick(onClick)
		,m_Position(position)
		,m_Dimensions(dimensions)
		,m_Highlighted(false)
	{
		SetName(m_Name);
	}

	Button::~Button()
	{
		if (m_NameTexture)
		{
			m_NameTexture->Unload();
			delete m_NameTexture;
		}
	}

	void Button::SetName(const std::string& name)
	{
		m_Name = name;

		if (m_NameTexture)
		{
			m_NameTexture->Unload();
			delete m_NameTexture;
			m_NameTexture = nullptr;
		}

		m_NameTexture = m_Font->RenderText(m_Name);
	}
	
	bool Button::ContainsPoint(const glm::vec2& point)
	{
		bool notContains =	point.x < (m_Position.x - m_Dimensions.x / 2.0f) ||
							point.x > (m_Position.x + m_Dimensions.x / 2.0f) ||
							point.y < (m_Position.y - m_Dimensions.y / 2.0f) ||
							point.y > (m_Position.y + m_Dimensions.y / 2.0f) ;
		return !notContains;
	}
	
	void Button::OnClick()
	{
		if (m_OnClick)
			m_OnClick();
	}
}