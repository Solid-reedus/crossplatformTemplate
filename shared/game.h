#pragma once

#include "baseInput.h"
#include "baseWindow.h"
#include "baseRenderer.h"

class Game
{

	public:
	Game(BaseWindow* p_window, BaseInput* p_input, BaseRenderer* p_renderer);

	private:
	BaseWindow* m_window;
	BaseInput* m_input;
	BaseRenderer* m_renderer;

};

