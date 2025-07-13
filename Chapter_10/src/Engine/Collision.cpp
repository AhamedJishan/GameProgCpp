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

	glm::vec3 LineSegment::PointOnSegment(float t) const
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

	float LineSegment::MinDist(const LineSegment& s1, const LineSegment& s2)
	{
		glm::vec3   u = s1.m_End - s1.m_Start;
		glm::vec3   v = s2.m_End - s2.m_Start;
		glm::vec3   w = s1.m_Start - s2.m_Start;
		float    a = glm::dot(u, u);         // always >= 0
		float    b = glm::dot(u, v);
		float    c = glm::dot(v, v);         // always >= 0
		float    d = glm::dot(u, w);
		float    e = glm::dot(v, w);
		float    D = a * c - b * b;        // always >= 0
		float    sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
		float    tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0

		// compute the line parameters of the two closest points
		if (glm::epsilonEqual(D, 0.0f, 0.01f)) { // the lines are almost parallel
			sN = 0.0;         // force using point P0 on segment S1
			sD = 1.0;         // to prevent possible division by 0.0 later
			tN = e;
			tD = c;
		}
		else {                 // get the closest points on the infinite lines
			sN = (b * e - c * d);
			tN = (a * e - b * d);
			if (sN < 0.0) {        // sc < 0 => the s=0 edge is visible
				sN = 0.0;
				tN = e;
				tD = c;
			}
			else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
				sN = sD;
				tN = e + b;
				tD = c;
			}
		}

		if (tN < 0.0) {            // tc < 0 => the t=0 edge is visible
			tN = 0.0;
			// recompute sc for this edge
			if (-d < 0.0)
				sN = 0.0;
			else if (-d > a)
				sN = sD;
			else {
				sN = -d;
				sD = a;
			}
		}
		else if (tN > tD) {      // tc > 1  => the t=1 edge is visible
			tN = tD;
			// recompute sc for this edge
			if ((-d + b) < 0.0)
				sN = 0;
			else if ((-d + b) > a)
				sN = sD;
			else {
				sN = (-d + b);
				sD = a;
			}
		}
		// finally do the division to get sc and tc
		sc = (glm::epsilonEqual(sN, 0.0f, 0.01f) ? 0.0f : sN / sD);
		tc = (glm::epsilonEqual(tN, 0.0f, 0.01f) ? 0.0f : tN / tD);

		// get the difference of the two closest points
		glm::vec3   dP = w + (sc * u) - (tc * v);  // =  S1(sc) - S2(tc)

		return glm::length(dP);   // return the closest distance
	}


	Plane::Plane(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
	{
		glm::vec3 ab = b - a;
		glm::vec3 ac = c - a;

		m_Normal = glm::normalize(glm::cross(ab, ac));
		m_D = -glm::dot(a, m_Normal);
	}

	Plane::Plane(const glm::vec3& normal, float signedDistance)
		: m_Normal(normal)
		, m_D(signedDistance)
	{
	}

	float Plane::SignedDist(const glm::vec3& point)
	{
		return glm::dot(point, m_Normal) + m_D;
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

	bool AABB::Contains(const glm::vec3& point) const
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

	bool Intersects(const Capsule& a, const Capsule& b)
	{
		float distance = LineSegment::MinDist(a.m_Segment, b.m_Segment);
		float radiiSum = a.m_Radius + b.m_Radius;
		return distance <= radiiSum;
	}

	bool Intersects(const LineSegment& line, const Plane& plane, float& outT)
	{
		float denominator = glm::dot((line.m_End - line.m_Start), plane.m_Normal);
		// if denominator is 0, then the line is paraller to the plane
		if (glm::epsilonEqual(denominator, 0.0f, 0.01f))
		{
			// check if line is on the plane
			if (glm::epsilonEqual((glm::dot(line.m_Start, plane.m_Normal) + plane.m_D), 0.0f, 0.01f))
			{
				outT = 0.0f;
				return true;
			}
			else
				return false;
		}
		else
		{
			float numerator = - glm::dot(line.m_Start, plane.m_Normal) - plane.m_D;
			outT = numerator / denominator;
			
			if (outT >= 0.0f && outT <= 1.0f)
				return true;
			else
				return false;
		}
	}

	bool Intersects(const LineSegment& line, const Sphere& sphere, float& outT)
	{
		// Compute X, Y, a, b, c as per equations
		glm::vec3 X = line.m_Start - sphere.m_Center;
		glm::vec3 Y = line.m_End - line.m_Start;
		float a = glm::dot(Y, Y);
		float b = 2.0f * glm::dot(X, Y);
		float c = glm::dot(X, X) - sphere.m_Radius * sphere.m_Radius;
		// Compute discriminant
		float discriminant = b * b - 4.0f * a * c;
		if (discriminant < 0.0f)
			return false;
		else
		{
			discriminant = std::sqrt(discriminant);
			// Compute min and max solutions of t
			float tMin = (-b - discriminant) / (2.0f * a);
			float tMax = (-b + discriminant) / (2.0f * a);
			// Check whether either t is within bounds of segment
			if (tMin >= 0.0f && tMin <= 1.0f)
			{
				outT = tMin;
				return true;
			}
			else if (tMax >= 0.0f && tMax <= 1.0f)
			{
				outT = tMax;
				return true;
			}
			else
				return false;
		}
	}

	bool Intersects(const LineSegment& line, const AABB& aabb, float& outT)
	{
		// vector to store all the t values
		std::vector<float> tValues;

		//Test the x plane
		TestSidePlane(line.m_Start.x, line.m_End.x, aabb.m_Min.x, tValues);
		TestSidePlane(line.m_Start.x, line.m_End.x, aabb.m_Max.x, tValues);
		//Test the y plane
		TestSidePlane(line.m_Start.y, line.m_End.y, aabb.m_Min.y, tValues);
		TestSidePlane(line.m_Start.y, line.m_End.y, aabb.m_Max.y, tValues);
		//Test the z plane
		TestSidePlane(line.m_Start.z, line.m_End.z, aabb.m_Min.z, tValues);
		TestSidePlane(line.m_Start.z, line.m_End.z, aabb.m_Max.z, tValues);

		// sort the tValues in ascending order
		std::sort(tValues.begin(), tValues.end());

		// test if the box contains any of these tValues
		glm::vec3 point;
		for (float t : tValues)
		{
			point = line.PointOnSegment(t);
			if (aabb.Contains(point))
			{
				outT = t;
				return true;
			}
		}

		return false;
	}

	bool TestSidePlane(float start, float end, float negD, std::vector<float>& out)
	{
		float denominator = end - start;
		if (glm::epsilonEqual(denominator, 0.0f, 0.01f))
			return false;
		else
		{
			float numerator = -start + negD;
			float t = numerator / denominator;
			// Test t within bounds
			if (t >= 0.0f && t <= 1.0f)
			{
				out.emplace_back(t);
				return true;
			}
			else
				return false;
		}
	}

}