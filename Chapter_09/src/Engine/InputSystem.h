#pragma once

#include "KeyboardState.h"

namespace jLab
{
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

		void PreUpdate();
		void Update();

		InputState& GetState() { return m_InputState; }

	private:
		InputState m_InputState;
	};
}