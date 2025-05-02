#pragma once

#include "KeyboardState.h"

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

		const InputState& GetInputState() const { return m_InputState; }

	private:
		InputState m_InputState;
	};
}