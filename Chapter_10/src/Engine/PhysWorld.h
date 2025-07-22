#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include "Collision.h"

namespace jLab
{
	class PhysWorld
	{
	public:
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

		PhysWorld(class Game* game);

		void AddBox(class BoxComponent* box);
		void RemoveBox(class BoxComponent* box);

		bool SegmentCast(const LineSegment& line, CollisionInfo& outInfo);

	private:
		class Game* m_Game;
		std::vector<class BoxComponent*> m_Boxes;
	};
}