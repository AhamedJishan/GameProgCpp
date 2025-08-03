#include "InputSystem.h"

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
	
	void InputSystem::PreUpdate()
	{
		memcpy(m_InputState.Keyboard.m_PreviousState, m_InputState.Keyboard.m_CurrentState, SDL_NUM_SCANCODES);

		m_InputState.Mouse.m_PreviousButtons = m_InputState.Mouse.m_CurrentButtons;
		m_InputState.Mouse.m_ScrollWheel = glm::vec2(0);
	}
	
	void InputSystem::Update()
	{
		m_InputState.Keyboard.m_CurrentState = SDL_GetKeyboardState(NULL);

		int x = 0, y = 0;
		if (m_InputState.Mouse.IsRelative())
			m_InputState.Mouse.m_CurrentButtons = SDL_GetRelativeMouseState(&x, &y);
		else
			m_InputState.Mouse.m_CurrentButtons = SDL_GetMouseState(&x, &y);

		m_InputState.Mouse.m_Position.x = x;
		m_InputState.Mouse.m_Position.y = y;

		m_InputState.Mouse.m_Position = glm::vec2(x, y);
		
	}
	
	void InputSystem::ProcessEvent(SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_MOUSEWHEEL:
			m_InputState.Mouse.m_ScrollWheel = glm::vec2(event.wheel.x, event.wheel.y);
			break;
		default:
			break;
		}
	}
	
	void InputSystem::SetRelativeMouseMode(bool value)
	{
		SDL_bool set = value ? SDL_TRUE : SDL_FALSE;
		SDL_SetRelativeMouseMode(set);

		m_InputState.Mouse.m_IsRelative = value;
	}
}