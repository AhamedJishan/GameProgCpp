#include "Collision.h"

#include <glm/glm.hpp>

namespace jLab
{
	//------------------Line Segment-----------------------
	LineSegment::LineSegment(glm::vec3 start, glm::vec3 end)
		: m_Start(start)
		, m_End(end)
	{
	}

	glm::vec3 LineSegment::PointOnSegment(float t)
	{
		return m_Start + (m_End - m_Start) * t;
	}

	float LineSegment::MinDist(glm::vec3 point)
	{
		glm::vec3 ab = m_End - m_Start;
		glm::vec3 abNormalized = glm::normalize(ab);
		glm::vec3 ba = -ab;
		glm::vec3 ac = point - m_Start;
		glm::vec3 bc = point - m_End;

		if (glm::dot(ab, ac) < 0.0f)
			return glm::length(ac);

		if (glm::dot(ba, bc) < 0.0f)
			return glm::length(bc);

		float scalarP = glm::dot(ac, abNormalized);
		glm::vec3 vecP = scalarP * abNormalized;

		float distance = glm::length(ac - vecP);
		return distance;
	}
	//------------------------------------------------------


	//------------------------Plane----------------------------
	Plane::Plane(glm::vec3 a, glm::vec3 b, glm::vec3 c)
	{
		glm::vec3 ab = b - a;
		glm::vec3 ac = c - a;

		m_Normal = glm::normalize(glm::cross(ab, ac));
		m_D = glm::dot(a, m_Normal);
	}

	Plane::Plane(glm::vec3 normal, float signedDistance)
		: m_Normal(normal)
		, m_D(signedDistance)
	{
	}

	float Plane::SignedDist(const glm::vec3& point)
	{
		return glm::dot(point, m_Normal) - m_D;
	}
	//---------------------------------------------------------

}