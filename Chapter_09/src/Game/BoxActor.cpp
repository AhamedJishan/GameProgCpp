#include "BoxActor.h"

#include "Engine/Components/AudioComponent.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Model.h"

namespace jLab
{
	BoxActor::BoxActor(Game* game)
		:Actor(game)
	{
		AudioComponent* ac = new AudioComponent(this);
		ac->PlayEvent("event:/FireLoop");

		MeshRenderer* mr = new MeshRenderer(this);
		mr->SetMesh(m_Game->GetRenderer()->GetModel("Assets/Models/container/container.obj"));
	}
}