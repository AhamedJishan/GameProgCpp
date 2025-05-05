#pragma once

#include <glm/vec2.hpp>
#include <SDL/SDL.h>

namespace jLab
{
	class MouseState
	{
	public:
		friend class InputSystem;

		MouseState();

		bool GetButton(int button) const;
		bool GetButtonDown(int button) const;
		bool GetButtonUp(int button) const;
		bool IsRelative() const { return m_IsRelative; }

		glm::vec2 GetPosition() const { return m_Position; }
		glm::vec2 GetScrollWheel() const { return m_ScrollWheel; }

	private:
		glm::vec2 m_Position;
		glm::vec2 m_ScrollWheel;

		Uint32 m_CurrButtons;
		Uint32 m_PrevButtons;
		bool m_IsRelative;
	};
}