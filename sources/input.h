#pragma once
#include <GLFW/glfw3.h>

void ProcessInput(float frameTime);
void MouseCallback(GLFWwindow *window, double xpos, double ypos);
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
