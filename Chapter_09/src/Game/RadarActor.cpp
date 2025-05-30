#include "RadarActor.h"

#include "Engine/Components/SpriteComponent.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Texture.h"

namespace jLab
{
	RadarActor::RadarActor(Game* game)
		:Actor(game)
	{
		SpriteComponent* sc = new SpriteComponent(this);
		sc->SetTexture(GetGame()->GetRenderer()->GetTexture("Assets/Textures/Radar.png"));
	}
}