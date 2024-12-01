#pragma once

#include "../shared/baseWindow.h"
//#include <glad/glad.h> 
//#include <GLFW/glfw3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


class WinWindow : public BaseWindow
{
	public:
	WinWindow();
	void Update() override;
	void Clear() override;
	bool Init() override;


	bool ShouldClose() const override { return glfwWindowShouldClose(m_window); };
	GLFWwindow* GetGLFWWindow() const { return m_window; };

	private:
	GLFWwindow* m_window = nullptr;

};

