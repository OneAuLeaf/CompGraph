#include "shader.h"

Shader::Shader(int type, const std::string path) : type{type}, name{path}
{
	try {
		id = LoadNCreate(path, type);
	}
	catch (std::ifstream::failure& error) {
		std::cout << "With path:" << path << " Error in loading shader code: " << error.code() << std::endl;
		id = 0;
	}
	glCompileShader(id);
	CheckErrors();
}

unsigned int Shader::LoadNCreate(const std::string path, int type)
{
	std::ifstream file;
	std::string source_code;

	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		file.open(path);
		std::stringstream in;
		in << file.rdbuf();
		source_code = in.str();
		file.close();
	} 
	catch (std::ifstream::failure& error) {
		throw error;
	}
	const char* raw_code = source_code.c_str();

	unsigned int id = glCreateShader(type);
	glShaderSource(id, 1, &raw_code, NULL);

	return id;
}

void Shader::Attach(unsigned int program_id) {
	glAttachShader(program_id, id);
}

void Shader::Delete() {
	glDeleteShader(id);
}

void Shader::CheckErrors() {
	int success;
	char info_log[1024];

	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(id, 1024, NULL, info_log);
		std::cout << "With shader " << name << std::endl;
		if (type == GL_VERTEX_SHADER) {
			std::cout << "Error in compile (type: GL_VERTEX_SHADER)" << std::endl
				<< info_log << std::endl;
		}
		else if (type == GL_FRAGMENT_SHADER) {
			std::cout << "Error in compile (type: GL_FRAGMENT_SHADER)" << std::endl
				<< info_log << std::endl;
		}
	}
}

ShaderProgram::ShaderProgram(const std::string vertex_path, const std::string fragment_path)
{
	vertex = Shader(GL_VERTEX_SHADER, vertex_path);
	fragment = Shader(GL_FRAGMENT_SHADER, fragment_path);

	id = glCreateProgram();

	vertex.Attach(id);
	fragment.Attach(id);
	
	glLinkProgram(id);
	CheckErrors();

	vertex.Delete();
	fragment.Delete();
}

void ShaderProgram::Use()
{
	glUseProgram(id);
}

void ShaderProgram::CheckErrors() {
	int success;
	char info_log[1024];

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 1024, NULL, info_log);
		std::cout << "Error in linking: " << std::endl << info_log << std::endl;
	}

}


void ShaderProgram::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void ShaderProgram::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void ShaderProgram::setVec2(const std::string& name, const glm::vec2& value) const {
	glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setVec2(const std::string& name, float x, float y) const {
	glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}

void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) const {
	glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setVec3(const std::string& name, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}

void ShaderProgram::setVec4(const std::string& name, const glm::vec4& value) const {
	glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setVec4(const std::string& name, float x, float y, float z, float w) const {
	glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}

void ShaderProgram::setMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::setMat3(const std::string& name, const glm::mat3& mat) const {
	glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const {
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}