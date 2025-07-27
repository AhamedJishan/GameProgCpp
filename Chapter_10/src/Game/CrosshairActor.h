#pragma once

#include "Engine/Actor.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Component/SpriteRenderer.h"

namespace jLab
{
	class CrosshairActor : public Actor
	{
	public:
		CrosshairActor(Game* game)
			:Actor(game)
		{
			SpriteRenderer* sr = new SpriteRenderer(this);
			sr->SetTexture(m_Game->GetRenderer()->GetTexture("Assets/Textures/Crosshair.png"));
		}
	};
}