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

float4 GetBasicLighting(float3 positionCS, float3 normalRS, uint2 screenPos)
{
    float4 result = 0;

    for (int i = 0; i < _GlobalLightCount; i++)
    {
        float3 deltaPosition = _GlobalLights[i].PositionCS - positionCS;
//         deltaPosition = float3(0,0,0) - positionCS;

        float ndotl = dot(normalRS, normalize(deltaPosition));
//         ndotl = 1;
//         ndotl = ndotl * 0.5 + 0.5;

//         ndotl = 1;

        float intensity = (_GlobalLights[i].Intensity / (dot(deltaPosition, deltaPosition) + _GlobalLights[i].Color.w)) * ndotl;

        intensity = Dither(intensity, screenPos);

        result += intensity * float4(_GlobalLights[i].Color.xyz, 1.0);
    }

// ToDo
//     result = round(result * 16.0) / 16.0;

    return result;
}

#endif