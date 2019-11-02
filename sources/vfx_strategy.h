#pragma once
#include "vfx_generator.h"
#include "vfx_pixel.h"

class CVFXGenerator;
class IVFXStrategy
{
public:
    virtual void Initialize(CVFXGenerator *generator) = 0;
    virtual void RunFrameAlgorithm(float frameTime) = 0;

    virtual float GetGlobalSpeed() const = 0;
    virtual float GetSpeedFadeFactor() const = 0;
    virtual vfx_pixel_t GetPixelValue() const = 0;
    virtual const size_t &GetPointsCount() const = 0;
    virtual const size_t &GetPaletteIndex() const = 0;
};

class CVFXBaseStrategy : public IVFXStrategy
{
public:
    virtual void Initialize(CVFXGenerator *generator) override;
    virtual void RunFrameAlgorithm(float frameTime) = 0;
    virtual void SetupParameters() = 0;

    float GetGlobalSpeed() const;
    float GetSpeedFadeFactor() const;
    vfx_pixel_t GetPixelValue() const;
    const size_t &GetPointsCount() const;
    const size_t &GetPaletteIndex() const;

protected:
    size_t m_iPointsCount;
    size_t m_iPaletteIndex;
    float m_fGlobalSpeed;
    float m_fSpeedFadeFactor;
    vfx_pixel_t m_iPixelValue;
    CVFXGenerator *m_pGenerator;
};

class CVFXStrategyType1 : public CVFXBaseStrategy
{
public:
    void SetupParameters() override;
    void RunFrameAlgorithm(float frameTime) override;
};

class CVFXStrategyType2 : public CVFXBaseStrategy
{
public:
    void SetupParameters() override;
    void RunFrameAlgorithm(float frameTime) override;
};

class CVFXStrategyType3 : public CVFXBaseStrategy
{
public:
    void SetupParameters() override;
    void RunFrameAlgorithm(float frameTime) override;
};

class CVFXStrategyType4 : public CVFXBaseStrategy
{
public:
    void SetupParameters() override;
    void RunFrameAlgorithm(float frameTime) override;
};