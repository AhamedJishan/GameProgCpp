#pragma once

#include <SDL/SDL.h>

namespace jLab
{
	class KeyboardState
	{
	public:
		friend class InputSystem;

		KeyboardState();

		bool GetKey(SDL_Scancode key) const;
		bool GetKeyDown(SDL_Scancode key) const;
		bool GetKeyUp(SDL_Scancode key) const;

	private:
		const Uint8* m_CurrentState;
		Uint8 m_PreviousState[SDL_NUM_SCANCODES];
	};
}