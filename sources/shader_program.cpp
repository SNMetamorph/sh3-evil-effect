#include "shader_program.h"

CShaderProgram::CShaderProgram()
{
    m_glHandle = 0;
}

CShaderProgram::~CShaderProgram()
{
}

void CShaderProgram::AttachShader(const CShader &shader)
{
    if (!m_glHandle)
        m_glHandle = glCreateProgram();
    glAttachShader(m_glHandle, shader.GetGLHandle());
}

void CShaderProgram::Link()
{
    if (m_glHandle)
        glLinkProgram(m_glHandle);
}

void CShaderProgram::Use()
{
    if (m_glHandle)
        glUseProgram(m_glHandle);
}

GLuint CShaderProgram::GetHandle() const
{
    return m_glHandle;
}