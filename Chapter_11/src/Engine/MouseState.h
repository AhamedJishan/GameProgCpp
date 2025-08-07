#pragma once

#include <SDL/SDL.h>
#include <glm/vec2.hpp>

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
		Uint32 m_CurrentState;
		Uint32 m_PreviousState;

		glm::vec2 m_Position;
		glm::vec2 m_ScrollWheel;
		bool m_IsRelative;
	};
}