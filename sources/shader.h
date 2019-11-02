#pragma once
#define GLEW_STATIC
#include <glew.h>
#include <glfw/glfw3.h>
#include <string>

class CShader
{
public:
    friend class CShaderProgram;

    CShader();
    ~CShader();

    void Initialize(GLenum shaderType);
    bool LoadFromFile(std::string filePath);
    bool Compile();

private:
    void Dispatch();
    GLuint GetGLHandle() const;

private:
    const int m_iLogLength = 512;
    std::string m_sLogString;
    GLuint m_glHandle;
};

