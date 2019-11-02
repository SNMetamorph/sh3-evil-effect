#include "vfx_generator.h"
#include "vfx_palette.h"
#include <algorithm>


CVFXGenerator::CVFXGenerator()
{
    m_iMapWidth = 0;
    m_iMapHeight = 0;
    m_pStrategy = nullptr;
}

CVFXGenerator::~CVFXGenerator()
{
}

void CVFXGenerator::Initialize(int mapWidth, int mapHeight)
{
    m_iMapWidth = mapWidth;
    m_iMapHeight = mapHeight;
    m_PixelMap.resize(mapWidth * mapHeight, 0);
}

void CVFXGenerator::UpdateFrameState(float frameTime)
{
    if (!m_pStrategy)
        return;

    UpdateRefPoints(frameTime);
    m_pStrategy->RunFrameAlgorithm(frameTime);
}

void CVFXGenerator::SetGenerationStrategy(IVFXStrategy *strategy)
{
    m_pStrategy = strategy;
    m_pStrategy->Initialize(this);
}

void CVFXGenerator::UpdateRefPointsBuffer()
{
    if (m_pStrategy)
        m_RefPoints.resize(m_pStrategy->GetPointsCount());
}

void CVFXGenerator::ConvertToRGBA(uint8_t *textureBuffer, size_t textureWidth, size_t textureHeight)
{
    if (!textureBuffer)
        return;

    vfx_rgba32_t *colorDest = (vfx_rgba32_t*)textureBuffer;
    const size_t &paletteIndex = m_pStrategy->GetPaletteIndex();
    const vfx_palette_t &palette = g_PaletteArray[paletteIndex];

    for (size_t j = 0; j < textureHeight; ++j)
    {
        for (size_t i = 0; i < textureWidth; ++i)
        {
            size_t pixelIndex = (textureHeight * j) + i;
            const vfx_pixel_t &pixel = m_PixelMap[pixelIndex];
            const vfx_rgba32_t &colorSrc = palette.colors[pixel];

            *colorDest = colorSrc;
            ++colorDest;
        }
    }
}

void CVFXGenerator::UpdateRefPoints(float frameTime)
{
    float speedScale;
    float speedFadeDelta;
    float velocityDeltaX;
    float velocityDeltaY;

    for (vfx_refpoint_t &point : m_RefPoints)
    { 
        GetVelocityDelta(velocityDeltaX, velocityDeltaY);
        speedScale = m_pStrategy->GetGlobalSpeed() * point.speedFactor;
        speedFadeDelta = m_pStrategy->GetSpeedFadeFactor() * frameTime;

        // update point velocity
        velocityDeltaX *= speedScale * frameTime;
        velocityDeltaY *= speedScale * frameTime;
        point.velocityX += speedFadeDelta * -point.velocityX;
        point.velocityY += speedFadeDelta * -point.velocityY;
        point.velocityX += velocityDeltaX;
        point.velocityY += velocityDeltaY;

        /*
        these dividings to 128 applied for
        compatibility with SH3 coordinates scale
        in SH3 point coordinates belong to [0; map width (128)]
        but in this implementation decided to use [0; 1]
        */
        // update & check point origin
        point.originX += point.velocityX / 128.0f * frameTime;
        point.originY += point.velocityY / 128.0f * frameTime;

        // normalize origin
        if (point.originX < 0.0f)
            point.originX = 1.0f - fmodf(fabs(point.originX), 1.0f);
        else
            point.originX = fmodf(point.originX, 1.0f);

        if (point.originY < 0.0f)
            point.originY = 1.0f - fmodf(fabs(point.originY), 1.0f);
        else
            point.originY = fmodf(point.originY, 1.0f);
    }
}

void CVFXGenerator::ApplyMapFading(vfx_pixel_t fadeValue, vfx_pixel_t fadeMin)
{
    for (vfx_pixel_t &pixel : m_PixelMap)
    {
        if (fadeValue >= pixel)
            pixel = 0;
        else
            pixel -= fadeValue;

        if (pixel < fadeMin)
            pixel = fadeMin;
    }
}

void CVFXGenerator::ExpandMapPoints(size_t passCount)
{
    float deltaX, deltaY;
    float newPointX, newPointY;
    int pixelX, pixelY;

    for (const vfx_refpoint_t &point : m_RefPoints)
    {
        for (size_t i = 0; i < passCount; ++i)
        {
            deltaX = (GetRadiusFactor() * point.radius) / 128.0f;
            deltaY = (GetRadiusFactor() * point.radius) / 128.0f;
            newPointX = point.originX + deltaX;
            newPointY = point.originY + deltaY;

            pixelX = (int)(newPointX * (m_iMapWidth - 1));
            pixelY = (int)(newPointY * (m_iMapHeight - 1));
            NormalizeOrigin(pixelX, pixelY);
            PixelAt(pixelX, pixelY) = m_pStrategy->GetPixelValue();
        }
    }
}

