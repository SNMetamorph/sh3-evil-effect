#include "scene_object.h"
#include "OBJ_Loader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


CSceneObject::CSceneObject()
{
    m_vecAngles.p = 0.0f;
    m_vecAngles.y = 0.0f;
    m_vecAngles.r = 0.0f;
    m_vecOrigin.x = 0.0f;
    m_vecOrigin.y = 0.0f;
    m_vecOrigin.z = 0.0f;
    m_iTextureCount = 0;
}

CSceneObject::~CSceneObject()
{
}

bool CSceneObject::LoadMeshOBJ(std::string filePath)
{
    size_t indexListSize;
    size_t vertexListSize;
    objl::Loader objLoader;

    bool isFileLoaded = objLoader.LoadFile(filePath);
    if (!isFileLoaded)
        return false;

    // HACK: use first loaded mesh
    objl::Mesh &mesh = objLoader.LoadedMeshes[0];

    vertexListSize = mesh.Vertices.size() * sizeof(mesh.Vertices[0]);
    indexListSize = mesh.Indices.size() * sizeof(mesh.Indices[0]);
    m_iIndicesCount = mesh.Indices.size();

    // create object buffers
    glGenVertexArrays(1, &m_glVAO);
    glGenBuffers(1, &m_glVBO);
    glGenBuffers(1, &m_glEBO);

    glBindVertexArray(m_glVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_glVBO);
    glBufferData(
        GL_ARRAY_BUFFER, vertexListSize, mesh.Vertices.data(), GL_STATIC_DRAW);

    // vertex origin
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // vertex normal
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // vertex texture coordinates
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glEBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, indexListSize, 
        &mesh.Indices[0], GL_STATIC_DRAW
    );

    glBindVertexArray(0);
    return true;
}

void CSceneObject::AddTexture(CTexture *texture)
{
    if (m_iTextureCount < 32)
        m_pTextures[m_iTextureCount++] = texture;
}

void CSceneObject::Draw()
{
    BindTextures();
    glBindVertexArray(m_glVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glEBO);
    glDrawElements(GL_TRIANGLES, m_iIndicesCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

const glm::mat4 &CSceneObject::GetModelMatrix()
{
    CalcModelMatrix();
    return m_matModel;
}

const glm::vec3 &CSceneObject::GetOrigin()
{
    return m_vecOrigin;
}

const glm::vec3 &CSceneObject::GetAngles()
{
    return m_vecAngles;
}

void CSceneObject::SetOrigin(float x, float y, float z)
{
    m_vecOrigin.x = x;
    m_vecOrigin.y = y;
    m_vecOrigin.z = z;
}

void CSceneObject::SetAngles(float pitch, float yaw, float roll)
{
    m_vecAngles.p = pitch;
    m_vecAngles.y = yaw;
    m_vecAngles.r = roll;
}

void CSceneObject::BindTextures()
{
    for (int i = 0; i < m_iTextureCount; ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        m_pTextures[i]->Bind();
    }
}

void CSceneObject::CalcModelMatrix()
{
    float &anglePitch   = m_vecAngles.p;
    float &angleYaw     = m_vecAngles.y;
    float &angleRoll    = m_vecAngles.r;

    m_matModel = glm::rotate(
        glm::mat4(1.0f), glm::degrees(anglePitch), glm::vec3(1.0f, 0.0f, 0.0f));
    m_matModel = glm::rotate(
        m_matModel, glm::degrees(angleYaw), glm::vec3(0.0f, 1.0f, 0.0f));
    m_matModel = glm::rotate(
        m_matModel, glm::degrees(angleRoll), glm::vec3(0.0f, 0.0f, 1.0f));
    m_matModel = glm::translate(m_matModel, m_vecOrigin);
}

