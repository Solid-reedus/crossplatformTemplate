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

#include <string>

class BaseShader
{
	public:
	unsigned int ID;
	virtual ~BaseShader() = default;
	virtual void use() const = 0;

	virtual void setBool(const std::string& p_name, bool p_value) const = 0;
	virtual void setInt(const std::string& p_name, int p_value) const = 0;
	virtual void setFloat(const std::string& p_name, float p_value) const = 0;
	virtual void setVec3(const std::string& p_name, const glm::vec3& p_value) const = 0;
	virtual void setVec3(const std::string& p_name, float p_x, float p_y, float p_z) const = 0;
	virtual void setMat4(const std::string& p_name, const glm::mat4& p_mat) const = 0;

	protected:

};

