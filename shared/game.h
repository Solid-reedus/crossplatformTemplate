#pragma once

#include "baseInput.h"
#include "baseWindow.h"
#include "baseShader.h"

class Game
{
	public:
	Game(BaseWindow* p_window, BaseInput* p_input, BaseShader* p_shader);
	void Run();

	private:
	BaseWindow* m_window;
	BaseInput* m_input;
	BaseShader* m_shader;

};

