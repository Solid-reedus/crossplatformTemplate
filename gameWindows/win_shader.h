#pragma once

//#include "glad/glad.h" 
//#include "GLFW/glfw3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../libraries/shared/glm/glm.hpp"
#include <gtc/matrix_transform.hpp>

//#include <glm.hpp>
//#include <gtc/matrix_transform.hpp>



#include "../shared/baseShader.h"
#include <string>

class WinShader : public BaseShader
{
	public:
	//unsigned int ID;

	WinShader(const char* p_vertexShaderDir, const char* p_fragmentShaderDir);
	~WinShader() {};
	void use() const override
	{
		glUseProgram(ID);
	};


	void setBool(const std::string& p_name, bool p_value) const override;
	void setInt(const std::string& p_name, int p_value) const override;
	void setFloat(const std::string& p_name, float p_value) const override;

	void setVec3(const std::string& p_name, const glm::vec3& p_value) const override;
	void setVec3(const std::string& p_name, float p_x, float p_y, float p_z) const override;

	void setMat4(const std::string& p_name, const glm::mat4& p_mat) const override;

	private:
};

