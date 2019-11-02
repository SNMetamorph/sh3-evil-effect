#include "vfx_refpoint.h"

static float GetRandomValue()
{
    // returns value from range [0.0; 1.0]
    return ((float)rand() / RAND_MAX);
}

static float GetRandomValue2()
{
    int a = rand();
    int b = rand();
    int c = (rand() % 2) << 15;
    int d = (b | c) << 15;
    return (float)((a | d) * 4.6566129e-10);
}

vfx_refpoint_t::vfx_refpoint_t()
{
    velocityX = 0;
    velocityY = 0;
    originX = GetRandomValue();
    originY = GetRandomValue();
    radius = (GetRandomValue2() * 0.8f + 0.2f) * 6.0f;
    speedFactor = 1.0f / ((GetRandomValue2() * 0.6f + 0.4f) * 16.0f);
}
