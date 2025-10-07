#include "Mouse.h"

namespace jLab
{
	Mouse::Mouse()
	{
		mCurrentState = 0;
		mPreviousState = 0;
		mPosition = glm::vec2(0);
		mRelativePosition = glm::vec2(0);
		mScrollWheel = glm::vec2(0);
		mIsCursorLocked = false;
	}

	bool Mouse::GetButton(Uint32 button)
	{
		return ((mPreviousState & SDL_BUTTON(button)) && (mCurrentState & SDL_BUTTON(button)));
	}

	bool Mouse::GetButtonDown(Uint32 button)
	{
		return (!(mPreviousState & SDL_BUTTON(button)) && (mCurrentState & SDL_BUTTON(button)));
	}

	bool Mouse::GetButtonUp(Uint32 button)
	{
		return ((mPreviousState & SDL_BUTTON(button)) && !(mCurrentState & SDL_BUTTON(button)));
	}
}