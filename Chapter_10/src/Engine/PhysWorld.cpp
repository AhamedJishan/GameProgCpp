#include "PhysWorld.h"

#include "Component/BoxComponent.h"
#include <algorithm>

namespace jLab
{
	PhysWorld::PhysWorld(Game* game)
		:m_Game(game)
	{
	}
	
	void PhysWorld::AddBox(BoxComponent* box)
	{
		m_Boxes.emplace_back(box);
	}
	
	void PhysWorld::RemoveBox(BoxComponent* box)
	{
		auto iter = std::find(m_Boxes.begin(), m_Boxes.end(), box);
		if (iter != m_Boxes.end())
		{
			std::iter_swap(iter, m_Boxes.end() - 1);
			m_Boxes.pop_back();
		}
	}
	
	bool PhysWorld::SegmentCast(const LineSegment& line, CollisionInfo& outInfo)
	{
		bool collided = false;
		float closestT = std::numeric_limits<float>::infinity();
		glm::vec3 normal;

		for (BoxComponent* box : m_Boxes)
		{
			float t;
			if (Intersects(line, box->GetWorldBox(), t, normal))
			{
				if (t < closestT)
				{
					closestT = t;
					outInfo.m_Point = line.PointOnSegment(t);
					outInfo.m_Normal = normal;
					outInfo.m_Box = box;
					outInfo.m_Actor = box->GetOwner();
					collided = true;
				}
			}
		}

		return collided;
	}
}