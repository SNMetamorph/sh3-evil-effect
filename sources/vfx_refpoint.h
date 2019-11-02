#pragma once
#include <stdint.h>
#include <stdlib.h>

struct vfx_refpoint_t
{
    vfx_refpoint_t();

    float originX;
    float originY;
    float velocityX;
    float velocityY;
    float radius;
    float speedFactor;
};
