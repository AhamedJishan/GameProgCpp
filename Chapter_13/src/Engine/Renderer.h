#pragma once

#include <SDL/SDL.h>

namespace jLab
{
	class Renderer
	{
	public:
		Renderer(class Game* game);

		bool Init(int screenWidth, int screenHeight);
		void Shutdown();
		void Draw();

	private:
		int mScreenWidth;
		int mScreenHeight;

		class Game* mGame;
		SDL_Window* mWindow;
		SDL_GLContext mContext;
	};
}