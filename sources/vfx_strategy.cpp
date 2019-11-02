#include "vfx_strategy.h"

void CVFXBaseStrategy::Initialize(CVFXGenerator *generator)
{
    m_pGenerator = generator;
    SetupParameters();
    m_pGenerator->UpdateRefPointsBuffer();
}

float CVFXBaseStrategy::GetGlobalSpeed() const
{
    return m_fGlobalSpeed;
}

float CVFXBaseStrategy::GetSpeedFadeFactor() const
{
    return m_fSpeedFadeFactor;
}

vfx_pixel_t CVFXBaseStrategy::GetPixelValue() const
{
    return m_iPixelValue;
}

const size_t &CVFXBaseStrategy::GetPointsCount() const
{
    return m_iPointsCount;
}

const size_t &CVFXBaseStrategy::GetPaletteIndex() const
{
    return m_iPaletteIndex;
}



/*
*   EFFECT TYPE #1
*/
void CVFXStrategyType1::SetupParameters()
{
    m_iPaletteIndex     = 0;
    m_iPixelValue       = 0x7F;
    m_iPointsCount      = 30;
    m_fGlobalSpeed      = 370.0f;
    m_fSpeedFadeFactor  = 0.7099f;
}

void CVFXStrategyType1::RunFrameAlgorithm(float frameTime)
{
    static uint64_t frameCount = 0;
    if (frameCount % 2 == 0)
    {
        m_pGenerator->ApplyMapBlur(0, 127);
        m_pGenerator->ProjectMapPoints();
        m_pGenerator->ExpandMapPoints(15);
    }
    ++frameCount;
}



/*
*   EFFECT TYPE #2
*/
void CVFXStrategyType2::SetupParameters()
{
    m_iPaletteIndex     = 0;
    m_iPixelValue       = 0xB2;
    m_iPointsCount      = 96;
    m_fGlobalSpeed      = 1340.0f;
    m_fSpeedFadeFactor  = 0.8f;
}

void CVFXStrategyType2::RunFrameAlgorithm(float frameTime)
{
    static uint64_t frameCount = 0;
    if (frameCount % 2 == 0)
    {
        m_pGenerator->DrawMapBlinkingField(0x52, 0, 70);
        m_pGenerator->ApplyMapBlur(0, 0, false);
        m_pGenerator->ProjectMapPoints();
    }
    ++frameCount;
}



/*
*   EFFECT TYPE #3
*/
void CVFXStrategyType3::SetupParameters()
{
    m_iPaletteIndex     = 4;
    m_iPixelValue       = 0x7F;
    m_iPointsCount      = 96;
    m_fGlobalSpeed      = 1100.0f;
    m_fSpeedFadeFactor  = 1.f;
}

void CVFXStrategyType3::RunFrameAlgorithm(float frameTime)
{
    static uint64_t frameCount = 0;
    if (frameCount % 2 == 0)
    {
        m_pGenerator->DrawMapBlinkingField(0x7F, 0, 80);
        m_pGenerator->ApplyMapFading(1, 0);
        m_pGenerator->ProjectMapPoints();
    }
    ++frameCount;
}



/*
*   EFFECT TYPE #4
*/
void CVFXStrategyType4::SetupParameters()
{
    m_iPaletteIndex     = 4;
    m_iPixelValue       = 0xFF;
    m_iPointsCount      = 20;
    m_fGlobalSpeed      = 900.0f;
    m_fSpeedFadeFactor  = 0.8f;
}

void CVFXStrategyType4::RunFrameAlgorithm(float frameTime)
{
    static uint64_t frameCount = 0;
    if (frameCount % 2 == 0)
    {
        m_pGenerator->DrawMapBlinkingField(0x7F, 0xFD, 50);
        m_pGenerator->ApplyMapBlur(119, 126);
        m_pGenerator->ProjectMapPoints();
        m_pGenerator->ExpandMapPoints(3);
    }
    ++frameCount;
}