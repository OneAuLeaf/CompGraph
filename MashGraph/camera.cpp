#include "camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 worldup, float yaw, float pitch) : 
    yaw{ yaw }, pitch{ pitch }, speed{ SPEED }, sense{ SENSITIVITY }, fov{ FOV },
    front{ glm::vec3(0.0f, 0.0f, -1.0f) }, position{ position }, worldup{ worldup }
{
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 new_front;
    new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    new_front.y = sin(glm::radians(pitch));
    new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    
    front = glm::normalize(new_front);
    right = glm::normalize(glm::cross(front, worldup));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::ProcessMouseMove(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= sense;
    yoffset *= sense;
    yaw += xoffset;
    pitch += yoffset;
    if (constrainPitch) {
        if (pitch > 89.0f) {
            pitch = 89.0f;
        }

        if (pitch < -89.0f) {
            pitch = -89.0f;
        }
    }
    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
}

void Camera::ProcessKeyboard(Move direction, float deltaTime)
{
    float velocity = speed * deltaTime;
    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;
}