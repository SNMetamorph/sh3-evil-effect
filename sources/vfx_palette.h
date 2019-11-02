#pragma once
#include <stdint.h>

struct vfx_rgba32_t
{
    uint8_t r, g, b, a;
};

struct vfx_palette_t
{
    union
    {
        vfx_rgba32_t colors[256];
        uint8_t pixelData[256 * sizeof(vfx_rgba32_t)];
    };
};

extern vfx_palette_t g_PaletteArray[];