#pragma once

#include "../shared/baseWindow.h"
#include <GLFW/glfw3.h>

class WinWindow : BaseWindow
{
	public:
	WinWindow();
	void Update() override;
	bool Init() override;

	const bool CheckIfWindowShouldClose() const { return !glfwWindowShouldClose(m_window);};
	GLFWwindow* GetGLFWWindow() const { return m_window; };

	private:
	GLFWwindow* m_window = nullptr;

};

