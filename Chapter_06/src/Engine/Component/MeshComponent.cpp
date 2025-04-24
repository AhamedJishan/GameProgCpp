#include "MeshComponent.h"

#include <glm/gtc/type_ptr.hpp>
#include "Engine/Shader.h"
#include "Engine/Actor.h"
#include "Engine/Model.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"

namespace jLab
{
	MeshComponent::MeshComponent(Actor* owner, int updateOrder)
		:Component(owner, updateOrder),
		m_Model(nullptr),
		m_SpecularColor(glm::vec3(0)),
		m_SpecularPower(0)
	{
		// TODO: Add this Mesh Component to renderer
		m_Owner->GetGame()->GetRenderer()->AddMeshComponent(this);
	}
	
	MeshComponent::~MeshComponent()
	{
		// TODO: Remove this Mesh Component to renderer
		m_Owner->GetGame()->GetRenderer()->RemoveMeshComponent(this);
	}
	
	void MeshComponent::Draw(const Shader* shader)
	{
		if (m_Model)
		{
			shader->SetMat4("uWorldTransform", m_Owner->GetWorldTransform());
			shader->SetVec3("uSpecularColor", m_SpecularColor);
			shader->SetFloat("uSpecularPower", m_SpecularPower);
			m_Model->Draw(shader);
		}
	}
}