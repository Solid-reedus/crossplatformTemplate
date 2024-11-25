#include "win_input.h"
#include <GLFW/glfw3.h>
#include "../shared/baseInput.h"

// Translate BaseInput::KeyBoardKey to GLFW key codes
int WinInput::TranslateKey(BaseInput::KeyBoardKey key) const
{
    switch (key) 
    {
        case BaseInput::KeyBoardKey::KEY_A: return GLFW_KEY_A;
        case BaseInput::KeyBoardKey::KEY_B: return GLFW_KEY_B;
        case BaseInput::KeyBoardKey::KEY_C: return GLFW_KEY_C;
        case BaseInput::KeyBoardKey::KEY_D: return GLFW_KEY_D;
        case BaseInput::KeyBoardKey::KEY_E: return GLFW_KEY_E;
        case BaseInput::KeyBoardKey::KEY_F: return GLFW_KEY_F;
        case BaseInput::KeyBoardKey::KEY_G: return GLFW_KEY_G;
        case BaseInput::KeyBoardKey::KEY_H: return GLFW_KEY_H;
        case BaseInput::KeyBoardKey::KEY_I: return GLFW_KEY_I;
        case BaseInput::KeyBoardKey::KEY_J: return GLFW_KEY_J;
        case BaseInput::KeyBoardKey::KEY_K: return GLFW_KEY_K;
        case BaseInput::KeyBoardKey::KEY_L: return GLFW_KEY_L;
        case BaseInput::KeyBoardKey::KEY_M: return GLFW_KEY_M;
        case BaseInput::KeyBoardKey::KEY_N: return GLFW_KEY_N;
        case BaseInput::KeyBoardKey::KEY_O: return GLFW_KEY_O;
        case BaseInput::KeyBoardKey::KEY_P: return GLFW_KEY_P;
        case BaseInput::KeyBoardKey::KEY_Q: return GLFW_KEY_Q;
        case BaseInput::KeyBoardKey::KEY_R: return GLFW_KEY_R;
        case BaseInput::KeyBoardKey::KEY_S: return GLFW_KEY_S;
        case BaseInput::KeyBoardKey::KEY_T: return GLFW_KEY_T;
        case BaseInput::KeyBoardKey::KEY_U: return GLFW_KEY_U;
        case BaseInput::KeyBoardKey::KEY_V: return GLFW_KEY_V;
        case BaseInput::KeyBoardKey::KEY_W: return GLFW_KEY_W;
        case BaseInput::KeyBoardKey::KEY_X: return GLFW_KEY_X;
        case BaseInput::KeyBoardKey::KEY_Y: return GLFW_KEY_Y;
        case BaseInput::KeyBoardKey::KEY_Z: return GLFW_KEY_Z;
        case BaseInput::KeyBoardKey::KEY_LEFT: return GLFW_KEY_LEFT;
        case BaseInput::KeyBoardKey::KEY_RIGHT: return GLFW_KEY_RIGHT;
        case BaseInput::KeyBoardKey::KEY_UP: return GLFW_KEY_UP;
        case BaseInput::KeyBoardKey::KEY_DOWN: return GLFW_KEY_DOWN;
        default: return -1; // Unsupported key
    }
}

// Check if a specific key is currently pressed
bool WinInput::IsKeyPressed(BaseInput::KeyBoardKey key) const 
{
    int glfwKey = TranslateKey(key);
    if (glfwKey == -1) 
    {
        return false; // Unsupported key
    }
    return glfwGetKey(m_window, glfwKey) == GLFW_PRESS;
}

// Poll for a single key input (simplified example)
BaseInput::KeyBoardKey WinInput::PollKeyBoardInput() 
{
    // This function can be customized to poll for specific key events
    for (int key = GLFW_KEY_A; key <= GLFW_KEY_Z; ++key) 
    {
        if (glfwGetKey(m_window, key) == GLFW_PRESS) 
        {
            return static_cast<BaseInput::KeyBoardKey>(key - GLFW_KEY_A + static_cast<int>(BaseInput::KeyBoardKey::KEY_A));
        }
    }
    return BaseInput::KeyBoardKey::CURRENT_KEY_UNKNOWN;
}

// Check if a mouse button is currently pressed
bool WinInput::IsMouseButtonPressed(int button) const 
{
    return glfwGetMouseButton(m_window, button) == GLFW_PRESS;
}

// Poll for mouse input information
BaseInput::MouseInfo WinInput::PollMouse() 
{
    BaseInput::MouseInfo mouseInfo = {};
    double xpos, ypos;

    // Get current mouse position
    glfwGetCursorPos(m_window, &xpos, &ypos);
    mouseInfo.x = static_cast<int>(xpos);
    mouseInfo.y = static_cast<int>(ypos);

    // Update button state (example: left mouse button)
    mouseInfo.buttonState = 0;
    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) 
    {
        mouseInfo.buttonState |= (1 << GLFW_MOUSE_BUTTON_LEFT);
    }
    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) 
    {
        mouseInfo.buttonState |= (1 << GLFW_MOUSE_BUTTON_RIGHT);
    }
    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) 
    {
        mouseInfo.buttonState |= (1 << GLFW_MOUSE_BUTTON_MIDDLE);
    }

    return mouseInfo;
}
