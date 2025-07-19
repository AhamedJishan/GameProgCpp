#include "CameraComponent.h"

#include "Engine/Actor.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"

namespace jLab
{
	CameraComponent::CameraComponent(Actor* owner, int updateOrder)
		:Component(owner, updateOrder)
	{
	}
	
	void CameraComponent::SetViewMatrix(const glm::mat4 view)
	{
		m_Owner->GetGame()->GetRenderer()->SetViewMatrix(view);
		// TODO: Set view matrix for audioSystem
	}
}