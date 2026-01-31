#include "MeshComponent.h"

#include "Engine/Actor.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Shader.h"
#include "Engine/Model.h"
#include "Engine/LevelLoader.h"

namespace jLab
{
	MeshComponent::MeshComponent(Actor* owner, bool isSkinned, int drawOrder)
		: Component(owner, drawOrder)
		, mModel(nullptr)
		, mIsVisible(true)
		, mSpecularColor(glm::vec3(1))
		, mSpecularPower(32)
		, mIsSkinned(isSkinned)
	{
		mOwner->GetGame()->GetRenderer()->AddMeshComponent(this);
	}

	MeshComponent::~MeshComponent()
	{
		mOwner->GetGame()->GetRenderer()->RemoveMeshComponent(this);
	}

	void MeshComponent::Draw(const Shader* shader)
	{
		if (mModel && mIsVisible)
		{
			shader->SetMat4("uWorldTransform", mOwner->GetWorldTransformMatrix());
			shader->SetVec3("uSpecularColor", mSpecularColor);
			shader->SetFloat("uSpecularPower", mSpecularPower);
			mModel->Draw(shader);
		}
	}

	void MeshComponent::LoadProperties(const rapidjson::Value& inObj)
	{
		Component::LoadProperties(inObj);

		std::string meshFile;
		if (LevelLoader::GetString(inObj, "meshFile", meshFile))
			mOwner->GetGame()->GetRenderer()->GetModel(meshFile);

		LevelLoader::GetVec3(inObj, "specularColor", mSpecularColor);
		LevelLoader::GetFloat(inObj, "specularPower", mSpecularPower);
		LevelLoader::GetBool(inObj, "isSkinned", mIsSkinned);
		LevelLoader::GetBool(inObj, "isVisible", mIsVisible);
	}
}