#pragma once

#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace jLab
{
	class UIScreen
	{
	public:
		UIScreen(class Game* game);
		virtual ~UIScreen();

		virtual void Update(float deltaTime);
		virtual void Draw(class Shader* shader);
		virtual void ProcessInput(struct InputState& inputState);

		enum UIState { E_Active, E_Closing };
		void Close();
		UIState GetState() const { return m_State; }

		void SetTitle(const std::string& text, glm::vec4 color = glm::vec4(1), int pointSize = 40);

	protected:
		void DrawTexture(class Shader* shader, class Texture* texture,
			glm::vec2 position = glm::vec2(0),
			glm::vec2 scale = glm::vec2(1)
		);

	protected:
		class Game* m_Game;
		class Font* m_Font;
		class Texture* m_Title;
		glm::vec2 m_TitlePos;
		UIState m_State;
	};
}