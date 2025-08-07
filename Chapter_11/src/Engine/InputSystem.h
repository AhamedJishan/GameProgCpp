#pragma once

#include "KeyboardState.h"
#include "MouseState.h"

namespace jLab
{
	struct InputState
	{
		KeyboardState Keyboard;
		MouseState Mouse;
	};

	class InputSystem
	{
	public:
		InputSystem();

		bool Init();
		void Shutdown();

		void ProcessInput(SDL_Event& event);
		void PreUpdate();
		void Update();

		InputState& GetState() { return m_InputState; }

		void SetRelativeMouseMode(bool value);

	private:
		InputState m_InputState;
	};
}