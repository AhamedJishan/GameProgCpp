#include "Collision.h"

#include <array>
#include <algorithm>

namespace jLab
{
	LineSegment::LineSegment(const glm::vec3& start, const glm::vec3& end)
		: m_Start(start)
		, m_End(end)
	{
	}

	glm::vec3 LineSegment::PointOnSegment(float t)
	{
		return m_Start + (m_End - m_Start) * t;
	}

	float LineSegment::MinDist(const glm::vec3& point)
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


	Plane::Plane(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
	{
		glm::vec3 ab = b - a;
		glm::vec3 ac = c - a;

		m_Normal = glm::normalize(glm::cross(ab, ac));
		m_D = glm::dot(a, m_Normal);
	}

	Plane::Plane(const glm::vec3& normal, float signedDistance)
		: m_Normal(normal)
		, m_D(signedDistance)
	{
	}

	float Plane::SignedDist(const glm::vec3& point)
	{
		return glm::dot(point, m_Normal) - m_D;
	}


	Sphere::Sphere(const glm::vec3& center, float radius)
		: m_Center(center)
		, m_Radius(radius)
	{
	}

	bool Sphere::Contains(const glm::vec3& point)
	{
		float distance = glm::length(point - m_Center);
		return (distance <= m_Radius);
	}


	AABB::AABB(const glm::vec3& min, const glm::vec3& max)
		: m_Min(min), m_OriginalMin(min)
		, m_Max(max), m_OriginalMax(max)
	{
	}

	void AABB::UpdateMinMax(const glm::vec3& point)
	{
		m_Min.x = std::min(m_Min.x, point.x);
		m_Min.y = std::min(m_Min.y, point.y);
		m_Min.z = std::min(m_Min.z, point.z);
		m_Max.x = std::min(m_Max.x, point.x);
		m_Max.y = std::min(m_Max.y, point.y);
		m_Max.z = std::min(m_Max.z, point.z);
	}

	void AABB::Rotate(const glm::quat& rotation)
	{
		glm::mat3 rotationMat = glm::mat3_cast(rotation);
		std::array<glm::vec3, 8> points;

		// Min point is always the corner
		points[0] = m_OriginalMin;
		// Permutation with 2 min and 1 max
		points[1] = glm::vec3(m_OriginalMax.x, m_OriginalMin.y, m_OriginalMin.z);
		points[2] = glm::vec3(m_OriginalMin.x, m_OriginalMax.y, m_OriginalMin.z);
		points[3] = glm::vec3(m_OriginalMin.x, m_OriginalMin.y, m_OriginalMax.z);
		// Permutation with 1 min and 2 max
		points[4] = glm::vec3(m_OriginalMin.x, m_OriginalMax.y, m_OriginalMax.z);
		points[5] = glm::vec3(m_OriginalMax.x, m_OriginalMin.y, m_OriginalMax.z);
		points[6] = glm::vec3(m_OriginalMax.x, m_OriginalMax.y, m_OriginalMin.z);
		// Max point is also a corner
		points[7] = m_OriginalMax;

		glm::vec3 p = rotationMat * m_OriginalMin;

		m_Min = p;
		m_Max = p;

		for (int i = 1; i < 8; i++)
		{
			p = rotationMat * points[i];
			UpdateMinMax(p);
		}
	}

	bool AABB::Contains(const glm::vec3& point)
	{
		bool outside =
			point.x < m_Min.x ||
			point.y < m_Min.y ||
			point.z < m_Min.z ||
			point.x < m_Max.x ||
			point.y < m_Max.y ||
			point.z < m_Max.z;

		return !outside;
	}

	float AABB::MinDist(const glm::vec3& point) const
	{
		float dx = std::max(m_Min.x - point.x, 0.0f);
		dx = std::max(dx, point.x - m_Max.x);
		float dy = std::max(m_Min.y - point.y, 0.0f);
		dy = std::max(dy, point.y - m_Max.y);
		float dz = std::max(m_Min.z - point.z, 0.0f);
		dz = std::max(dz, point.z - m_Max.z);

		float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
		return distance;
	}

	Capsule::Capsule(const glm::vec3& start, const glm::vec3& end, float radius)
		:m_Segment(LineSegment(start, end))
		,m_Radius(radius)
	{
	}

	bool Capsule::Contains(const glm::vec3& point)
	{
		float distance = m_Segment.MinDist(point);
		return distance < m_Radius;
	}

	bool ConvexPolygon::Contains(const glm::vec2& point)
	{
		if (m_Vertices.size() < 3) return false;

		float sum = 0.0f;

		for (int i = 0; i < m_Vertices.size(); i++)
		{
			glm::vec2 a = glm::normalize(m_Vertices[i] - point);
			glm::vec2 b = glm::normalize(m_Vertices[(i + 1) % m_Vertices.size()] - point);

			sum += glm::acos(glm::dot(a, b));
		}

		return glm::epsilonEqual(sum, glm::two_pi<float>(), 0.1f);
	}



	bool Intersects(const Sphere& a, const Sphere& b)
	{
		float distance = glm::length(a.m_Center - b.m_Center);
		float radiiSum = a.m_Radius + b.m_Radius;
		return distance <= radiiSum;
	}

	bool Intersects(const AABB& a, const AABB& b)
	{
		bool notIntersecting =
			a.m_Max.x < b.m_Min.x ||
			a.m_Max.y < b.m_Min.y ||
			a.m_Max.z < b.m_Min.z ||
			a.m_Min.x > b.m_Max.x ||
			a.m_Min.y > b.m_Max.y ||
			a.m_Min.z > b.m_Max.z;

		return !notIntersecting;
	}

	bool Intersects(const Sphere& sphere, const AABB& box)
	{
		float distance = box.MinDist(sphere.m_Center);
		return (distance <= sphere.m_Radius);
	}

}