#include "Window.h"

Window::Window() : width(800), height(600)
{
    for (std::size_t i; i < 1024; i++)
        keys[i] = 0;
}
Window::Window(GLint windowWidth, GLint windowHeight) : width(windowWidth), height(windowHeight)
{
    for (std::size_t i; i < 1024; i++)
        keys[i] = 0;
}

Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}

int Window::initialize()
{
    // Initialise GLFW
    if (!glfwInit())
    {
        std::cout << "GLFW Initialisation has failed!" << std::endl;
        glfwTerminate();
        return 1;
    }
    // Setup GLFW Window Properties
    // OpenGL Version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core Profile means that there will be no backward compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    mainWindow = glfwCreateWindow(width, height, "Box Shooter", NULL, NULL);
    if (!mainWindow)
    {
        std::cout << "GLFW Window Creation has failed!" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Get Framebuffer size information
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLFW to use
    glfwMakeContextCurrent(mainWindow);

    // Handle key callbacks
    this->createCallback();
    glfwSetInputMode(mainWindow, GLFW_STICKY_KEYS, GL_FALSE);
    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW Initialisation has failed!" << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }
    // Set Viewport Size
    glViewport(0, 0, bufferWidth, bufferHeight);
    glfwSetWindowUserPointer(this->mainWindow, this);
}
void Window::createCallback()
{
    glfwSetKeyCallback(mainWindow, handlekeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}
void Window::handlekeys(GLFWwindow *window, int key, int code, int action, int mode)
{
    Window *theWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            theWindow->keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            theWindow->keys[key] = false;
        }
    }
}
void Window::handleMouse(GLFWwindow *window, double xPos, double yPos)
{
    Window *theWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));

    if (theWindow->mouseFirstMoved)
    {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMoved = false;
    }

    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos;

    theWindow->lastY = yPos;
    theWindow->lastX = xPos;
}
std::tuple<GLfloat, GLfloat, bool> Window::mouse_feedback()
{
    // If pressed
    if (glfwGetMouseButton(mainWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        return std::make_tuple(lastX, lastY, true);
    }
    return std::make_tuple(lastX, lastY, false);
}

GLfloat Window::getX() { return lastX; }
GLfloat Window::getY() { return lastY; }

GLfloat Window::getXchange()
{
    GLfloat theChange = xChange;
    xChange = 0.0f;
    return theChange;
}
GLfloat Window::getYchange()
{
    GLfloat theChange = yChange;
    yChange = 0.0f;
    return theChange;
}
