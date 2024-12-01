#include "game.h"

#ifdef LINUX_BUILD
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif // LINUX_BUILD

#ifdef WINDOWS_BUILD
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#endif // WINDOWS_BUILD


#include "model.h"
#include "camera.h"
#include "common.h"

#include <iostream>
#include <string>

Game::Game(BaseWindow* p_window, BaseInput* p_input, BaseShader* p_shader)
	: m_window(p_window), m_input(p_input), m_shader(p_shader)
{}

void Game::Run()
{
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
	Model* model = new Model(std::string(ASSETS_DIR) + "monkey/monkey.obj");
	glm::vec3 cameraPosition(0.0f, 0.0f, 3.0f); // Move camera back
	glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);  // Look at the origin
	Camera* cam = new Camera(cameraPosition, cameraTarget);

	while (!m_window->ShouldClose())
	{
		m_window->Clear();
		m_shader->use();

		glm::mat4 MVP = cam->GetProjectionMatrix() * cam->GetViewMatrix() * trans;
		m_shader->setMat4("MVP", MVP);
		m_shader->setMat4("model", trans);
		model->Draw(dynamic_cast<BaseShader*>(m_shader), *cam);

		m_window->Update();
	}

}
