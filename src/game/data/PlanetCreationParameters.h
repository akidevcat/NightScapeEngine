#pragma once

#include "../../engine/addon/FastNoiseLite.h"

struct PlanetCreationParameters
{
    int seed = 0;
    float radius = 10000.0f;
    float terrainMaxHeight = 4000.0f;
    float atmosphereHeight = 4000.0f;
    FastNoiseLite noise;
    float4 primaryColor = {1,1,1,1};
};