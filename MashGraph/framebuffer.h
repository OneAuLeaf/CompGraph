#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "object.h"

struct Framebuffer
{
	unsigned int FBO;
	unsigned int RBO;
	Object screen;

	Framebuffer() = default;
	Framebuffer(std::string path_to_vertex_shader, std::string path_to_fragment_shader, unsigned int width, unsigned int height);

	void PrepareRender();
	void Draw();
	void Terminate();
	void UpdateScreenSize(int w, int h);
};
