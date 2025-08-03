#include "MouseState.h"

namespace jLab
{
    MouseState::MouseState()
    {
        m_IsRelative = false;
        m_CurrentButtons = 0;
        m_PreviousButtons = 0;
        m_Position = glm::vec2(0);
        m_ScrollWheel= glm::vec2(0);
    }

    bool MouseState::GetButton(int button) const
    {
        return (SDL_BUTTON(button) & m_PreviousButtons) && (SDL_BUTTON(button) & m_CurrentButtons);
    }
    
    bool MouseState::GetButtonDown(int button) const
    {
        return !(SDL_BUTTON(button) & m_PreviousButtons) && (SDL_BUTTON(button) & m_CurrentButtons);
    }
    
    bool MouseState::GetButtonUp(int button) const
    {
        return (SDL_BUTTON(button) & m_PreviousButtons) && !(SDL_BUTTON(button) & m_CurrentButtons);
    }
}