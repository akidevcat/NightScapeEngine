#ifndef NSE_LIGHTING_INCLUDED
#define NSE_LIGHTING_INCLUDED

struct Light
{
    float3 PositionCS;
    float Intensity;
    float4 Color;
};

cbuffer LightsProperties
{
    int     _GlobalLightCount;
    float3  _GlobalLightParams;
    float4  _GlobalAmbientLightColor;
    Light   _GlobalLights[8];
};

float4 GetBasicLighting(float3 positionCS, float3 normalRS)
{
    float4 result = 0;

    for (int i = 0; i < _GlobalLightCount; i++)
    {
        float3 deltaPosition = _GlobalLights[i].PositionCS - positionCS;

        float ndotl = dot(normalRS, normalize(deltaPosition));
        ndotl = ndotl * 0.5 + 0.5;

        result += (_GlobalLights[i].Intensity / (dot(deltaPosition, deltaPosition) + _GlobalLights[i].Color.w)) * float4(_GlobalLights[i].Color.xyz, 1.0) * ndotl;
    }

// ToDo
    result = round(result * 16.0) / 16.0;

    return result;
}

#endif