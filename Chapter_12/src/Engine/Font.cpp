#include "Font.h"

#include <vector>
#include "Texture.h"
#include "Game.h"

namespace jLab
{
	Font::Font(Game* game)
		:m_Game(game)
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
			if (font == nullptr)
			{
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load font '%s'", filename.c_str());
				return false;
			}
			m_FontData.emplace(size, font);
		}

		return true;
	}
	
	void Font::Unload()
	{
		for (auto& iter : m_FontData)
		{
			TTF_CloseFont(iter.second);
		}
		m_FontData.clear();
	}
	
	Texture* Font::RenderText(const std::string& text, const glm::vec4& color, int pointSize)
	{
		Texture* texture = nullptr;
		std::string actualText = m_Game->GetText(text);

		SDL_Color sdlColor;
		sdlColor.r = color.r * 255;
		sdlColor.g = color.g * 255;
		sdlColor.b = color.b * 255;
		sdlColor.a = color.a * 255;

		auto iter = m_FontData.find(pointSize);
		if (iter != m_FontData.end())
		{
			TTF_Font* font = iter->second;

			SDL_Surface* surface = TTF_RenderUTF8_Blended(font, actualText.c_str(), sdlColor);
			SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);

			if (surface != nullptr)
			{
				texture = new Texture();
				texture->CreateFromSurface(converted);
			}
			SDL_FreeSurface(surface);
			SDL_FreeSurface(converted);
		}
		else
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Point Size '%d' is unsupported!", pointSize);
			return nullptr;
		}

		return texture;
	}
}