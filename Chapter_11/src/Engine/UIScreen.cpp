#include "UIScreen.h"

#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"
#include "Font.h"
#include "Button.h"
#include "InputSystem.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace jLab
{
	UIScreen::UIScreen(Game* game)
		:m_Game(game)
		,m_Title(nullptr)
		,m_Background(nullptr)
		,m_TitlePos(glm::vec2(0, 280))
		, m_NextButtonPos(glm::vec2(0, 200))
		,m_BGPos(0,0)
		,m_State(E_Active)
	{
		m_Game->PushUI(this);
		m_Font = m_Game->GetFont("Assets/Fonts/Carlito-Regular.ttf");
		m_ButtonOn = m_Game->GetRenderer()->GetTexture("Assets/Textures/ButtonYellow.png");
		m_ButtonOff = m_Game->GetRenderer()->GetTexture("Assets/Textures/ButtonBlue.png");
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
		// Draw Background
		if (m_Background)
			DrawTexture(shader, m_Background, m_BGPos);

		// Draw Title
		if (m_Title)
			DrawTexture(shader, m_Title, m_TitlePos);

		// Draw Buttons
		for (Button* button : m_Buttons)
		{
			Texture* texture = button->GetHighlighted() ? m_ButtonOn : m_ButtonOff;

			DrawTexture(shader, texture, button->GetPosition());
			DrawTexture(shader, button->GetNameTexture(), button->GetPosition());
		}
	}
	
	void UIScreen::ProcessInput(InputState& inputState)
	{
		if(!m_Buttons.empty())
		{
			glm::vec2 mousePos = inputState.Mouse.GetPosition();

			for (Button* button : m_Buttons)
			{
				bool highlight = button->ContainsPoint(mousePos);
				if (highlight)
				{
					button->SetHighlighted(true);
					if(inputState.Mouse.GetButtonDown(SDL_BUTTON_LEFT))
					{
						button->OnClick();
					}
				}
				else
				{
					button->SetHighlighted(false);
				}
			}
		}
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

	void UIScreen::AddButton(const std::string& name, std::function<void()> onClick)
	{
		glm::vec2 dimensions = glm::vec2(m_ButtonOn->GetWidth(), m_ButtonOn->GetHeight());

		Button* button = new Button(name, m_Font, onClick, m_NextButtonPos, dimensions);
		m_Buttons.emplace_back(button);

		m_NextButtonPos.y -= m_ButtonOff->GetHeight() + 20;
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