/*
in debug build configuration this method
causes huge frame execution time impact
for unknown reasons.
in release config - everything is ok.
*/
void CVFXGenerator::ApplyMapBlur(uint8_t minValue, uint8_t intensity, bool clampValue)
{
    int pixelValue;
    int neighborSum;
    vfx_pixel_t currentPixel;
    int pixelPrev, pixelNext;
    static std::vector<int> accumulator;

    if (accumulator.size() != m_iMapWidth)
        accumulator.resize(m_iMapWidth);

    for (int j = 0; j < m_iMapHeight; ++j)
    {
        pixelPrev = j - 1;
        pixelNext = j + 1;

        if (pixelPrev < 0)
            pixelPrev = m_iMapWidth - 1;
        else if (pixelNext >= m_iMapWidth)
            pixelNext %= m_iMapWidth;

        for (int i = 0; i < m_iMapWidth; ++i)
        {
            int &sumValue = accumulator[i];
            sumValue = 0;
            sumValue += PixelAt(i, pixelPrev);
            sumValue += PixelAt(i, j);
            sumValue += PixelAt(i, pixelNext);
        }

        for (int i = 0; i < m_iMapWidth; ++i)
        {
            pixelPrev = i - 1;
            pixelNext = i + 1;

            if (pixelPrev < 0)
                pixelPrev = m_iMapWidth - 1;
            else if (pixelNext >= m_iMapWidth)
                pixelNext %= m_iMapWidth;

            currentPixel = PixelAt(i, j);
            neighborSum = accumulator[i];
            neighborSum += accumulator[pixelPrev];
            neighborSum += accumulator[pixelNext];

            neighborSum = (neighborSum - currentPixel) / 8;
            neighborSum += currentPixel;
            neighborSum /= 2;

            if (clampValue)
            {
                pixelValue = intensity * (neighborSum - minValue);
                pixelValue /= 128;
                pixelValue += minValue;
            }
            else
                pixelValue = neighborSum;

            if (pixelValue > 0)
            {
                if (pixelValue > 255)
                    pixelValue = 255;
            }
            else
                pixelValue = 0;

            PixelAt(i, j) = pixelValue;
        }
    }
}

void CVFXGenerator::DrawMapBlinkingField(uint8_t pattern, uint8_t baseValue, size_t passCount)
{
    int offsetX;
    int offsetY;
    vfx_pixel_t pixelValue;

    for (size_t i = 0; i < passCount; ++i)
    {
        offsetX = (GetRandomValue() >> 24) % m_iMapWidth;
        offsetY = (GetRandomValue() >> 24) % m_iMapHeight;
        pixelValue = (GetRandomValue() >> 23) % (pattern + 1);
        PixelAt(offsetX, offsetY) = baseValue | pixelValue;
    }
}

void CVFXGenerator::ProjectMapPoints()
{
    for (const vfx_refpoint_t &point : m_RefPoints)
        PixelAt(point.originX, point.originY) = m_pStrategy->GetPixelValue();
}

vfx_pixel_t &CVFXGenerator::PixelAt(float x, float y)
{
    int pixelX;
    int pixelY;
    int pixelIndex;

    pixelX = (int)(x * (m_iMapWidth - 1));
    pixelY = (int)(y * (m_iMapHeight - 1));
    NormalizeOrigin(pixelX, pixelY);
    pixelIndex = (pixelY * m_iMapWidth) + pixelX;
    return m_PixelMap[pixelIndex];
}

void CVFXGenerator::NormalizeOrigin(int &originX, int &originY)
{
    int mapWidth = m_iMapWidth - 1;
    int mapHeight = m_iMapHeight - 1;

    if (originX > mapWidth)
        originX %= m_iMapWidth;
    else if (originX < 0)
        originX = mapWidth - (-originX % m_iMapWidth);

    if (originY > mapHeight)
        originY %= m_iMapHeight;
    else if (originY < 0)
        originY = mapHeight - (-originY % m_iMapHeight);
}

void CVFXGenerator::GetVelocityDelta(float &value1, float &value2)
{
    // algorithm in this method is fully similar to SH3 algorithm
    int a, b, c;

    a = (rand() % 2) << 15;
    b = (rand() | a) << 15;
    c = rand() | b;
    value1 = (float)((double)c * 9.3132257e-10 - 1.0);

    a = (rand() % 2) << 15;
    b = (rand() | a) << 15;
    c = rand() | b;
    value2 = (float)((double)c * 9.3132257e-10 - 1.0);
}

float CVFXGenerator::GetRadiusFactor()
{
    double totalSum = 0.0;
    for (size_t i = 0; i < 3; ++i)
        totalSum += (double)GetRandomValue();

    return (float)(totalSum * 4.6566129e-10);
}

uint32_t CVFXGenerator::GetRandomValue(bool updateSeed)
{
    // linear congruental method (Numerical Repices)
    // same consts used in according function in SH3
    static uint32_t seed = 5;
    const uint32_t a = 1664525;
    const uint32_t c = 1013904223;

    uint32_t value = a * seed + c;
    if (updateSeed)
        seed = value;

    return value;
}
