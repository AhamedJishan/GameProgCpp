#pragma once

#include "SDL/SDL.h"
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

		void PreUpdate();
		void Update();

		void ProcessEvent(SDL_Event& event);

		const InputState& GetInputState() const { return m_InputState; }

		void SetRelativeMouseMode(bool value);

	private:
		InputState m_InputState;
	};
}