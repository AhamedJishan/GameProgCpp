#include "DialogBox.h"

#include "Game.h"
#include "Renderer.h"
#include "Button.h"

namespace jLab
{
	DialogBox::DialogBox(Game* game, const std::string& text, std::function<void()> onOk)
		:UIScreen(game)
	{
		mBGPos = glm::vec2(0);
		mTitlePos = glm::vec2(0, 100);
		mNextButtonPos = glm::vec2(0);
		mBGTexture = mGame->GetRenderer()->GetTexture("Assets/Textures/DialogBG.png");
		SetTitle(text);

		AddButton("OKButton", onOk);
		AddButton("CancelButton", [this]
			{
				Close();
			});
	}

	DialogBox::~DialogBox()
	{
	}
}