#include "Font.h"

#include <cstdio>
#include <vector>
#include "Game.h"
#include "Texture.h"

namespace jLab
{
	Font::Font(class Game* game)
		:mGame(game)
	{
	}

	Font::~Font()
	{
	}

	bool Font::Load(const std::string& filename)
	{
		std::vector<int> fontSizes = {
			8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42,
			44, 46, 48, 52, 56, 60, 64, 68, 72
		};

		for (int& size : fontSizes)
		{
			TTF_Font* font = TTF_OpenFont(filename.c_str(), size);
			if (!font)
			{
				printf("Failed to load Font '%s' at point size '%d'\n%s\n", filename.c_str(), size, TTF_GetError());
				return false;
			}
			mFontData.emplace(size, font);
		}
		return true;
	}

	void Font::Unload()
	{
		for (auto& it : mFontData)
			TTF_CloseFont(it.second);
		mFontData.clear();
	}

	Texture* Font::RenderText(const std::string& text, const glm::vec4& color, int pointSize)
	{
		const std::string& actualText = mGame->GetText(text);

		SDL_Color sdlColor;
		sdlColor.r = color.r * 255;
		sdlColor.g = color.g * 255;
		sdlColor.b = color.b * 255;
		sdlColor.a = color.a * 255;

		auto it = mFontData.find(pointSize);
		if (it == mFontData.end())
		{
			printf("Loaded Font does not have the point size '%d'", pointSize);
			return nullptr;
		}

		TTF_Font* font = it->second;

		SDL_Surface* surface = TTF_RenderUTF8_Blended(font, actualText.c_str(), sdlColor);
		SDL_Surface* convertedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
		if (!convertedSurface)
		{
			printf("Error occurred while rendering text '%s'", actualText.c_str());
			return nullptr;
		}

		Texture* texture = new Texture();
		texture->CreateFromTexture(convertedSurface);

		SDL_FreeSurface(surface);
		SDL_FreeSurface(convertedSurface);

		return texture;
	}
}