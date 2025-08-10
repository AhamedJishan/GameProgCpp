#include "MouseState.h"

namespace jLab
{
    MouseState::MouseState()
    {
        m_CurrentState = 0;
        m_PreviousState = 0;
        m_Position = glm::vec2(0);
        m_RelativePosition = glm::vec2(0);
        m_ScrollWheel = glm::vec2(0);
        m_IsCursorLocked = false;
    }

    bool MouseState::GetButton(int button) const
    {
        return (m_PreviousState & SDL_BUTTON(button)) && (m_CurrentState & SDL_BUTTON(button));
    }

    bool MouseState::GetButtonDown(int button) const
    {
        return !(m_PreviousState & SDL_BUTTON(button)) && (m_CurrentState & SDL_BUTTON(button));
    }

    bool MouseState::GetButtonUp(int button) const
    {
        return (m_PreviousState & SDL_BUTTON(button)) && !(m_CurrentState & SDL_BUTTON(button));
    }

    void MouseState::SetCursorLocked(bool value)
    {
        m_IsCursorLocked = value;

        SDL_bool set = value ? SDL_TRUE : SDL_FALSE;
        SDL_SetRelativeMouseMode(set);
    }
}