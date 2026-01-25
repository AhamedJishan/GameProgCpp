#pragma once

#include <string>
#include <vector>
#include <functional>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace jLab
{
	class UIScreen
	{
	public:
		enum class State
		{
			Active,
			Closing
		};

		UIScreen(class Game* game);
		virtual ~UIScreen();

		virtual void Update(float deltaTime);
		virtual void Input(struct InputState& inputState);
		virtual void Draw(const class Shader* shader);

		void Close();

		void SetTitle(const std::string& text, glm::vec4 color = glm::vec4(1), int pointSize = 40);

		State GetState() const { return mState; };

		void AddButton(const std::string& name, std::function<void()> onClick);

	protected:
		void DrawTexture(const class Shader* shader,
			class Texture* texture,
			glm::vec2 offset = glm::vec2(0),
			glm::vec2 scale = glm::vec2(1));

	protected:
		class Game* mGame;
		class Font* mFont;
		class Texture* mTitle;
		class Texture* mBGTexture;
		class Texture* mButtonOnTexture;
		class Texture* mButtonOffTexture;

		glm::vec2 mTitlePos;
		glm::vec2 mBGPos;
		glm::vec2 mBGScale;
		glm::vec2 mNextButtonPos;

		State mState;

		std::vector<class Button*> mButtons;
	};
}