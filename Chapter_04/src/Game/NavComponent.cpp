#include "NavComponent.h"

#include "Tile.h"

namespace jLab
{
	NavComponent::NavComponent(Actor* owner, int updateOrder)
		:MoveComponent(owner, updateOrder),
		m_NextTile(nullptr)
	{
	}
	
	void NavComponent::Update(float deltaTime)
	{
		Vector2 diff = m_Owner->GetPosition() - m_NextTile->GetPosition();
		if (diff.Length() <= 4.0f)
		{
			m_NextTile = m_NextTile->GetParent();
			TurnTo(m_NextTile->GetPosition());
		}
	}
	
	void NavComponent::StartPath(const Tile* start)
	{
		m_NextTile = start->GetParent();
		TurnTo(m_NextTile->GetPosition());
	}
	
	void NavComponent::TurnTo(const Vector2& pos)
	{
		Vector2 dir = pos - m_Owner->GetPosition();
		float angle = Math::Atan2(-dir.y, dir.x);
		m_Owner->SetRotation(angle);
	}
}