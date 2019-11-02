#include "texture.h"
#include <FreeImage.h>

CTexture::CTexture()
{
    m_glPBO = 0;
    m_glHandle = 0;
    m_glFormat = 0;
    m_iWidth = 0;
    m_iHeight = 0;
    m_iTotalSize = 0;
    m_iColorDepth = 0;
}

CTexture::~CTexture()
{
    glDeleteTextures(1, &m_glHandle);
    glDeleteBuffers(1, &m_glPBO);
}

void CTexture::CreateObject()
{
    if (!m_glHandle)
        glGenTextures(1, &m_glHandle);
    else
    {
        glDeleteTextures(1, &m_glHandle);
        m_glHandle = 0;
        CreateObject();
    }
}

void CTexture::Initialize(size_t width, size_t height, 
    size_t colorDepth, GLuint format)
{
    uint8_t *imageBuffer;
    m_iWidth        = width;
    m_iHeight       = height;
    m_iColorDepth   = colorDepth;
    m_glFormat      = format;
    m_iTotalSize    = m_iWidth * m_iHeight * (m_iColorDepth / 8);

    imageBuffer = new uint8_t[m_iTotalSize];
    Generate(imageBuffer);
    delete[] imageBuffer;
}

bool CTexture::LoadFromFile(std::string filePath)
{
    FIBITMAP *image;
    size_t imagePitch;
    uint8_t *imageBuffer;
    const char *szFilePath;
    FREE_IMAGE_FORMAT imageFormat;

    szFilePath = filePath.c_str();
    imageFormat = FreeImage_GetFileType(szFilePath);
    if (imageFormat == FIF_UNKNOWN)
    {
        imageFormat = FreeImage_GetFIFFromFilename(szFilePath);
        if (imageFormat == FIF_UNKNOWN)
            return false;
    }

    if (FreeImage_FIFSupportsReading(imageFormat))
         image = FreeImage_Load(imageFormat, szFilePath);
    else
        return false;

    if (!image)
        return false;

    m_iWidth = FreeImage_GetWidth(image);
    m_iHeight = FreeImage_GetHeight(image);
    m_iColorDepth = FreeImage_GetBPP(image);

    if (m_iColorDepth < 24)
        m_iColorDepth = 24;

    m_iTotalSize = m_iWidth * m_iHeight * (m_iColorDepth / 8);

    imagePitch = FreeImage_GetPitch(image);
    imageBuffer = new uint8_t[m_iTotalSize];

    FreeImage_ConvertToRawBits(
        imageBuffer, image, imagePitch, m_iColorDepth, 
        FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK
    );
    UpdateGLFormat();
    Generate(imageBuffer);

    FreeImage_Unload(image);
    delete[] imageBuffer;
    return true;
}

void CTexture::StartUpdate(uint8_t *&textureBuffer)
{
    if (!m_glPBO)
        return;

    Bind();
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_glPBO);

    // Note that glMapBuffer() causes sync issue.
    // If GPU is working with this buffer, glMapBuffer() will wait(stall)
    // until GPU to finish its job. To avoid waiting (idle), you can call
    // first glBufferData() with NULL pointer before glMapBuffer().
    // If you do that, the previous data in PBO will be discarded and
    // glMapBuffer() returns a new allocated pointer immediately
    // even if GPU is still working with the previous data.
    glBufferData(GL_PIXEL_UNPACK_BUFFER, m_iTotalSize, 0, GL_STREAM_DRAW);
    textureBuffer = (uint8_t*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
}

void CTexture::StopUpdate()
{
    if (!m_glPBO)
        return;

    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

    // copy data from PBO to texture
    glTexSubImage2D(
        GL_TEXTURE_2D, 0, 0, 0, m_iWidth, 
        m_iHeight, m_glFormat, GL_UNSIGNED_BYTE, 0
    );

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void CTexture::CreatePBO()
{
    if (!m_glPBO)
        glCreateBuffers(1, &m_glPBO);
}

void CTexture::Bind() const
{
    if (m_glHandle)
        glBindTexture(GL_TEXTURE_2D, m_glHandle);
}

size_t CTexture::GetWidth() const
{
    return m_iWidth;
}

size_t CTexture::GetHeight() const
{
    return m_iHeight;
}

size_t CTexture::GetColorDepth() const
{
    return m_iColorDepth;
}

void CTexture::Generate(uint8_t *buffer)
{
    GLint internalFormat;
    const bool mipmapEnabled = false;

    if (m_iColorDepth == 32)
        internalFormat = GL_RGBA;
    else
        internalFormat = GL_RGB;

    CreateObject();
    glBindTexture(GL_TEXTURE_2D, m_glHandle);
    glTexImage2D(
        GL_TEXTURE_2D, 0, internalFormat, m_iWidth, 
        m_iHeight, 0, m_glFormat, GL_UNSIGNED_BYTE, buffer
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (mipmapEnabled)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void CTexture::UpdateGLFormat()
{
    /*
    FreeImage documentation note:
    However, the pixel layout used by this model is OS dependant. 
    Using a byte by byte memory order to label the pixel layout, then 
    FreeImage uses a BGR[A] pixel layout under a Little Endian 
    processor (Windows, Linux) and uses a RGB[A] pixel layout 
    under a Big Endian processor (Mac OS X or any Big Endian Linux / Unix). 
    This choice was made to ease the use of FreeImage with graphics API.
    */

    if (m_iColorDepth == 32)
        m_glFormat = GL_BGRA;
    else
        m_glFormat = GL_BGR;
}
