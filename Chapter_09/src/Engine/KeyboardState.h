#pragma once

#include <SDL/SDL.h>

namespace jLab
{
	class KeyboardState
	{
	public:
		friend class InputSystem;

		KeyboardState();

		bool GetKey(SDL_Scancode keyCode) const;
		bool GetKeyDown(SDL_Scancode keyCode) const;
		bool GetKeyUp(SDL_Scancode keyCode) const;

	private:
		const Uint8* m_CurrentState;
		Uint8 m_PreviousState[SDL_NUM_SCANCODES];
	};
}