#pragma once

#include "Engine/Actor.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Component/SpriteComponent.h"
#include "Engine/Texture.h"

namespace jLab
{
	class SpriteTestActor : public Actor
	{
	public:
		SpriteTestActor(class Game* game)
			:Actor(game)
		{
			SpriteComponent* sc = new SpriteComponent(this);
			Texture* texture = mGame->GetRenderer()->GetTexture("Assets/Textures/Crosshair.png");
			sc->SetTexture(texture);
		}
	};
}