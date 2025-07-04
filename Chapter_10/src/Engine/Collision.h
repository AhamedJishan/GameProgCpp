#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

namespace jLab
{
	struct LineSegment
	{
		LineSegment(const glm::vec3& start, const glm::vec3& end);

		glm::vec3 PointOnSegment(float t);
		float MinDist(const glm::vec3& point);

	public:
		glm::vec3 m_Start;
		glm::vec3 m_End;
	};


	struct Plane
	{
		Plane(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
		Plane(const glm::vec3& normal, float signedDistance);

		float SignedDist(const glm::vec3& point);

	public:
		glm::vec3 m_Normal;
		// signed minimal distance between the plane and the origin
		float m_D;
	};


	struct Sphere
	{
		Sphere(const glm::vec3& center, float radius);

		bool Contains(const glm::vec3& point);

	public:
		glm::vec3 m_Center;
		float m_Radius;
	};


	struct AABB
	{
		AABB(const glm::vec3& min, const glm::vec3& max);

		void UpdateMinMax(const glm::vec3& point);
		void Rotate(const glm::quat& rotation);
		bool Contains(const glm::vec3& point);

	public:
		glm::vec3 m_Min;
		glm::vec3 m_Max;
	private:
		const glm::vec3 m_OriginalMin;
		const glm::vec3 m_OriginalMax;
	};


	struct OBB
	{
		glm::vec3 m_Center;
		glm::quat m_Rotation;
		glm::vec3 m_Extents;
	};


	struct Capsule
	{
		Capsule(const glm::vec3& start, const glm::vec3& end, float radius);

		bool Contains(const glm::vec3& point);

	public:
		LineSegment m_Segment;
		float m_Radius;
	};


	struct ConvexPolygon
	{
		bool Contains(const glm::vec2& point);

	public:
		std::vector<glm::vec2> m_Vertices;
	};
}