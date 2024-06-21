#ifndef LIGHTSPROPERTIES_H
#define LIGHTSPROPERTIES_H

#include <directxmath.h>

struct LightData
{
    DirectX::XMFLOAT3 PositionCS;
    float Intensity;
    DirectX::XMFLOAT4 Color;
};

struct LightsProperties
{
    int LightCount;
    DirectX::XMFLOAT3 LightParams;
    DirectX::XMFLOAT4 AmbientLightColor;
    LightData Lights[8];
};

#endif //LIGHTSPROPERTIES_H
