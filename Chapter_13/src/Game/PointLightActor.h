#pragma once

#include <glm/vec3.hpp>
#include "Engine/Actor.h"
#include "Engine/Game.h"
#include "Engine/Component/PointLightComponent.h"

namespace jLab
{
	class PointLightActor : public Actor
	{
	public:
		PointLightActor(Game* game)
			:Actor(game)
		{
			mLightComponent = new PointLightComponent(this);
			mLightComponent->SetInnerRadius(0.9f);
			mLightComponent->SetOuterRadius(1.2f);
		}

		void SetColor(glm::vec3 color)
		{
			mLightComponent->SetDiffuseColor(color);
		}

	private:
		PointLightComponent* mLightComponent;
	};
}