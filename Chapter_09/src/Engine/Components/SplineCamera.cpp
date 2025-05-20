#include "SplineCamera.h"

namespace jLab
{
	// -------------------------Spline struct--------------------------------
	glm::vec3 Spline::Compute(unsigned int startIndex, float t) const
	{
		if (startIndex >= m_ControlPoints.size())			return m_ControlPoints.back();
		else if (startIndex == 0)							return m_ControlPoints[startIndex];
		else if (startIndex + 2 >= m_ControlPoints.size())  return m_ControlPoints[startIndex];

		glm::vec3 p0 = m_ControlPoints[startIndex - 1];
		glm::vec3 p1 = m_ControlPoints[startIndex];
		glm::vec3 p2 = m_ControlPoints[startIndex + 1];
		glm::vec3 p3 = m_ControlPoints[startIndex + 2];

		glm::vec3 position = 0.5f * (2.0f * p1 + (-p0 + p2) * t
							+ (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t * t
							+ (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t * t * t);

		return position;
	}
	// ----------------------------------------------------------------------
	
	SplineCamera::SplineCamera(Actor* owner, const Spline& path)
		:CameraComponent(owner)
	{
		m_Path = path;
		m_Index = 1;
		m_T = 0.0f;
		m_Speed = 0.25f;
		m_Paused = false;
	}
	
	void SplineCamera::Update(float deltaTime)
	{
		if (!m_Paused)
		{
			m_T += m_Speed * deltaTime;

			if (m_T >= 1.0f)
			{
				if (m_Index + 3 < m_Path.GetNumPoints())
				{
					m_Index++;
					m_T -= 1.0f;
				}
				else
					m_Paused = true;
			}
		}

		glm::vec3 position = m_Path.Compute(m_Index, m_T);
		glm::vec3 target = m_Path.Compute(m_Index, m_T + 0.1f);
		glm::vec3 up = glm::vec3(0, 1, 0);

		glm::mat4 view = glm::lookAt(position, target, up);

		SetViewMatrix(view);
	}

	void SplineCamera::Restart()
	{
		m_Index = 1;
		m_T = 0.0f;
		m_Speed = 0.25f;
		m_Paused = false;
	}
}