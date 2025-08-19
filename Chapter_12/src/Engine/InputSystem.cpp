#include "InputSystem.h"

#include <SDL/SDL.h>
#include <cstring>
#include "Game.h"
#include "Renderer.h"

namespace jLab
{
	InputSystem::InputSystem(Game* game)
		:m_Game(game)
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
		int rx = 0, ry = 0;
		m_InputState.Mouse.m_CurrentState = SDL_GetMouseState(&x, &y);
		SDL_GetRelativeMouseState(&rx, &ry);

		x -= m_Game->GetRenderer()->GetWidth() / 2.0f;
		y = m_Game->GetRenderer()->GetHeight() / 2.0f - y;

		m_InputState.Mouse.m_Position = glm::vec2(x, y);
		m_InputState.Mouse.m_RelativePosition = glm::vec2(rx, ry);

		if(m_InputState.Mouse.IsCursorLocked())
		{
			int lockXPos = m_Game->GetRenderer()->GetWidth() / 2.0f;
			int lockYPos = m_Game->GetRenderer()->GetHeight() / 2.0f;
			SDL_WarpMouseInWindow(m_Game->GetRenderer()->GetWindow(), lockXPos, lockYPos);
		}
	}
}