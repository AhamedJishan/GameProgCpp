#pragma once

#include <SDL/SDL.h>
#include "KeyboardState.h"
#include "MouseState.h"

namespace jLab
{
	enum ButtonState
	{
		E_None,
		E_Pressed,
		E_Held,
		E_Released
	};

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

		// Called right before SDL_PollEvents
		void PreUpdate();
		// Called right after SDL_PollEvents
		void Update();

		void ProcessEvent(SDL_Event& event);

		const InputState& GetInputState() const { return m_InputState; }

		void SetRelativeMouseMode(bool value);

	private:
		InputState m_InputState;
	};
}