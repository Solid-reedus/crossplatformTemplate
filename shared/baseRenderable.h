#pragma once

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


#include <vector>
#include "instance.h"

struct Camera;

class BaseRenderable
{
	public:
	BaseRenderable(Camera& p_cameraRef) : m_cameraRef(p_cameraRef) {}
	virtual void Render() = 0;
	virtual void AddInstance(Instance newInstance) { m_intances.push_back(newInstance); };
	virtual ~BaseRenderable() = default;

	protected:
	//Shader* m_shader = nullptr;
	//Model* m_model = nullptr;

	std::vector<Instance> m_intances;
	Camera& m_cameraRef;

};

