#pragma once

#include "Engine/Actor.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Component/SpriteComponent.h"
#include "Engine/Texture.h"

#include <glm/vec4.hpp>
#include "Engine/Font.h"

namespace jLab
{
	class SpriteTestActor : public Actor
	{
	public:
		SpriteTestActor(class Game* game)
			:Actor(game)
		{
			SpriteComponent* sc = new SpriteComponent(this);
			//Texture* texture = mGame->GetRenderer()->GetTexture("Assets/Textures/Crosshair.png");

			Font* font = new Font(mGame);
			font->Load("Assets/Fonts/Carlito-Regular.ttf");
			Texture* texture = font->RenderText("PauseTitle");
			sc->SetTexture(texture);
		}
	};
}