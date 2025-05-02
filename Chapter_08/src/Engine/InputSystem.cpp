#include "InputSystem.h"

#include <cstring>

namespace jLab
{
	InputSystem::InputSystem()
		:m_InputState(InputState())
	{
	}

	bool InputSystem::Init()
	{
		m_InputState.Keyboard.m_CurrentState = SDL_GetKeyboardState(NULL);
		memset(m_InputState.Keyboard.m_PreviousState, 0, SDL_NUM_SCANCODES);

		return true;
	}
	
	void InputSystem::Shutdown()
	{
	}
	
	void InputSystem::PreUpdate()
	{
		memcpy(m_InputState.Keyboard.m_PreviousState, m_InputState.Keyboard.m_CurrentState, SDL_NUM_SCANCODES);
	}
	
	void InputSystem::Update()
	{
		m_InputState.Keyboard.m_CurrentState = SDL_GetKeyboardState(NULL);
	}
}