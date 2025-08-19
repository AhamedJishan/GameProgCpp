#pragma once

#include "UIScreen.h"
#include <string>
#include <functional>

namespace jLab
{
	class DialogBox : public UIScreen
	{
	public:
		DialogBox(class Game* game, const std::string& text, std::function<void()> onOk);
		~DialogBox();
	};
}