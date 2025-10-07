#include "InputSystem.h"

#include <SDL/SDL.h>

#include "Game.h"

namespace jLab
{
	InputSystem::InputSystem(Game* game)
		:mGame(game)
	{
		mInputState = InputState();
	}

	bool InputSystem::Init()
	{
		memset(mInputState.Keyboard.mPreviousState, 0, SDL_NUM_SCANCODES);
		mInputState.Keyboard.mCurrentState = SDL_GetKeyboardState(NULL);

		return true;
	}

	void InputSystem::PreUpdate()
	{
		memcpy(mInputState.Keyboard.mPreviousState, mInputState.Keyboard.mCurrentState, SDL_NUM_SCANCODES);
	}

	void InputSystem::Update()
	{
		mInputState.Keyboard.mCurrentState = SDL_GetKeyboardState(NULL);
	}
}