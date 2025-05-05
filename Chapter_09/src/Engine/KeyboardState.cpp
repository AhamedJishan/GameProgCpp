#include "KeyboardState.h"

#include <cstring>

namespace jLab
{
	KeyboardState::KeyboardState()
	{
		m_CurrentState = nullptr;
		memset(m_PreviousState, 0, SDL_NUM_SCANCODES);
	}
	
	bool KeyboardState::GetKey(SDL_Scancode keyCode) const
	{
		return (m_PreviousState[keyCode]) && (m_CurrentState[keyCode]);
	}
	
	bool KeyboardState::GetKeyDown(SDL_Scancode keyCode) const
	{
		return (!m_PreviousState[keyCode]) && (m_CurrentState[keyCode]);
	}
	
	bool KeyboardState::GetKeyUp(SDL_Scancode keyCode) const
	{
		return (m_PreviousState[keyCode]) && (!m_CurrentState[keyCode]);
	}
}