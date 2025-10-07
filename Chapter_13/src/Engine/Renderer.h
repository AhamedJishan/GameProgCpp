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

		int GetScreenWidth() const { return mScreenWidth; }
		int GetScreenHeight() const { return mScreenHeight; }

		SDL_Window* GetWindow() const { return mWindow; }

	private:
		int mScreenWidth;
		int mScreenHeight;

		class Game* mGame;
		SDL_Window* mWindow;
		SDL_GLContext mContext;
	};
}