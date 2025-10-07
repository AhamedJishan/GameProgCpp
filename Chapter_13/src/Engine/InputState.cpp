#include "InputSystem.h"

#include <SDL/SDL.h>

#include "Game.h"
#include "Renderer.h"

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

	void Shutdown()
	{

	}

	void InputSystem::PreUpdate()
	{
		// KEYBOARD
		memcpy(mInputState.Keyboard.mPreviousState, mInputState.Keyboard.mCurrentState, SDL_NUM_SCANCODES);

		// MOUSE
		mInputState.Mouse.mPreviousState = mInputState.Mouse.mCurrentState;
		mInputState.Mouse.mScrollWheel = glm::vec2(0);
	}

	void InputSystem::Update()
	{
		// KEYBOARD
		mInputState.Keyboard.mCurrentState = SDL_GetKeyboardState(NULL);

		// MOUSE
		int x, y;
		int rx, ry;
		mInputState.Mouse.mCurrentState = SDL_GetMouseState(&x, &y);
		SDL_GetRelativeMouseState(&rx, &ry);

		int screenCenterX = std::round(mGame->GetRenderer()->GetScreenWidth() / 2.0f);
		int screenCenterY = std::round(mGame->GetRenderer()->GetScreenHeight() / 2.0f);

		x -= screenCenterX;
		y = screenCenterY - y;

		mInputState.Mouse.mPosition = glm::vec2(x, y);
		mInputState.Mouse.mRelativePosition = glm::vec2(rx, ry);

		if (mInputState.Mouse.IsCursorLocked())
			SDL_WarpMouseInWindow(mGame->GetRenderer()->GetWindow(), screenCenterX, screenCenterY);
	}
	void InputSystem::SetCursorLocked(const bool value)
	{
		mInputState.Mouse.mIsCursorLocked = value;

		SDL_bool set = value ? SDL_TRUE : SDL_FALSE;
		SDL_SetRelativeMouseMode(set);
	}
}