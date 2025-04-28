#include "RadarActor.h"

#include "Engine/Component/SpriteComponent.h"
#include "Engine/Model.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"

namespace jLab
{
	RadarActor::RadarActor(Game* game)
		:Actor(game)
	{
		SpriteComponent* sc = new SpriteComponent(this, 100);
		sc->SetTexture(m_Game->GetRenderer()->GetTexture("Assets/Textures/Radar.png", Texture::E_DIFFUSE));
	}

	RadarActor::~RadarActor()
	{
	}

	void RadarActor::UpdateActor(float deltaTime)
	{
	}
}