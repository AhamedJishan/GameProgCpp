#include "MeshRenderer.h"

#include "Engine/Actor.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Shader.h"
#include "Engine/Model.h"

namespace jLab
{
	MeshRenderer::MeshRenderer(Actor* owner, int drawOrder)
		: Component(owner, drawOrder)
		, m_Model(nullptr)
		, m_IsVisible(true)
		, m_SpecularColor(glm::vec3(1))
		, m_SpecularPower(32)
	{
		m_Owner->GetGame()->GetRenderer()->AddMeshRenderer(this);
	}
	
	MeshRenderer::~MeshRenderer()
	{
		m_Owner->GetGame()->GetRenderer()->RemoveMeshRenderer(this);
	}
	
	void MeshRenderer::Draw(const Shader* shader)
	{
		if (m_Model && m_IsVisible)
		{
			shader->SetMat4("u_WorldTransform", m_Owner->GetWorldTransform());
			shader->SetVec3("u_SpecularColor", m_SpecularColor);
			shader->SetFloat("u_SpecularPower", m_SpecularPower);
			m_Model->Draw(shader);
		}
	}
}