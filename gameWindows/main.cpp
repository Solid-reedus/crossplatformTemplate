
#include <iostream>
#include "../shared/game.h"
#include "win_window.h"
#include "win_input.h"


int main()
{
    //construct the game
    WinWindow* window = new WinWindow();
    WinInput* input = new WinInput(window->GetGLFWWindow());

    while (window->CheckIfWindowShouldClose())
    {
        BaseInput::MouseInfo info = input->PollMouse();
        printf("mouse button info is %d \n", info.buttonState);
        BaseInput::KeyBoardKey key =  input->PollKeyBoardInput();
        printf("key info is %d \n", static_cast<int>(key));

        window->Update();

    }
}
