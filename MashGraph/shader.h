#pragma once

#include <glad/glad.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


class Shader
{
public:
	unsigned int id = 0;
	int type = 0;
	std::string name;

public:
	Shader() = default;
	Shader(int type, const std::string path);
	
	void Attach(unsigned int program);
	void Delete();
private:
	unsigned int LoadNCreate(const std::string path, int type);
	void CheckErrors();
};

class ShaderProgram
{
public:
	unsigned int id = 0;
	Shader vertex;
	Shader fragment;
public:
	ShaderProgram() = default;
	ShaderProgram(const std::string vertex_path, const std::string fragment_path);

	void Use();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, const glm::vec2& value) const;
	void setVec2(const std::string& name, float x, float y) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w) const;
	void setMat2(const std::string& name, const glm::mat2& mat) const;
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;
private:
	void CheckErrors();
};