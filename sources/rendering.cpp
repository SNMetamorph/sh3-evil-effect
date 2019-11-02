#include "rendering.h"
#include "scene_object.h"
#include "shader_program.h"
#include "exception.h"
#include "camera.hpp"
#include "vfx_generator.h"

#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int g_iScreenWidth;
int g_iScreenHeight;
Camera g_Camera;
glm::mat4 g_matProjection;
extern GLFWwindow *g_pWindow;

CTexture g_TextureDiffuse;
CTexture g_TextureEffect;
CSceneObject g_SceneObject;
CShaderProgram g_ShaderProgram;

CVFXGenerator g_VFXGenerator;
CVFXStrategyType1 g_VFXType1;
CVFXStrategyType2 g_VFXType2;
CVFXStrategyType3 g_VFXType3;
CVFXStrategyType4 g_VFXType4;

void RenderInit()
{
    float aspectRatio;
    CShader vertexShader;
    CShader fragmentShader;

    const float fovAngle  = 45.0f;
    const float nearZ     = 0.1f;
    const float farZ      = 100.0f;

    // create projection matrix
    glfwGetFramebufferSize(g_pWindow, &g_iScreenWidth, &g_iScreenHeight);
    glViewport(0, 0, g_iScreenWidth, g_iScreenHeight);
    aspectRatio = (float)g_iScreenWidth / (float)g_iScreenHeight;
    g_matProjection = glm::perspective(fovAngle, aspectRatio, nearZ, farZ);

    // load scene object
    if (!g_SceneObject.LoadMeshOBJ("../../models/room/room.obj"))
        EXCEPT("failed to load OBJ model file");

    g_SceneObject.SetOrigin(0.f, 0.f, 0.f);
    //g_SceneObject.SetAngles(90.f, 0.f, 0.f);

    // load object texture
    g_TextureDiffuse.LoadFromFile("../../models/room/room.jpg");

    g_VFXGenerator.Initialize(128, 128);
    g_VFXGenerator.SetGenerationStrategy(&g_VFXType1);
    g_TextureEffect.Initialize(128, 128, 32, GL_RGBA);
    g_TextureEffect.CreatePBO();

    g_SceneObject.AddTexture(&g_TextureDiffuse);
    g_SceneObject.AddTexture(&g_TextureEffect);

    // initializing shaders
    vertexShader.Initialize(GL_VERTEX_SHADER);
    vertexShader.LoadFromFile("../../shaders/vertex.vert");
    if (!vertexShader.Compile())
        EXCEPT("failed to compile vertex shader");
    else
        g_ShaderProgram.AttachShader(vertexShader);

    fragmentShader.Initialize(GL_FRAGMENT_SHADER);
    fragmentShader.LoadFromFile("../../shaders/fragment.frag");
    if (!fragmentShader.Compile())
        EXCEPT("failed to compile fragment shader");
    else
        g_ShaderProgram.AttachShader(fragmentShader);

    g_ShaderProgram.Link();
}

static void UpdateEffectTexture()
{
    size_t textureWidth;
    size_t textureHeight;
    uint8_t *textureBuffer;

    textureWidth = g_TextureEffect.GetWidth();
    textureHeight = g_TextureEffect.GetHeight();
    g_TextureEffect.StartUpdate(textureBuffer);
    g_VFXGenerator.ConvertToRGBA(
        textureBuffer, textureWidth, textureHeight
    );
    g_TextureEffect.StopUpdate();
}

void RenderFrame(float frameTime)
{
    GLint uMatModel = glGetUniformLocation(g_ShaderProgram.GetHandle(), "matModel");
    GLint uMatView = glGetUniformLocation(g_ShaderProgram.GetHandle(), "matView");
    GLint uMatProjection = glGetUniformLocation(g_ShaderProgram.GetHandle(), "matProjection");
    GLint uCameraPos = glGetUniformLocation(g_ShaderProgram.GetHandle(), "cameraPos");
    GLint uTexDiffuse = glGetUniformLocation(g_ShaderProgram.GetHandle(), "texDiffuse");
    GLint uTexEffect = glGetUniformLocation(g_ShaderProgram.GetHandle(), "texEffect");

    glUniformMatrix4fv(uMatModel, 1, GL_FALSE, glm::value_ptr(g_SceneObject.GetModelMatrix()));
    glUniformMatrix4fv(uMatView, 1, GL_FALSE, glm::value_ptr(g_Camera.GetViewMatrix()));
    glUniformMatrix4fv(uMatProjection, 1, GL_FALSE, glm::value_ptr(g_matProjection));
    glUniform3f(uCameraPos, g_Camera.Position.x, g_Camera.Position.y, g_Camera.Position.z);
    glUniform1i(uTexDiffuse, 0);
    glUniform1i(uTexEffect, 1);

    g_ShaderProgram.Use();
    g_VFXGenerator.UpdateFrameState(frameTime);
    UpdateEffectTexture();
    g_SceneObject.Draw();
}
