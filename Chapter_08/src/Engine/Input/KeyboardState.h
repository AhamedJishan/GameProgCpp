#pragma once

#include <cstdint>
#include <SDL/SDL.h>

namespace jLab
{
	class KeyboardState
	{
	public:
		friend class InputSystem;

		KeyboardState();

		bool GetKeyValue(SDL_Scancode keyCode) const;
		enum ButtonState GetKeyState(SDL_Scancode keyCode) const;

	private:
		const uint8_t* m_CurrentState;
		uint8_t m_PreviousState[SDL_NUM_SCANCODES];
	};
}