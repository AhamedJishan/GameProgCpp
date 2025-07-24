#include "BallActor.h"

#include "Engine/Component/BallMoveComponent.h"
#include "Engine/Component/MeshRenderer.h"
#include "Engine/Model.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"

namespace jLab
{
	BallActor::BallActor(Game* game)
		:Actor(game)
		,m_LifeSpan(3.0f)
	{
		m_MeshRenderer = new MeshRenderer(this);
		m_MeshRenderer->SetMesh(GetGame()->GetRenderer()->GetModel("Assets/Models/ball/ball.obj"));
		m_MeshRenderer->SetSpecularColor(glm::vec3(1.0f), 64.0f);

		m_BallMove = new BallMoveComponent(this);
		m_BallMove->SetVelocity(glm::vec3(0, 0, 10.0f));
	}
	
	void BallActor::ActorUpdate(float deltaTime)
	{
		m_LifeSpan -= deltaTime;
		if (m_LifeSpan <= 0.0f)
			SetState(E_Dead);
	}
	
	void BallActor::SetPlayer(Actor* player)
	{
		m_BallMove->SetPlayer(player);
	}
}