#include "MeshRenderer.h"

#include <Engine/Actor.h>
#include <Engine/Game.h>
#include <Engine/Renderer.h>
#include "Engine/Model.h"
#include "Engine/Shader.h"

namespace jLab
{
	MeshRenderer::MeshRenderer(Actor* owner, int drawOrder)
		:Component(owner, drawOrder),
		m_Model(nullptr),
		m_SpecularColor(glm::vec3(0)),
		m_SpecularPower(32)
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
			shader->SetFloat("u_SpecularPower", m_SpecularPower);
			shader->SetVec3("u_SpecularColor", m_SpecularColor);
			m_Model->Draw(shader);
		}
	}
}