#pragma once

#include <SDL.h>

#include <string>
#include <vector>
#include <unordered_map>
#include "Actor.h"
#include "SpriteComponent.h"

namespace ch2
{
	class Game
	{
	public:
		Game();

		bool Init();
		void RunLoop();
		void Shutdown();

		void AddActor(class Actor* actor);
		void RemoveActor(class Actor* actor);

		void AddSprite(class SpriteComponent* sprite);
		void RemoveSprite(class SpriteComponent* sprite);

		void LoadData();
		void UnLoadData();

		SDL_Texture* GetTexture(const char* filename);
	private:
		void ProcessInput();
		void UpdateGame();
		void GenerateOutput();

	private:
		SDL_Window* m_Window;
		SDL_Renderer* m_Renderer;

		bool m_IsRunning;
		Uint32 m_TicksCount;

		std::vector<Actor*> m_Actors;
		std::vector<Actor*> m_PendingActors;
		bool m_UpdatingActor;

		std::unordered_map<std::string, SDL_Texture*> m_Textures;
		std::vector<class SpriteComponent*> m_Sprites;

		class Ship* m_Ship;
	};

}