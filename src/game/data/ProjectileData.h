#pragma once
#include "../../engine/math/Math.h"

struct ProjectileData
{
    NSE::Vector3d position;
    float3 velocity;
    float age;
    float energy;
};
