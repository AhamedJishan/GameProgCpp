#pragma once

#include "Engine/Actor.h"

#include "Engine/Components/SplineCamera.h"
#include "Engine/InputSystem.h"

namespace jLab
{
	class SplineActor : public Actor
	{
	public:
		SplineActor(class Game* game)
			:Actor(game)
		{
			Spline path;
			path.m_ControlPoints.emplace_back(glm::vec3(0));
			for (int i = 0; i < 5; i++)
			{
				if (i % 2 == 0)
				{
					path.m_ControlPoints.emplace_back(glm::vec3(3.0f * (i + 1), 3.0f, 3.0f));
				}
				else
				{
					path.m_ControlPoints.emplace_back(glm::vec3(3.0f * (i + 1), 0.0f, 0.0f));
				}
			}

			m_Camera = new SplineCamera(this, path);
		}

		void Input(struct InputState& inputState)
		{
			if (inputState.Keyboard.GetKey(SDL_SCANCODE_SPACE))
				m_Camera->Restart();
		}
		
	private:
		SplineCamera* m_Camera;
	};
}