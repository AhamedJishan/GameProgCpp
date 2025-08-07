#include "InputSystem.h"

#include <SDL/SDL.h>
#include <cstring>

#include <iostream>

namespace jLab
{
	InputSystem::InputSystem()
	{
		m_InputState = InputState();
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

	void InputSystem::ProcessInput(SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_MOUSEWHEEL:
			m_InputState.Mouse.m_ScrollWheel = glm::vec2(static_cast<float>(event.wheel.x), static_cast<float>(event.wheel.y));
			break;
		default:
			break;
		}
	}

	void InputSystem::PreUpdate()
	{
		// Keyboard
		memcpy(m_InputState.Keyboard.m_PreviousState, m_InputState.Keyboard.m_CurrentState, SDL_NUM_SCANCODES);

		// Mouse
		m_InputState.Mouse.m_PreviousState = m_InputState.Mouse.m_CurrentState;
		m_InputState.Mouse.m_ScrollWheel = glm::vec2(0);
	}

	void InputSystem::Update()
	{
		// Keyboard
		m_InputState.Keyboard.m_CurrentState = SDL_GetKeyboardState(NULL);

		// Mouse
		int x = 0, y = 0;
		if (m_InputState.Mouse.IsRelative())
			m_InputState.Mouse.m_CurrentState = SDL_GetRelativeMouseState(&x, &y);
		else
			m_InputState.Mouse.m_CurrentState = SDL_GetMouseState(&x, &y);

		m_InputState.Mouse.m_Position.x = static_cast<float>(x);
		m_InputState.Mouse.m_Position.y = static_cast<float>(y);
	}

	void InputSystem::SetRelativeMouseMode(bool value)
	{
		SDL_bool setValue = value ? SDL_TRUE : SDL_FALSE;
		SDL_SetRelativeMouseMode(setValue);

		m_InputState.Mouse.m_IsRelative = value;
	}
}