#pragma once

#include "Engine/UIScreen.h"
#include "Engine/Game.h"
#include "Engine/InputSystem.h"
#include "Engine/DialogBox.h"

namespace jLab
{
	class PauseMenu : public UIScreen
	{
	public:
		PauseMenu(class Game* game)
			:UIScreen(game)
		{
			m_Game->SetState(Game::E_Paused);
			SetTitle("PauseTitle");
			m_Game->GetInputSystem()->GetState().Mouse.SetCursorLocked(false);

			AddButton("ResumeButton", [this]()
				{
					Close();
				});

			AddButton("QuitButton", [this]()
				{
					DialogBox* db = new DialogBox(m_Game, "QuitText", [this]()
						{
							m_Game->SetState(Game::E_Quit);
						});
				});
		}

		~PauseMenu()
		{
			m_Game->SetState(Game::E_Gameplay);
			m_Game->GetInputSystem()->GetState().Mouse.SetCursorLocked(true);
		}

		void ProcessInput(InputState& inputState) override
		{
			UIScreen::ProcessInput(inputState);
			if (inputState.Keyboard.GetKeyDown(SDL_SCANCODE_ESCAPE))
				Close();
		}
	};
}