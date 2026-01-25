#pragma once

#include <unordered_map>
#include <string>
#include <SDL/SDL_ttf.h>
#include <glm/vec4.hpp>

namespace jLab
{
	class Font
	{
	public:
		Font(class Game* game);
		~Font();

		bool Load(const std::string& filename);
		void Unload();

		class Texture* RenderText(const std::string& text, const glm::vec4& color = glm::vec4(1), int pointSize = 30);

	private:
		std::unordered_map<int, TTF_Font*> mFontData;
		class Game* mGame;
	};
}