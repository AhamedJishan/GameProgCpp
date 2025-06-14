#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace jLab
{
	struct LineSegment
	{
		LineSegment(const glm::vec3& start, const glm::vec3& end);

		glm::vec3 PointOnSegment(float t);
		float MinDist(const glm::vec3& point);

		glm::vec3 m_Start;
		glm::vec3 m_End;
	};


	struct Plane
	{
		Plane(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
		Plane(const glm::vec3& normal, float signedDistance);

		float SignedDist(const glm::vec3& point);

		glm::vec3 m_Normal;
		// signed minimal distance between the plane and the origin
		float m_D;
	};

	struct Sphere
	{
		Sphere(const glm::vec3& center, float radius);

		glm::vec3 m_Center;
		float m_Radius;
	};

	struct AABB
	{
		AABB(const glm::vec3& min, const glm::vec3& max);

		void UpdateMinMax(const glm::vec3& point);
		void Rotate(const glm::quat& rotation);

		glm::vec3 m_Min;
		glm::vec3 m_Max;
	private:
		const glm::vec3 m_OriginalMin;
		const glm::vec3 m_OriginalMax;
	};
}