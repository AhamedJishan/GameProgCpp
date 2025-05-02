#include "KeyboardState.h"

#include "InputSystem.h"
#include <algorithm>

namespace jLab
{
	KeyboardState::KeyboardState()
	{
		m_CurrentState = SDL_GetKeyboardState(NULL);
		std::fill(std::begin(m_PreviousState), std::end(m_PreviousState), 0);
	}
	
	bool KeyboardState::GetKeyValue(SDL_Scancode keyCode) const
	{
		return m_CurrentState[keyCode];
	}
	
	ButtonState KeyboardState::GetKeyState(SDL_Scancode keyCode) const
	{
		if (m_CurrentState[keyCode] == 0)
		{
			if (m_PreviousState[keyCode] == 0)
				return ButtonState::E_None;
			else
				return ButtonState::E_Released;
		}
		else
		{
			if (m_PreviousState[keyCode] == 0)
				return ButtonState::E_Pressed;
			else
				return ButtonState::E_Held;
		}
	}
}