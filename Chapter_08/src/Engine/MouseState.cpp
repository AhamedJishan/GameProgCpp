#include "MouseState.h"

#include <SDL/SDL.h>
#include "InputSystem.h"

namespace jLab
{
	MouseState::MouseState()
	{
		m_Position = glm::vec2(0);
		m_PreviousState = 0;
		m_CurrentState = 0;
	}
	
	glm::vec2 MouseState::GetPosition() const
	{
		return m_Position;
	}
	
	bool MouseState::GetButtonValue(int button) const
	{
		return m_CurrentState & SDL_BUTTON(button);
	}
	
	ButtonState MouseState::GetButtonState(int button) const
	{
		if (m_CurrentState & SDL_BUTTON(button))
		{
			if (m_PreviousState & SDL_BUTTON(button))
				return ButtonState::E_Held;
			else
				return ButtonState::E_Pressed;
		}
		else
		{
			if(m_PreviousState & SDL_BUTTON(button))
				return ButtonState::E_Released;
			else
				return ButtonState::E_None;
		}
	}
}