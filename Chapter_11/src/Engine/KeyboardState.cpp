#include "KeyboardState.h"

#include <cstring>

namespace jLab
{
	KeyboardState::KeyboardState()
	{
		m_CurrentState = nullptr;
		memset(m_PreviousState, 0, SDL_NUM_SCANCODES);
	}
	
	bool KeyboardState::GetKey(SDL_Scancode key) const
	{
		return (m_PreviousState[key] && m_CurrentState[key]);
	}
	
	bool KeyboardState::GetKeyDown(SDL_Scancode key) const
	{
		return (!m_PreviousState[key] && m_CurrentState[key]);
	}
	
	bool KeyboardState::GetKeyUp(SDL_Scancode key) const
	{
		return (m_PreviousState[key] && !m_CurrentState[key]);
	}
}