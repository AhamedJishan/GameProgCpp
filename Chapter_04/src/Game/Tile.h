#pragma once

#include <vector>
#include "Actor.h"

namespace jLab
{
	class Tile : public Actor
	{
	public:
		friend class Grid;
		enum TileState
		{
			EDefault,
			EStart,
			EPath,
			EBase
		};

		Tile(class Game* game);

		void SetTileState(TileState state);
		TileState GetTileState() { return m_TileState; }

		void ToggleSelected();
		const Tile* GetParent() { return m_Parent; }

	private:
		std::vector<Tile*> m_Adjacents;
		Tile* m_Parent;
		float f;
		float g;
		float h;
		bool m_InOpenSet;
		bool m_InClosedSet;
		bool m_Blocked;

		void UpdateTexture();
		class SpriteComponent* m_Sprite;
		TileState m_TileState;
		bool m_Selected;
	};
}