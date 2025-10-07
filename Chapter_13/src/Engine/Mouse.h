#pragma once

#include <SDL/SDL.h>
#include <glm/vec2.hpp>

namespace jLab
{
	class Mouse
	{
	public:
		Mouse();

		bool GetButton(Uint32 button);
		bool GetButtonDown(Uint32 button);
		bool GetButtonUp(Uint32 button);

		glm::vec2 GetPosition() const { return mPosition; }
		glm::vec2 GetRelativePosition() const { return mRelativePosition; }
		glm::vec2 GetScrollWheel() const { return mScrollWheel; }

		bool IsCursorLocked() const { return mIsCursorLocked; }

	private:
		friend class InputSystem;

		Uint32 mCurrentState;
		Uint32 mPreviousState;

		glm::vec2 mPosition;
		glm::vec2 mRelativePosition;
		glm::vec2 mScrollWheel;

		bool mIsCursorLocked;
	};
}