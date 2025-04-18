#pragma once

#include <SDL/SDL.h>
#include <vector>
#include <unordered_map>
#include <string>
#include "util/Math.h"

namespace jLab
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

		// Sprites related stuff
		
		SDL_Texture* GetTexture(const std::string& filename);

		class Grid* GetGrid() { return m_Grid; }
		std::vector<class Enemy*>& GetEnemies() { return m_Enemies; }
		class Enemy* GetNearestEnemy(const Vector2& pos);

	private:
		void ProcessInput();
		void UpdateGame();
		void GenerateOutput();

		void LoadData();
		void UnloadData();

	private:
		Uint32			m_TicksCount;
		SDL_Renderer*	m_Renderer;
		SDL_Window*		m_Window;
		bool			m_IsRunning;

		bool m_UpdatingActors;
		std::vector<class Actor*> m_Actors;
		std::vector<class Actor*> m_PendingActors;

		std::unordered_map<std::string, SDL_Texture*> m_Textures;
		std::vector<class SpriteComponent*> m_Sprites;

		class Grid* m_Grid;
		std::vector<class Enemy*> m_Enemies;
	};
}