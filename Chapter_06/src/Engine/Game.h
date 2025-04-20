#pragma once

#include <SDL/SDL.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Texture.h"

namespace jLab
{
	class Game
	{
	public:
		Game();

		bool Init();
		void Run();
		void ShutDown();

		void AddActor(class Actor* actor);
		void RemoveActor(class Actor* actor);

		class Texture* GetTexture(const std::string& filename, Texture::TextureType type);

	private:
		void UpdateGame();
		void ProcessInput();
		void GenerateOutput();

	private:
		SDL_Window* m_Window;
		SDL_GLContext m_Context;

		bool m_IsRunning;
		int m_TicksCount;

		std::vector<class Actor*> m_Actors;
		std::vector<class Actor*> m_PendingActors;
		bool m_UpdatingActors;

		std::unordered_map<std::string, class Texture*> m_Textures;
	};
}