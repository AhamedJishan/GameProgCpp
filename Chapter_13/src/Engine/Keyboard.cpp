#include "Keyboard.h"

namespace jLab
{
	bool Keyboard::GetKey(SDL_Scancode scancode)
	{
		return (mPreviousState[scancode] && mCurrentState[scancode]);
	}

	bool Keyboard::GetKeyDown(SDL_Scancode scancode)
	{
		return (!mPreviousState[scancode] && mCurrentState[scancode]);
	}

	bool Keyboard::GetKeyUp(SDL_Scancode scancode)
	{
		return (mPreviousState[scancode] && !mCurrentState[scancode]);
	}
}