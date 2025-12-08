#pragma once

#include "Engine/UIScreen.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "Engine/Game.h"
#include "Engine/InputSystem.h"
#include "Engine/DialogBox.h"

namespace jLab
{
	class PauseMenu : public UIScreen
	{
	public:
		PauseMenu(Game* game)
			:UIScreen(game)
		{
			mGame->GetInputSystem()->SetCursorLocked(false);
			mGame->SetState(Game::GameState::Paused);

			SetTitle("PauseTitle", glm::vec4(0, 0, 0, 1.0f));
			mTitlePos = glm::vec2(0, 100);

			mNextButtonPos = glm::vec2(0);
			AddButton("ResumeButton", [this]()
				{
					Close();
				});
			AddButton("QuitButton", [this]()
				{
					new DialogBox(mGame, "QuitText", [this]()
						{
							mGame->SetState(Game::GameState::Quit);
						});
				});
		}

		~PauseMenu()
		{
			mGame->GetInputSystem()->SetCursorLocked(true);
			mGame->SetState(Game::GameState::Gameplay);
		}
	};
}