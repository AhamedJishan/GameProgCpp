#pragma once

#include <string>
#include <functional>

#include "UIScreen.h"

namespace jLab
{
	class DialogBox : public UIScreen
	{
	public:
		DialogBox(class Game* game, const std::string& text, std::function<void()> onOk);
		~DialogBox();
	};
}