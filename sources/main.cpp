#define GLEW_STATIC
#include <glew.h>
#include <GLFW/glfw3.h>
#include <FreeImage.h>

#include "input.h"
#include "exception.h"
#include "rendering.h"
#include <iostream>

GLFWwindow *g_pWindow;

static void ProgramInit()
{
    int windowWidth     = 1600;
    int windowHeight    = 900;
    int versionMajor    = 3;
    int versionMinor    = 3;
    int msaaSamples     = 4;

#ifdef FREEIMAGE_LIB
    FreeImage_Initialise();
#endif
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // MSAA enabling
    glfwWindowHint(GLFW_SAMPLES, msaaSamples);
    glEnable(GLFW_SAMPLES);

    // window creating
    g_pWindow = glfwCreateWindow(
        windowWidth, 
        windowHeight, 
        "Another World Evil Effect VFX Demo", 
        nullptr, 
        nullptr
    );
    if (!g_pWindow)
        EXCEPT("failed to create window");
    else
        glfwMakeContextCurrent(g_pWindow);

    glfwSetKeyCallback(g_pWindow, KeyCallback);
    glfwSetCursorPosCallback(g_pWindow, MouseCallback);
    glfwSetInputMode(g_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // initialize glew
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        EXCEPT("failed to initialize GLEW");

    // vsync & depth test enabling
    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
    RenderInit();
}

static void ProgramLoop()
{
    double startTime;
    static float frameTime = 0.0f;

    while (!glfwWindowShouldClose(g_pWindow))
    {
        startTime = glfwGetTime();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();

        ProcessInput(frameTime);
        RenderFrame(frameTime);

        glfwSwapBuffers(g_pWindow);
        frameTime = (float)(glfwGetTime() - startTime);
    }
}

static void ProgramTerminate()
{
    glfwTerminate();
#ifdef FREEIMAGE_LIB
    FreeImage_DeInitialise();
#endif
}

int main()
{
    try {
        ProgramInit();
        ProgramLoop();
        ProgramTerminate();
        return 0;
    }
    catch (CException &ex)
    {
        std::cout << ex.GetFormattedMessage() << std::endl;
        std::cout << "Press any key to continue..." << std::endl;
        ProgramTerminate();
        getchar();
        return 1;
    }
}