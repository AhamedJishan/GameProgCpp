#include "CameraComponent.h"

#include "Engine/Actor.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/AudioSystem.h"

namespace jLab
{
	CameraComponent::CameraComponent(Actor* owner, int updateOrder)
		:Component(owner, updateOrder)
	{
	}

	void CameraComponent::SetViewMatrix(const glm::mat4& view)
	{
		mOwner->GetGame()->GetRenderer()->SetSetViewMatrix(view);
		mOwner->GetGame()->GetAudioSystem()->SetListener(view);
	}
}