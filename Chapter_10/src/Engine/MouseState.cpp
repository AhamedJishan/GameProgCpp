#include "MouseState.h"

namespace jLab
{
    MouseState::MouseState()
    {
        m_CurrentState = 0;
        m_PreviousState = 0;
        m_Position = glm::vec2(0);
        m_ScrollWheel = glm::vec2(0);
        m_IsRelative = false;
    }
    
    bool MouseState::GetButton(int button)
    {
        return (m_CurrentState & SDL_BUTTON(button)) && (m_PreviousState & SDL_BUTTON(button));
    }
    
    bool MouseState::GetButtonDown(int button)
    {
        return !(m_CurrentState & SDL_BUTTON(button)) && (m_PreviousState & SDL_BUTTON(button));
    }
    
    bool MouseState::GetButtonUp(int button)
    {
        return (m_CurrentState & SDL_BUTTON(button)) && !(m_PreviousState & SDL_BUTTON(button));
    }
}