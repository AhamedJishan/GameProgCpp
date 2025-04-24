#include "HealthBarActor.h"

#include "Engine/Component/SpriteComponent.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Texture.h"

namespace jLab
{
	HealthBarActor::HealthBarActor(Game* game)
		:Actor(game)
	{
		SpriteComponent* sc = new SpriteComponent(this);
		sc->SetTexture(m_Game->GetRenderer()->GetTexture("Assets/Textures/HealthBar.png", Texture::DiffuseTexture));
	}

	HealthBarActor::~HealthBarActor()
	{
	}

	void HealthBarActor::UpdateActor(float deltaTime)
	{
	}
}