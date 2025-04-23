#include "TestActor2.h"

#include "Engine/Component/SpriteComponent.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Texture.h"

namespace jLab
{
	TestActor2::TestActor2(Game* game)
		:Actor(game)
	{
		SpriteComponent* sc = new SpriteComponent(this);
		sc->SetTexture(m_Game->GetRenderer()->GetTexture("Assets/Textures/Radar.png", Texture::DiffuseTexture));
	}
	
	TestActor2::~TestActor2()
	{
	}

	void TestActor2::UpdateActor(float deltaTime)
	{
	}
}