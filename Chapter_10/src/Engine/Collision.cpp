#include "Collision.h"

namespace jLab
{
	glm::vec3 LineSegment::PointOnSegment(float t)
	{
		return m_Start + (m_End - m_Start) * t;
	}
}