#include "PointLightComponent.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "Engine/Actor.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Shader.h"
#include "Engine/Model.h"

namespace jLab
{
	PointLightComponent::PointLightComponent(Actor* owner)
		:Component(owner)
		,mDiffuseColor(glm::vec3(1))
		,mInnerRadius(0.5f)
		,mOuterRadius(1)
	{
		mOwner->GetGame()->GetRenderer()->AddLight(this);
	}

	PointLightComponent::~PointLightComponent()
	{
		mOwner->GetGame()->GetRenderer()->RemoveLight(this);
	}
	
	void PointLightComponent::Draw(const Shader* shader)
	{
		glm::mat4 worldTransform =
			glm::translate(glm::mat4(1.0f), mOwner->GetPosition()) *
			glm::scale(glm::mat4(1.0f), mOwner->GetScale() * mOuterRadius);

		shader->SetMat4("uWorldTransform", worldTransform);
		shader->SetVec3("uPointLight.mWorldPos", mOwner->GetPosition());
		shader->SetVec3("uPointLight.mDiffuseColor", mDiffuseColor);
		shader->SetFloat("uPointLight.mInnerRadius", mInnerRadius);
		shader->SetFloat("uPointLight.mOuterRadius", mOuterRadius);

		Model* lightModel = mOwner->GetGame()->GetRenderer()->GetLightModel();
		lightModel->Draw(shader);
	}
}