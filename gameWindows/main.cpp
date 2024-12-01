#include "win_shader.h"
#include "win_window.h"
#include "win_input.h"

#include "../shared/game.h"


int main()
{
    WinWindow* window = new WinWindow();
    WinInput* input = new WinInput(window->GetGLFWWindow());
    WinShader* shader = new WinShader("default_vert_win.glsl", "default_frag_win.glsl");

    Game game = Game(dynamic_cast<BaseWindow*>(window), dynamic_cast<BaseInput*>(input), dynamic_cast<BaseShader*>(shader));
    game.Run();

    return 1;
}

