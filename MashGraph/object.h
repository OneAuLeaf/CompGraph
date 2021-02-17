#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "texture.h"

#include <string>
#include <vector>


struct Object
{
	std::vector<float> vertices;
	std::vector<int> vertice_offsets;
	unsigned int VBO;
	unsigned int VAO;
	ShaderProgram shader;
	std::vector<Texture> textures;

	Object() = default;
	Object(std::vector<float>& vert, std::vector<int>& offsets,
		std::string path_to_vertex_shader, std::string path_to_fragment_shader,
		std::vector<std::string>& paths_to_textures, unsigned int textures_type, unsigned int color_sheme = GL_RGB, unsigned int width = 0, unsigned int height = 0);

	void PrepareRender();
	void Draw();
	void Terminate();

};