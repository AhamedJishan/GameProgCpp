#pragma once

#include <glm/vec3.hpp>

namespace jLab
{
	struct LineSegment
	{
		glm::vec3 m_Start;
		glm::vec3 m_End;

		glm::vec3 PointOnSegment(float t);
	};
}