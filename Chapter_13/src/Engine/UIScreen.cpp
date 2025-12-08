#include "UIScreen.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLEW/GL/glew.h>

#include "Game.h"
#include "Renderer.h"
#include "InputSystem.h"
#include "Texture.h"
#include "Shader.h"
#include "Font.h"
#include "Button.h"

namespace jLab
{
	UIScreen::UIScreen(Game* game)
		:mGame(game)
		,mTitle(nullptr)
		,mTitlePos(glm::vec2(0))
		,mBGPos(glm::vec2(0))
		,mBGScale(glm::vec2(1))
		,mNextButtonPos(glm::vec2(0, -100))
		,mState(State::Active)
	{
		mGame->PushUI(this);
		mFont = mGame->GetFont("Assets/Fonts/Carlito-Regular.ttf");
		mBGTexture = mGame->GetRenderer()->GetTexture("Assets/Textures/DialogBG.png");
		mButtonOnTexture = mGame->GetRenderer()->GetTexture("Assets/Textures/ButtonYellow.png");
		mButtonOffTexture = mGame->GetRenderer()->GetTexture("Assets/Textures/ButtonBlue.png");
	}

	UIScreen::~UIScreen()
	{
		if (mTitle)
			delete mTitle;

		for (Button* button : mButtons)
			delete button;
		mButtons.clear();
	}
	
	void UIScreen::Update(float deltaTime)
	{
	}
	
	void UIScreen::Input(InputState& inputState)
	{
		glm::vec2 mousePos = inputState.Mouse.GetPosition();

		for (Button* button : mButtons)
		{
			if (button->ContainsPoint(mousePos))
				button->SetHighlighted(true);
			else
				button->SetHighlighted(false);
		}

		if (inputState.Mouse.GetButtonDown(SDL_BUTTON_LEFT))
			for (Button* button : mButtons)
				if (button->GetHighlighted()) button->OnClick();
	}
	
	void UIScreen::Draw(const Shader* shader)
	{
		if (mBGTexture)
			DrawTexture(shader, mBGTexture, mBGPos, mBGScale);

		if (mTitle)
			DrawTexture(shader, mTitle, mTitlePos);

		for (Button* button : mButtons)
		{
			Texture* bg = button->GetHighlighted() ? mButtonOnTexture : mButtonOffTexture;
			DrawTexture(shader, bg, button->GetPosition());
			DrawTexture(shader, button->GetNameTexture(), button->GetPosition());
		}
	}
	
	void UIScreen::Close()
	{
		mState = State::Closing;
	}
	
	void UIScreen::SetTitle(const std::string& text, glm::vec4 color, int pointSize)
	{
		if (mTitle)
			delete mTitle;

		mTitle = mFont->RenderText(text, color, pointSize);
	}
	
	void UIScreen::AddButton(const std::string& name, std::function<void()> onClick)
	{
		glm::vec2 dims = glm::vec2(mButtonOffTexture->GetWidth(), mButtonOffTexture->GetHeight());
		Button* button = new Button(name, onClick, mFont, mNextButtonPos, dims);
		mButtons.emplace_back(button);

		mNextButtonPos.y -= mButtonOffTexture->GetHeight() + 20.0f;
	}

	void UIScreen::DrawTexture(const Shader* shader, Texture* texture, glm::vec2 offset, glm::vec2 scale)
	{
		glm::mat4 scaleMat = glm::scale(glm::mat4(1), glm::vec3(scale.x * texture->GetWidth(), scale.y * texture->GetHeight(), 0));
		glm::mat4 translationMat = glm::translate(glm::mat4(1), glm::vec3(offset.x, offset.y, 0));
		glm::mat4 worldTransform = translationMat * scaleMat;

		shader->SetMat4("uWorldTransform", worldTransform);
		texture->SetActive();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}