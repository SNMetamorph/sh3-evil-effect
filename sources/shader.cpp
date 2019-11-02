#include "shader.h"
#include <stdio.h>

CShader::CShader()
{
    m_glHandle = 0;
    m_sLogString.reserve(m_iLogLength);
}

CShader::~CShader()
{
    Dispatch();
}

void CShader::Initialize(GLenum shaderType)
{
    if (!m_glHandle)
        m_glHandle = glCreateShader(shaderType);
}

bool CShader::LoadFromFile(std::string filePath)
{
    int codeLength;
    char *codeBuffer;
    FILE *shaderFile;

    if (!m_glHandle)
        return false;

    shaderFile = fopen(filePath.c_str(), "rb");
    if (!shaderFile)
        return false;

    fseek(shaderFile, 0, SEEK_END);
    codeLength = ftell(shaderFile);
    fseek(shaderFile, 0, SEEK_SET);

    if (!codeLength)
        return false;

    codeBuffer = new char[codeLength + 1];
    memset(codeBuffer, 0, codeLength + 1);
    fread(codeBuffer, codeLength, 1, shaderFile);
    glShaderSource(m_glHandle, 1, &codeBuffer, NULL);
    fclose(shaderFile);
    delete[] codeBuffer;

    return true;
}

bool CShader::Compile()
{
    if (!m_glHandle)
        return false;

    GLint isCompiled;
    glCompileShader(m_glHandle);
    glGetShaderiv(m_glHandle, GL_COMPILE_STATUS, &isCompiled);
    glGetShaderInfoLog(
        m_glHandle, m_iLogLength, 
        nullptr, &m_sLogString[0]
    );

    if (isCompiled)
        return true;
    else
        return false;
}

void CShader::Dispatch()
{
    if (m_glHandle)
        glDeleteShader(m_glHandle);
}

GLuint CShader::GetGLHandle() const
{
    return m_glHandle;
}
