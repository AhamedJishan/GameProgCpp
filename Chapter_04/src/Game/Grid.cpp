#include "Grid.h"

#include "Tile.h"

namespace jLab
{
	Grid::Grid(Game* game)
		:Actor(game),
		m_SelectedTile(nullptr)
	{
		m_Tiles.resize(m_NumRows);
		for (int i = 0; i < m_Tiles.size(); i++)
			m_Tiles[i].resize(m_NumCols);

		for (int i = 0; i < m_NumRows; i++)
		{
			for (int j = 0; j < m_NumCols; j++)
			{
				m_Tiles[i][j] = new Tile(game);
				m_Tiles[i][j]->SetPosition( Vector2((m_TileSize / 2 + j * m_TileSize), (m_StartY + i * m_TileSize)) );
			}
		}

		GetStartTile()->SetTileState(Tile::EStart);
		GetEndTile()->SetTileState(Tile::EBase);

		for (int y = 0; y < m_NumRows; y++)
		{
			for (int x = 0; x < m_NumCols; x++)
			{
				if (x > 0)
					m_Tiles[y][x]->m_Adjacents.emplace_back(m_Tiles[y][x - 1]);
				if (x < m_NumCols - 1)
					m_Tiles[y][x]->m_Adjacents.emplace_back(m_Tiles[y][x + 1]);
				if (y > 0)
					m_Tiles[y][x]->m_Adjacents.emplace_back(m_Tiles[y - 1][x]);
				if (y < m_NumRows - 1)
					m_Tiles[y][x]->m_Adjacents.emplace_back(m_Tiles[y + 1][x]);
			}
		}

		FindPath(GetEndTile(), GetStartTile());
		UpdatePathTiles();

		m_NextEnemyTime = m_EnemyTime;
	}
	
	void Grid::UpdateActor(float deltaTime)
	{
		Actor::UpdateActor(deltaTime);

		m_NextEnemyTime -= deltaTime;
		if (m_NextEnemyTime <= 0)
		{
			// spawn ENEMY
			m_NextEnemyTime = m_EnemyTime;
		}
	}
	
	void Grid::ProcessClicks(int x, int y)
	{
		y -= static_cast<int>(m_StartY - m_TileSize/2);
		if (y >= 0)
		{
			y /= static_cast<int>(m_TileSize);
			x /= static_cast<int>(m_TileSize);
			if (x >= 0 && x < m_NumCols && y >= 0 && y < m_NumRows)
				SelectTile(x, y);
		}
	}

	void Grid::SelectTile(int x, int y)
	{
		Tile::TileState tState = m_Tiles[y][x]->GetTileState();
		if (tState != Tile::EStart && tState != Tile::EBase)
		{
			if (m_SelectedTile)
				m_SelectedTile->ToggleSelected();

			m_SelectedTile = m_Tiles[y][x];
			m_SelectedTile->ToggleSelected();
		}
	}
	
	bool Grid::FindPath(Tile* start, Tile* end)
	{
		for (int i = 0; i < m_NumRows; i++)
		{
			for (int j = 0; j < m_NumCols; j++)
			{
				m_Tiles[i][j]->g = 0.0f;
				m_Tiles[i][j]->m_InClosedSet = false;
				m_Tiles[i][j]->m_InOpenSet = false;
			}
		}

		std::vector<Tile*> openSet;

		Tile* current = start;
		current->m_InClosedSet = true;

		do
		{
			for (Tile* neighbor : current->m_Adjacents)
			{
				if (neighbor->m_Blocked)
					continue;

				if (!neighbor->m_InClosedSet)
				{
					if (!neighbor->m_InOpenSet)
					{
						neighbor->m_Parent = current;
						neighbor->h = (end->GetPosition() - neighbor->GetPosition()).Length();
						neighbor->g = current->g + m_TileSize;
						neighbor->f = neighbor->g + neighbor->h;
						openSet.emplace_back(neighbor);
						neighbor->m_InOpenSet = true;
					}
					else
					{
						float newG = current->g + m_TileSize;
						if (newG < neighbor->g)
						{
							neighbor->m_Parent = current;
							neighbor->g = newG;
							neighbor->f = neighbor->g + neighbor->h;
						}
					}
				}
			}

			if (openSet.empty())
				break;

			auto iter = std::min_element(openSet.begin(), openSet.end(), 
										[](Tile* a, Tile* b) {
											return a->f < b->f;
										});
			current = *iter;
			openSet.erase(iter);
			current->m_InOpenSet = false;
			current->m_InClosedSet = true;

		} while (current != end);

		return (current == end);
	}

	void Grid::UpdatePathTiles()
	{
		for (int i = 0; i < m_NumRows; i++)
		{
			for (int j = 0; j < m_NumCols; j++)
			{
				if (!(i == 3 && j == 0) && !(i == 3 && j == 19))
					m_Tiles[i][j]->SetTileState(Tile::EDefault);
			}
		}

		Tile* t = GetStartTile()->m_Parent;
		while (t != GetEndTile())
		{
			t->SetTileState(Tile::EPath);
			t = t->m_Parent;
		}
	}
	
	void Grid::BuildTower()
	{
		if (m_SelectedTile && !m_SelectedTile->m_Blocked)
		{
			m_SelectedTile->m_Blocked = true;
			if (FindPath(GetEndTile(), GetStartTile()))
			{
				// Create new tower
			}
			else
			{
				m_SelectedTile->m_Blocked = false;
				FindPath(GetEndTile(), GetStartTile());
			}

			UpdatePathTiles();
		}
	}
	
	Tile* Grid::GetStartTile()
	{
		return m_Tiles[3][0];
	}
	
	Tile* Grid::GetEndTile()
	{
		return m_Tiles[3][19];
	}
}