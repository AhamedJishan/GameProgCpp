#pragma once

#include "CameraComponent.h"
#include <vector>
#include <glm/vec3.hpp>

namespace jLab
{
	// -------------------------Spline struct--------------------------------
	struct Spline
	{
		std::vector<glm::vec3> m_ControlPoints;
		glm::vec3 Compute(unsigned int startIndex, float t) const;
		unsigned int GetNumPoints() const { return m_ControlPoints.size(); }
	};
	// ----------------------------------------------------------------------

	class SplineCamera : public CameraComponent
	{
	public:
		SplineCamera(class Actor* owner, const Spline& path);

		void Update(float deltaTime) override;
		
		void Restart();

		void SetSpeed(const float speed) { m_Speed = speed; }
		void SetPath(const Spline& path) { m_Path = path; }

	private:
		Spline m_Path;
		unsigned int m_Index;
		float m_T;
		float m_Speed;
		bool m_Paused;
	};
}