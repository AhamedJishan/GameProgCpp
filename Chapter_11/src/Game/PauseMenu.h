#pragma once

#include "Engine/UIScreen.h"
#include "Engine/Game.h"
#include "Engine/InputSystem.h"

namespace jLab
{
	class PauseMenu : public UIScreen
	{
	public:
		PauseMenu(class Game* game)
			:UIScreen(game)
		{
			m_Game->SetState(Game::E_Paused);
			SetTitle("PAUSED");
		}

		~PauseMenu()
		{
			m_Game->SetState(Game::E_Gameplay);
		}

		void ProcessInput(InputState& inputState) override
		{
			UIScreen::ProcessInput(inputState);
			if (inputState.Keyboard.GetKeyDown(SDL_SCANCODE_ESCAPE))
				Close();
		}
	};
}