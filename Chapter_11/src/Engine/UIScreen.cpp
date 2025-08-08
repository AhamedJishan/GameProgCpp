#include "UIScreen.h"

#include "Game.h"
#include "Texture.h"
#include "Shader.h"
#include "Font.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace jLab
{
	UIScreen::UIScreen(Game* game)
		:m_Game(game)
		,m_Title(nullptr)
		,m_TitlePos(glm::vec2(0, 280))
		,m_State(E_Active)
	{
		m_Game->PushUI(this);
		m_Font = m_Game->GetFont("Assets/Fonts/Carlito-Regular.ttf");
	}

	UIScreen::~UIScreen()
	{
		if (m_Title)
		{
			m_Title->Unload();
			delete m_Title;
		}
	}
	
	void UIScreen::Update(float deltaTime)
	{
	}
	
	void UIScreen::Draw(Shader* shader)
	{
		if (m_Title)
			DrawTexture(shader, m_Title, m_TitlePos);
	}
	
	void UIScreen::ProcessInput(InputState& inputState)
	{
	}
	
	void UIScreen::Close()
	{
		m_State = E_Closing;
	}

	void UIScreen::SetTitle(const std::string& text, glm::vec4 color, int pointSize)
	{
		if (m_Title)
		{
			m_Title->Unload();
			delete m_Title;
			m_Title = nullptr;
		}
		m_Title = m_Font->RenderText(text, color, pointSize);
	}

	void UIScreen::DrawTexture(Shader* shader, Texture* texture, glm::vec2 position, glm::vec2 scale)
	{
		glm::mat4 worldTransform = glm::scale(glm::mat4(1), glm::vec3(texture->GetWidth() * scale.x, texture->GetHeight() * scale.y, 1));
		worldTransform = glm::translate(glm::mat4(1), glm::vec3(position.x, position.y, 1.0f)) * worldTransform;

		shader->SetMat4("u_WorldTransform", worldTransform);
		texture->SetActive();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}