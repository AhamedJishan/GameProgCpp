#include "BoxComponent.h"

#include "Engine/Actor.h"
#include "Engine/Game.h"
#include "Engine/PhysWorld.h"

namespace jLab
{
	BoxComponent::BoxComponent(Actor* owner, int updateOrder)
		: Component(owner, updateOrder)
		, mObjectBox(glm::vec3(0), glm::vec3(0))
		, mWorldBox(glm::vec3(0), glm::vec3(0))
		, mShouldRotate(true)
	{
		mOwner->GetGame()->GetPhysWorld()->AddBox(this);
	}

	BoxComponent::~BoxComponent()
	{
		mOwner->GetGame()->GetPhysWorld()->RemoveBox(this);
	}
	
	void BoxComponent::OnUpdateWorldTransform()
	{
		// Reset to object box
		mWorldBox = mObjectBox;
		// SCALE
		mWorldBox.m_Min.x *= mOwner->GetScale().x;
		mWorldBox.m_Min.y *= mOwner->GetScale().y;
		mWorldBox.m_Min.z *= mOwner->GetScale().z;
		mWorldBox.m_Max.x *= mOwner->GetScale().x;
		mWorldBox.m_Max.y *= mOwner->GetScale().y;
		mWorldBox.m_Max.z *= mOwner->GetScale().z;

		// ROTATE
		if (mShouldRotate)
			mWorldBox.Rotate(mOwner->GetRotation());

		// TRANSLATE
		mWorldBox.m_Min += mOwner->GetPosition();
		mWorldBox.m_Max += mOwner->GetPosition();
	}
}