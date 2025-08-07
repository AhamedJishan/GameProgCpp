#pragma once

#include "Engine/Actor.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Component/SpriteRenderer.h"

namespace jLab
{
	class RadarActor : public Actor
	{
	public:
		RadarActor(Game* game)
			:Actor(game)
		{
			SpriteRenderer* sr = new SpriteRenderer(this);
			sr->SetTexture(m_Game->GetRenderer()->GetTexture("Assets/Textures/Radar.png"));
		}
	};
}