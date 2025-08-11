#include "DialogBox.h"

#include "Game.h"
#include "Renderer.h"

namespace jLab
{

	DialogBox::DialogBox(Game* game, const std::string& text, std::function<void()> onOk)
		:UIScreen(game)
	{
		m_BGPos = glm::vec2(0);
		m_TitlePos = glm::vec2(0, 100);
		m_NextButtonPos = glm::vec2(0);

		m_Background = m_Game->GetRenderer()->GetTexture("Assets/Textures/DialogBG.png");
		SetTitle(text, glm::vec4(0, 0, 0, 1), 30);

		AddButton("Ok", [onOk]()
			{
				onOk();
			});

		AddButton("Cancel", [this]()
			{
				Close();
			});
	}

	DialogBox::~DialogBox()
	{
	}
}