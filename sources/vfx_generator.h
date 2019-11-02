#pragma once
#include "vfx_refpoint.h"
#include "vfx_strategy.h"
#include "vfx_pixel.h"
#include <stdint.h>
#include <vector>

class IVFXStrategy;
class CVFXGenerator
{
public:
    CVFXGenerator();
    ~CVFXGenerator();

    void Initialize(int mapWidth, int mapHeight);
    void UpdateFrameState(float frameTime);
    void SetGenerationStrategy(IVFXStrategy *strategy);
    void ConvertToRGBA(uint8_t *textureBuffer, 
        size_t textureWidth, size_t textureHeight);

    // methods for strategy
    void UpdateRefPointsBuffer();
    void ApplyMapFading(vfx_pixel_t fadeValue, vfx_pixel_t fadeMin);
    void ApplyMapBlur(uint8_t minValue, 
        uint8_t intensity, bool clampValue = true);
    void DrawMapBlinkingField(uint8_t pixelValue, 
        uint8_t baseValue, size_t passCount);
    void ExpandMapPoints(size_t passCount);
    void ProjectMapPoints();

private:
    vfx_pixel_t &PixelAt(float x, float y);
    inline vfx_pixel_t &PixelAt(int x, int y) {
        return m_PixelMap[(y * m_iMapWidth) + x];
    };

    void NormalizeOrigin(int &originX, int &originY);
    void UpdateRefPoints(float frameTime);
    void GetVelocityDelta(float &value1, float &value2);
    uint32_t GetRandomValue(bool updateSeed = true);
    float GetRadiusFactor();

private:
    int m_iMapWidth;
    int m_iMapHeight;
    IVFXStrategy *m_pStrategy;
    std::vector<vfx_pixel_t> m_PixelMap;
    std::vector<vfx_refpoint_t> m_RefPoints;
};

