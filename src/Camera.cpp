#include "Camera.h"
#include <cmath>
#include <iostream>
Camera::Camera(glm::vec3 Position, glm::vec3 Up, GLfloat Yaw, GLfloat Pitch, GLfloat Movespeed, GLfloat Turnspeed) : position(Position), worldUp(Up), yaw(Yaw), pitch(Pitch), movementSpeed(Movespeed), turnSpeed(Turnspeed)
{
    front = glm::vec3(.0f, .0f, -1.0f);
    this->update();
}

void Camera::update()
{

    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    front = glm::normalize(front);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));


}

void Camera::keyControl(bool *keys, GLfloat deltaTime)
{
    auto velocity = movementSpeed * deltaTime;
    if (keys[GLFW_KEY_W])
    {
        position += front * velocity;
    }
    if (keys[GLFW_KEY_S])
    {
        position -= front * velocity;
    }
    if (keys[GLFW_KEY_A])
    {
        position -= right * velocity;
    }
    if (keys[GLFW_KEY_D])
    {
        position += right * velocity;
    }
}
glm::mat4 Camera::calculateViewMatrix()
{
    return glm::lookAt(position,
                       position + front,
                       up);
}

void Camera::mouseControl(glm::vec2 change)
{

}