#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

#include <iostream>
#include <vector>
#include <string>

struct Texture
{
	unsigned int id;
	unsigned int type;
	unsigned int width;
	unsigned int height;

	Texture() = default;

	Texture(std::string path, unsigned int type, unsigned int color_scheme = GL_RGB, unsigned int width = 0, unsigned int height = 0);
	Texture(std::vector<std::string> paths, unsigned int color_scheme = GL_RGB, unsigned int width = 0, unsigned int height = 0); // for CUBE_MAPs

	void Attach(unsigned int index = 0);

private:
	void LoadNInit(std::string path, unsigned color_scheme);
	void LoadNInit(std::vector<std::string> paths, unsigned color_scheme);
};




