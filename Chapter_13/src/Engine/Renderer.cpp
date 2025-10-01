#include "Renderer.h"

#include <cstdio>
#include <GLEW/GL/glew.h>

#include "Game.h"

namespace jLab
{
	Renderer::Renderer(Game* game)
	{
		mGame = game;
	}

	bool Renderer::Init(int screenWidth, int screenHeight)
	{
		mScreenWidth = screenWidth;
		mScreenHeight = screenHeight;

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		mWindow = SDL_CreateWindow("Chapter_13: Intermediate Graphics", 50, 50, mScreenWidth, mScreenHeight, SDL_WINDOW_OPENGL);
		if (!mWindow)
		{
			printf("ERROR: Failed to Create Window\n\t'%s'\n", SDL_GetError());
			return false;
		}

		mContext = SDL_GL_CreateContext(mWindow);
		if (!mContext)
		{
			printf("ERROR: Failed to create Gl Context\n\t'%s'\n", SDL_GetError());
			return false;
		}

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			printf("ERROR: Failed to Initialize GLEW\n");
			return false;
		}
		glGetError();

		return true;
	}

	void Renderer::Draw()
	{
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SDL_GL_SwapWindow(mWindow);
	}
}