#pragma once

#include <glm/vec2.hpp>

namespace jLab
{
	class MouseState
	{
	public:
		friend class InputSystem;

		MouseState();

		glm::vec2 GetPosition() const;

		bool GetButtonValue(int button) const;
		enum ButtonState GetButtonState(int button) const;

	private:
		glm::vec2 m_Position;
		uint32_t m_CurrentState;
		uint32_t m_PreviousState;
	};
}