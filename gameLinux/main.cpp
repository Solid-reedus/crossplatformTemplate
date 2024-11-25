#include <cstdio>
#include "../shared/game.h"
#include "lnx_window.h"
#include "lnx_input.h"

int main()
{
    LnxWindow* window = new LnxWindow();
    LnxInput* input = new LnxInput();

    while (!window->ShouldClose()) 
    {
        window->Update();
        //printf("mouseInput %d \n", input->PollMouse().buttonState);
        input->PollKeyBoardInput();
        //printf("key input %d \n", static_cast<int>(input->PollKeyBoardInput()));

    }


    return 0;
}
