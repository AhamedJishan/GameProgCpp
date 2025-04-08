#pragma once

#include "Actor.h"
#include <vector>

namespace jLab
{
	class Grid : public Actor
	{
	public:
		Grid(class Game* game);

		void UpdateActor(float deltaTime) override;

		void ProcessClicks(int x, int y);
		bool FindPath(class Tile* start, class Tile* end);
		void BuildTower();

		class Tile* GetStartTile();
		class Tile* GetEndTile();

	private:
		void SelectTile(int x, int y);
		void UpdatePathTiles();

		class Tile* m_SelectedTile;
		std::vector<std::vector<class Tile*>> m_Tiles;
		float m_NextEnemyTime;
		const float m_EnemyTime = 1.5f;
		const unsigned int m_NumRows = 7;
		const unsigned int m_NumCols = 20;
		const float m_StartY = 168.0f;
		const float m_TileSize = 64.0f;
	};
}