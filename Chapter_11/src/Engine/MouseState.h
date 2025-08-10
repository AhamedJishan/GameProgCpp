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

		glm::vec2 GetPosition() const { return m_Position; }
		glm::vec2 GetRelativePosition() const { return m_RelativePosition; }
		glm::vec2 GetScrollWheel() const { return m_ScrollWheel; }

		void SetCursorLocked(bool value);
		bool IsCursorLocked() const { return m_IsCursorLocked; }

	private:
		Uint32 m_CurrentState;
		Uint32 m_PreviousState;

		glm::vec2 m_Position;
		glm::vec2 m_RelativePosition;
		glm::vec2 m_ScrollWheel;

		bool m_IsCursorLocked;
	};
}