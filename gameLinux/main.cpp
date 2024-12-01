#include "lnx_window.h"
#include "lnx_input.h"
#include "lnx_shader.h"

#include "game.h"

#include <unistd.h>
#include <iostream>
#include <cstdio>

int main()
{
    LnxWindow* window = new LnxWindow();
    LnxInput* input = new LnxInput();
    LnxShader* shader = new LnxShader("../../../gameLinux/default_vert_lnx.glsl", "../../../gameLinux/default_frag_lnx.glsl");

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) 
    {
        std::cout << "Current working directory: " << cwd << std::endl;
    }

    Game game = Game(dynamic_cast<BaseWindow*>(window), dynamic_cast<BaseInput*>(input), dynamic_cast<BaseShader*>(shader));
    game.Run();

    return 1;
}
