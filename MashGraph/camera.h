#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>


enum Move {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;

class Camera
{
public:
    float yaw;
    float pitch;
    float speed;
    float sense;
    float fov;

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldup;

public:
    Camera() = default;
    Camera(glm::vec3 position, glm::vec3 worldup, float yaw = YAW, float pitch = PITCH);
    
    glm::mat4 GetViewMatrix();

    void ProcessMouseMove(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);
    void ProcessKeyboard(Move direction, float deltaTime);

private:
    void UpdateCameraVectors();
};

