#pragma once
#include <glew.h>
#include <glfw/glfw3.h>
#include <stdint.h>
#include <string>

class CTexture
{
public:
    CTexture();
    ~CTexture();

    void Initialize(size_t width, size_t height, 
        size_t colorDepth, GLuint format);
    bool LoadFromFile(std::string filePath);
    void StartUpdate(uint8_t *&textureBuffer);
    void StopUpdate();
    void CreatePBO();
    void Bind() const;

    size_t GetWidth() const;
    size_t GetHeight() const;
    size_t GetColorDepth() const;

private:
    void CreateObject();
    void Generate(uint8_t *buffer);
    void UpdateGLFormat();

private:
    size_t m_iWidth;
    size_t m_iHeight;
    size_t m_iColorDepth;
    size_t m_iTotalSize;

    GLuint m_glFormat;
    GLuint m_glHandle;
    GLuint m_glPBO;
};

