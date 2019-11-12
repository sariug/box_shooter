#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera(glm::vec3 Position, glm::vec3 Up, GLfloat Yaw, GLfloat Pitch, GLfloat Movespeed, GLfloat Turnspeed);
    ~Camera();

    glm::mat4 calculateViewMatrix();

    void keyControl(bool* keys, GLfloat deltaTime);
    void mouseControl(glm::vec2 change);
        glm::vec3 get_front(){return front;};

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    GLfloat yaw;
    GLfloat pitch;

    GLfloat movementSpeed;
    GLfloat turnSpeed;

    void update();
};