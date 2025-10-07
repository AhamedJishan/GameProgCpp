#pragma once

#include "Keyboard.h"

namespace jLab
{
	struct InputState
	{
		Keyboard Keyboard;
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

	private:
		class Game* mGame;

		InputState mInputState;
	};
}