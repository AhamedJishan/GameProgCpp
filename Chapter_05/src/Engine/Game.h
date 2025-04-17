#pragma once

#include <SDL/SDL.h>
#include <vector>
#include <unordered_map>
#include <string>

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

		void AddSprite(class SpriteComponent* sprite);
		void RemoveSprite(class SpriteComponent* sprite);

		class Texture* GetTexture(const std::string& filename);

		void AddAsteroid(class Asteroid* asteroid);
		void RemoveAsteroid(class Asteroid* asteroid);
		std::vector<class Asteroid*>& GetAsteroids() { return m_Asteroids; }

	private:
		void ProcessInput();
		void UpdateGame();
		void GenerateOutput();

		void InitSpriteVerts();
		bool InitShaders();

		void LoadData();
		void UnloadData();

	private:
		SDL_Window* m_Window;
		SDL_GLContext m_Context;

		class VertexArray* m_SpriteVerts;
		class Shader* m_SpriteShader;

		bool m_UpdatingActors;
		std::vector<class Actor*> m_Actors;
		std::vector<class Actor*> m_PendingActors;

		std::unordered_map<std::string, class Texture*> m_Textures;
		std::vector<class SpriteComponent*> m_Sprites;
		
		bool m_IsRunning;
		int m_TicksCount;

		class Ship* m_Ship;
		std::vector<class Asteroid*> m_Asteroids;
	};
}