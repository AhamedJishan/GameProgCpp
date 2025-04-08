#include "Tile.h"

#include "SpriteComponent.h"
#include "Game.h"
#include <string>

namespace jLab
{
	Tile::Tile(Game* game)
		:Actor(game),
		m_Parent(nullptr),
		f(0),
		g(0),
		h(0),
		m_Blocked(false),
		m_Sprite(nullptr),
		m_TileState(TileState::EDefault),
		m_Selected(false)
	{
		m_Sprite = new SpriteComponent(this);
		UpdateTexture();
	}
	
	void Tile::SetTileState(TileState state)
	{
		m_TileState = state;
		UpdateTexture();
	}
	
	void Tile::ToggleSelected()
	{
		m_Selected = !m_Selected;
		UpdateTexture();
	}
	
	void Tile::UpdateTexture()
	{
		std::string text;
		switch (m_TileState)
		{
			case TileState::EStart:
				text = "Assets/TileTan.png";
				break;

			case TileState::EBase:
				text = "Assets/TileGreen.png";
				break;

			case TileState::EPath:
				if (m_Selected) text = "Assets/TileGreySelected.png";
				else			text = "Assets/TileGrey.png";
				break;

			case TileState::EDefault:
			default:
				if (m_Selected) text = "Assets/TileBrownSelected.png";
				else			text = "Assets/TileBrown.png";
		}
		m_Sprite->SetTexture(GetGame()->GetTexture(text));
	}
}