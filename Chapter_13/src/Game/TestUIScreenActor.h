#pragma once

#include "Engine/UIScreen.h"
#include "Engine/Game.h"

namespace jLab
{
	class TestUIScreenActor : public UIScreen
	{
	public:
		TestUIScreenActor(class Game* game)
			:UIScreen(game)
		{
			mBGPos = glm::vec2(0, 210);
			SetTitle("PauseTitle", glm::vec4(0, 0, 0, 1));
			mTitlePos = glm::vec2(0, 320);
		}
	};
}