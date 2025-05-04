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
		m_InputState.Mouse.m_ScrollWheel = glm::vec2(0);
	}
	
	void InputSystem::Update()
	{
		// KEYBOARD
		m_InputState.Keyboard.m_CurrentState = SDL_GetKeyboardState(NULL);

		// MOUSE
		int x = 0, y = 0;
		if (m_InputState.Mouse.m_RelativeMode)
			m_InputState.Mouse.m_CurrentState = SDL_GetRelativeMouseState(&x, &y);
		else
			m_InputState.Mouse.m_CurrentState = SDL_GetMouseState(&x, &y);

		m_InputState.Mouse.m_Position.x = static_cast<float>(x);
		m_InputState.Mouse.m_Position.y = static_cast<float>(y);
	}


	void InputSystem::ProcessEvent(SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_MOUSEWHEEL:
		{
			m_InputState.Mouse.m_ScrollWheel = glm::vec2(static_cast<float>(event.wheel.x), static_cast<float>(event.wheel.y));
			break;
		}
		default:
			break;
		}
	}

	void InputSystem::SetRelativeMouseMode(bool value)
	{
		SDL_bool set = value ? SDL_TRUE : SDL_FALSE;
		SDL_SetRelativeMouseMode(set);

		m_InputState.Mouse.m_RelativeMode = value;
	}
}