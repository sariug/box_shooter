#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <tuple>
#include <iostream>
class Window
{
public:
    Window();
    Window(GLint windowWidth, GLint windowHeight);

    int initialize();

    GLfloat getBufferWidth() { return bufferWidth; }
    GLfloat getBufferHeight() { return bufferHeight; }

    bool shouldWindowClose()
    {
        return glfwWindowShouldClose(mainWindow);
    }

    bool *getKeys() { return keys; }

    GLfloat getXchange();
    GLfloat getYchange();
    GLfloat getX();
    GLfloat getY();
    std::tuple<GLfloat, GLfloat, bool> mouse_feedback();
    void swapBuffers() { glfwSwapBuffers(mainWindow); }

    ~Window();

private:
    GLFWwindow *mainWindow;
    GLint width, height;
    int bufferWidth, bufferHeight;

    // Mouse
    GLfloat lastX;
    GLfloat lastY;
    GLfloat xChange;
    GLfloat yChange;
    bool mouseFirstMoved;
    static void handleMouse(GLFWwindow *window, double xPos, double yPos);

    // Keyboard
    bool keys[1024];
    void createCallback();
    static void handlekeys(GLFWwindow *window, int key, int code, int action, int mode);
};
