#pragma once

#include "Keyboard.h"
#include "Mouse.h"

namespace jLab
{
	struct InputState
	{
		Keyboard Keyboard;
		Mouse Mouse;
	};
	
	class InputSystem
	{
	public:
		InputSystem(class Game* game);

		bool Init();
		void Shutdown();

		void PreUpdate();
		void Update();

		class Game* GetGame() const { return mGame; }
		InputState GetState() const { return mInputState; }

		void SetCursorLocked(const bool value);

	private:
		class Game* mGame;

		InputState mInputState;
	};
}