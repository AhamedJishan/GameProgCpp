#include "BallMoveComponent.h"

#include <glm/vec3.hpp>
#include "Engine/Actor.h"
#include "Engine/Game.h"
#include "Engine/Collision.h"
#include "Engine/PhysWorld.h"

namespace jLab
{
	BallMoveComponent::BallMoveComponent(Actor* owner, int updateOrder)
		:MoveComponent(owner, updateOrder)
	{
	}

	void BallMoveComponent::Update(float deltaTime)
	{
		float segmentLength = 1.0f;
		glm::vec3 dir = m_Owner->GetForward();
		glm::vec3 start = m_Owner->GetPosition();
		glm::vec3 end = start + dir * segmentLength;

		LineSegment line(start, end);
		PhysWorld::CollisionInfo info;

		PhysWorld* physWorld = m_Owner->GetGame()->GetPhysWorld();

		if (physWorld->SegmentCast(line, info) && info.m_Actor != m_Player)
		{
			dir = glm::reflect(m_Owner->GetForward(), info.m_Normal);
			m_Owner->LookAt(dir);
		}

		MoveComponent::Update(deltaTime);
	}
}