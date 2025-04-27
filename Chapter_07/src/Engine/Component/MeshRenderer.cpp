#include "MeshRenderer.h"

#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Actor.h"
#include "Engine/Model.h"
#include "Engine/Shader.h"

namespace jLab
{
	MeshRenderer::MeshRenderer(Actor* owner, int updateOrder)
		:Component(owner, updateOrder),
		m_Model(nullptr),
		m_SpecularColor(glm::vec3(0)),
		m_SpecularPower(0)
	{
		m_Owner->GetGame()->GetRenderer()->AddMeshRenderer(this);
	}
	
	MeshRenderer::~MeshRenderer()
	{
		m_Owner->GetGame()->GetRenderer()->RemoveMeshRenderer(this);
	}
	
	void MeshRenderer::Draw(const Shader* shader)
	{
		if (m_Model)
		{
			shader->SetMat4("u_WorldTransform", m_Owner->GetWorldTransform());
			shader->SetVec3("u_SpecularColor", m_SpecularColor);
			shader->SetFloat("u_SpecularPower", m_SpecularPower);

			m_Model->Draw(shader);
		}
	}
}