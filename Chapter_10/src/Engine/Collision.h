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


	struct Plane
	{
		Plane(glm::vec3 a, glm::vec3 b, glm::vec3 c);
		Plane(glm::vec3 normal, float signedDistance);

		float SignedDist(const glm::vec3& point);

		glm::vec3 m_Normal;
		// signed minimal distance between the plane and the origin
		float m_D;
	};
}