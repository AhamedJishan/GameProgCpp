#pragma once

#include <glm/vec3.hpp>

namespace jLab
{
	struct LineSegment
	{
		LineSegment(glm::vec3 start, glm::vec3 end);

		glm::vec3 PointOnSegment(float t);
		float MinDist(glm::vec3 point);

		glm::vec3 m_Start;
		glm::vec3 m_End;
	};
}