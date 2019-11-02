#include "camera.hpp"
#include "input.h"

int g_aKeyStatus[GLFW_KEY_LAST];
extern Camera g_Camera;

void ProcessInput(float frameTime)
{
    if (g_aKeyStatus[GLFW_KEY_LEFT_SHIFT])
        frameTime *= 2.0f;

    if (g_aKeyStatus[GLFW_KEY_W])
        g_Camera.ProcessKeyboard(FORWARD, frameTime);
    if (g_aKeyStatus[GLFW_KEY_S])
        g_Camera.ProcessKeyboard(BACKWARD, frameTime);
    if (g_aKeyStatus[GLFW_KEY_A])
        g_Camera.ProcessKeyboard(LEFT, frameTime);
    if (g_aKeyStatus[GLFW_KEY_D])
        g_Camera.ProcessKeyboard(RIGHT, frameTime);
}

void MouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    double offsetX;
    double offsetY;
    static double lastX;
    static double lastY;

    offsetX = xpos - lastX;
    offsetY = ypos - lastY;
    lastX = xpos;
    lastY = ypos;
    g_Camera.ProcessMouseMovement(offsetX, -offsetY);
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    else if (key != GLFW_KEY_UNKNOWN && key < GLFW_KEY_LAST)
    {
        if (action == GLFW_PRESS)
            g_aKeyStatus[key] = true;
        else if (action == GLFW_RELEASE)
            g_aKeyStatus[key] = false;
    }
}
