#pragma once
#include "texture.h"

#define GLEW_STATIC
#include <glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <string>
#include <stdint.h>

class CSceneObject
{
public:
    CSceneObject();
    ~CSceneObject();

    bool LoadMeshOBJ(std::string filePath);
    void AddTexture(CTexture *texture);
    void Draw();

    const glm::mat4 &GetModelMatrix();
    const glm::vec3 &GetOrigin();
    const glm::vec3 &GetAngles();

    void SetOrigin(float x, float y, float z);
    void SetAngles(float pitch, float yaw, float roll);

private:
    void BindTextures();
    void CalcModelMatrix();

private:
    GLuint  m_glVAO;
    GLuint  m_glVBO;
    GLuint  m_glEBO;
    size_t  m_iIndicesCount;
    int     m_iTextureCount;
    CTexture *m_pTextures[32];

    glm::vec3 m_vecOrigin;
    glm::vec3 m_vecAngles;
    glm::mat4 m_matModel;
};