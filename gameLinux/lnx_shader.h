#pragma once

#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../shared/baseShader.h"
#include <string>

class LnxShader : public BaseShader
{
	public:
	//unsigned int ID;

	LnxShader(const char* p_vertexShaderDir, const char* p_fragmentShaderDir);
	~LnxShader() {};
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

