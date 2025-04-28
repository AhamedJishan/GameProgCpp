#include "HealthBarActor.h"

#include "Engine/Component/SpriteComponent.h"
#include "Engine/Model.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"

namespace jLab
{
	HealthBarActor::HealthBarActor(Game* game)
		:Actor(game)
	{
		SpriteComponent* sc = new SpriteComponent(this, 100);
		sc->SetTexture(m_Game->GetRenderer()->GetTexture("Assets/Textures/HealthBar.png", Texture::E_DIFFUSE));
	}

	HealthBarActor::~HealthBarActor()
	{
	}

	void HealthBarActor::UpdateActor(float deltaTime)
	{
	}
}