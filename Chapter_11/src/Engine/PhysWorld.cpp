#include "PhysWorld.h"

#include "Component/BoxComponent.h"
#include "Actor.h"
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
	
	bool PhysWorld::SegmentCast(const LineSegment& line, CollisionInfo& outInfo, Actor* ignoreActor)
	{
		bool collided = false;
		float closestT = std::numeric_limits<float>::infinity();
		glm::vec3 normal;

 		for (BoxComponent* box : m_Boxes)
		{
			float t;
			if (Intersects(line, box->GetWorldBox(), t, normal))
			{
				if (t < closestT && (box->GetOwner() != ignoreActor || ignoreActor == nullptr))
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

	void PhysWorld::TestPairWise(std::function<void(Actor*, Actor*)> func)
	{
		for (int i = 0; i < m_Boxes.size(); i++)
		{
			for (int j = i + 1; j < m_Boxes.size(); j++)
			{
				BoxComponent* a = m_Boxes[i];
				BoxComponent* b = m_Boxes[j];
				if (Intersects(a->GetWorldBox(), b->GetWorldBox()))
					func(a->GetOwner(), b->GetOwner());
			}
		}
	}

	void PhysWorld::TestSweepAndPrune(std::function<void(Actor*, Actor*)> func)
	{
		std::sort(m_Boxes.begin(), m_Boxes.end(),
			[](BoxComponent* a, BoxComponent* b)
			{
				return a->GetWorldBox().m_Min.z < b->GetWorldBox().m_Max.z;
			});

		for (int i = 0; i < m_Boxes.size(); i++)
		{
			BoxComponent* a = m_Boxes[i];
			float max = a->GetWorldBox().m_Max.z;

			for (int j = i + 1; j < m_Boxes.size(); j++)
			{
				BoxComponent* b = m_Boxes[j];
				float min = b->GetWorldBox().m_Min.z;

				if (min > max) break;

				if (Intersects(a->GetWorldBox(), b->GetWorldBox()))
					func(a->GetOwner(), b->GetOwner());
			}
		}
	}
}