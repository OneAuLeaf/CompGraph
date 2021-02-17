#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "object.h"
#include "framebuffer.h"

#include <iostream>
#include <map>
#include <string>

struct EffectState
{
	bool advanced_light;
	bool post_effect;
	bool rotation;

	float height_coeff;
	float shininess_coeff;

	EffectState();
};


class Global
{
public:
	//Window
	GLFWwindow* window;
	unsigned int width;
	unsigned int height;
	//Camera
	Camera camera;
	float last_x;
	float last_y;
	bool first_mouse;
	//Timer
	float delta_time;
	float last_frame;
	//Keyboard control
	unsigned int key_pressed;
	//EffectState
	EffectState state = {};
	//Objects
	std::map<std::string, Object*> objects = {};
	//Buffers
	std::map<std::string, Framebuffer*> buffers = {};
public:
	Global() = default;
	Global(unsigned int width, unsigned int height);

	void UpdateCurrentTime();
	glm::mat4 GetProjectionMatrix();

	void PrepareRender();
	void ProcessInput();
	void SwapBuffersNPollEvents();
	void Terminate();


private:
	void Init();
	void Configure();
	//callbacks
	void FramebufferSizeCallback(int width, int height);
	void MouseMoveCallback(double xpos, double ypos);
	void MouseScrollCallback(double xoffset, double yoffset);
	//proxy
	void friend ProxyFramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void friend ProxyMouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
	void friend ProxyMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};

void ProxyFramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProxyMouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
void ProxyMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);