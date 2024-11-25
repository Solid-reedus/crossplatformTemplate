#pragma once

#include "../shared/baseInput.h"
#include <linux/input.h>
#include <string>
#include <thread>
#include <atomic>
#include <map>

class LnxInput : public BaseInput 
{
    public:
    LnxInput();
    ~LnxInput();

    bool IsKeyPressed(KeyBoardKey key) const override;
    KeyBoardKey PollKeyBoardInput() override;

    bool IsMouseButtonPressed(int button) const override;
    MouseInfo PollMouse() override;

    private:
    std::thread m_inputThread;
    std::atomic<bool> m_running;
    std::string m_keyboardDevice;
    std::string m_mouseDevice;


    mutable std::map<int, bool> m_keyState;
    MouseInfo m_mouseState;

    void InputThread();
    std::string FindInputDevice(const std::string& deviceType);
};
