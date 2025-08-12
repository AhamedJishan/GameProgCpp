#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <functional>
#include "Collision.h"

namespace jLab
{
	struct CollisionInfo
	{
		// Point of contact
		glm::vec3 m_Point;
		// Normal at collision
		glm::vec3 m_Normal;
		// Component collided with
		class BoxComponent* m_Box;
		// Owning actor of the component
		class Actor* m_Actor;
	};
	class PhysWorld
	{
	public:

		PhysWorld(class Game* game);

		void AddBox(class BoxComponent* box);
		void RemoveBox(class BoxComponent* box);

		bool SegmentCast(const LineSegment& line, CollisionInfo& outInfo, class Actor* ignoreActor = nullptr);

		void TestPairWise(std::function<void(class Actor*, class Actor*)> func);
		void TestSweepAndPrune(std::function<void(class Actor*, class Actor*)> func);

	private:
		class Game* m_Game;
		std::vector<class BoxComponent*> m_Boxes;
	};
}