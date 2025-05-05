#include "MouseState.h"

namespace jLab
{
	MouseState::MouseState()
	{
		m_CurrButtons = 0;
		m_PrevButtons = 0;
		m_IsRelative = false;
		m_Position = glm::vec2(0);
		m_ScrollWheel = glm::vec2(0);
	}
	
	bool MouseState::GetButton(int button) const
	{
		return (m_PrevButtons & SDL_BUTTON(button)) && (m_CurrButtons & SDL_BUTTON(button));
	}
	
	bool MouseState::GetButtonDown(int button) const
	{
		return !(m_PrevButtons & SDL_BUTTON(button)) && (m_CurrButtons & SDL_BUTTON(button));
	}
	
	bool MouseState::GetButtonUp(int button) const
	{
		return (m_PrevButtons & SDL_BUTTON(button)) && !(m_CurrButtons & SDL_BUTTON(button));
	}
}