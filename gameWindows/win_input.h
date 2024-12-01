#pragma once

//#include <glad/glad.h> 
//#include <GLFW/glfw3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>



#include "../shared/baseInput.h"

class WinInput : public BaseInput
{
    public:
    WinInput(GLFWwindow* p_window) : m_window(p_window) {};

    bool IsKeyPressed(KeyBoardKey key) const override;
    KeyBoardKey PollKeyBoardInput() override;

    bool IsMouseButtonPressed(int button) const override;
    MouseInfo PollMouse() override;

    private:
    GLFWwindow* m_window = nullptr;

    int TranslateKey(BaseInput::KeyBoardKey key) const;

};

