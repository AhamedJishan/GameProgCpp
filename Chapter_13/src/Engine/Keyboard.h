#pragma once

#include <SDL/SDL.h>

namespace jLab
{
	class Keyboard
	{
	public:
		bool GetKey(SDL_Scancode scancode);
		bool GetKeyDown(SDL_Scancode scancode);
		bool GetKeyUp(SDL_Scancode scancode);

	private:
		friend class InputSystem;
		Uint8 mPreviousState[SDL_NUM_SCANCODES];
		const Uint8* mCurrentState;
	};
}