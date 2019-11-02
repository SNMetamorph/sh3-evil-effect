#pragma once
#include "shader.h"

class CShaderProgram
{
public:
    CShaderProgram();
    ~CShaderProgram();

    void AttachShader(const CShader &shader);
    void Link();
    void Use();
    GLuint GetHandle() const;

private:
    GLuint m_glHandle;
};

