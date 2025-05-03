#include "InputSystem.h"

#include <cstring>
#include "Game.h"

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
		// KEYBOARD
		memcpy(m_InputState.Keyboard.m_PreviousState, m_InputState.Keyboard.m_CurrentState, SDL_NUM_SCANCODES);

		// MOUSE
		m_InputState.Mouse.m_PreviousState = m_InputState.Mouse.m_CurrentState;
	}
	
	void InputSystem::Update()
	{
		// KEYBOARD
		m_InputState.Keyboard.m_CurrentState = SDL_GetKeyboardState(NULL);

		// MOUSE
		int x, y;
		m_InputState.Mouse.m_CurrentState = SDL_GetMouseState(&x, &y);
		m_InputState.Mouse.m_Position.x = static_cast<float>(x - WINDOW_WIDTH/2.0f);
		m_InputState.Mouse.m_Position.y = static_cast<float>(WINDOW_HEIGHT/2.0f - y);
	}
}