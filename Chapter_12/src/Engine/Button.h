#pragma once

#include <functional>
#include <string>
#include <vector>
#include <glm/vec2.hpp>

namespace jLab
{
	class Button
	{
	public:
		Button(const std::string& name, class Font* font, std::function<void()> onClick, glm::vec2 position, glm::vec2 dimensions);
		~Button();

		void SetName(const std::string& name);
		bool ContainsPoint(const glm::vec2& point);
		void OnClick();

		class Texture* GetNameTexture() const { return m_NameTexture; }
		glm::vec2 GetPosition() const { return m_Position; }
		bool GetHighlighted() const { return m_Highlighted; }

		void SetHighlighted(bool highlighted) { m_Highlighted = highlighted; }

	private:
		std::function<void()> m_OnClick;
		std::string m_Name;
		class Texture* m_NameTexture;
		class Font* m_Font;
		glm::vec2 m_Position;
		glm::vec2 m_Dimensions;
		bool m_Highlighted;
	};
}