#pragma once

#include <glm/vec2.hpp>

namespace jLab
{
	class MouseState
	{
	public:
		friend class InputSystem;

		MouseState();

		glm::vec2 GetPosition() const { return m_Position; }
		glm::vec2 GetScrollWheel() const { return m_ScrollWheel; }

		bool GetButtonValue(int button) const;
		enum ButtonState GetButtonState(int button) const;

	private:
		glm::vec2 m_Position;
		glm::vec2 m_ScrollWheel;
		uint32_t m_CurrentState;
		uint32_t m_PreviousState;
		bool m_RelativeMode;
	};
}