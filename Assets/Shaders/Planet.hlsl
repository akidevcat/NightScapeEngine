#include "ShaderLibrary/Core.hlsl"
#include "ShaderLibrary/Lighting.hlsl"
#include "ShaderLibrary/Noise.hlsl"

cbuffer ChunkDrawBuffer
{
    float _ChunkScaling;
    float3 _ChunkPosition;
};

float _PlanetRadius;
float _AtmosphereRadius;
float4 _PrimaryColor;

DefaultPixelInput VertexMain(DefaultVertexInput input)
{
    DefaultPixelInput output;

    output.position = TransformObjectToClip(input.position);
    output.positionRS = float4(input.position, 1);
    output.normal = TransformObjectToClipDirection(input.normal);
    output.normalRS = TransformObjectToWorldDirection(input.normal);
    output.uv = input.uv;

    return output;
}

float4 GetAtmosphericScattering(float3 planetOrigin, float planetRadius, float atmosphereRadius, float3 rayOrigin, float3 rayDirection, float rayLength)
{
    const uint sampleCount = 6;
    float3 samplePos = rayOrigin;
    float sampleLength = rayLength / sampleCount;

    float camHeight = max(length(rayOrigin - planetOrigin) - planetRadius, 0.0) /  (atmosphereRadius - planetRadius);
    camHeight = saturate(camHeight);

    float atmAccumDensity = 0.0f;
    float atmTransmittance = 1.0f;
    float3 atmAccumLightEnergy = 0;

    for (int i = 0; i < sampleCount; i++)
    {
        float height = 1.0 - max(length(samplePos - planetOrigin) - planetRadius, 0.0) / (atmosphereRadius - planetRadius);
        height = saturate(height);

        float sampleDensity = height * 0.3;

        atmAccumDensity += sampleDensity * sampleLength;

        float3 absorbedLight = float3(0.506, 0.835, 1) * (exp(sampleLength)) * 0.1;

        // light correction by height
//         absorbedLight *= 1.0 + (1.0 - camHeight) * 10.0;

        atmAccumLightEnergy += atmTransmittance * absorbedLight;

        samplePos += rayDirection * sampleLength;
        atmTransmittance *= saturate(1.0 - sampleDensity);
    }



    return float4(atmAccumLightEnergy, 1.0 - atmTransmittance);
}

float4 PixelMain(DefaultPixelInput input) : SV_TARGET
{
    float2 uv = input.uv;
    uint2 screenPos = GetScreenPos(input.position.xy);

    float3 positionWS = TransformObjectToWorld(input.positionRS.xyz);
    float camDistance = length(positionWS) * _ChunkScaling;

//     float4 result = float4(0.804, 0.541, 0.91, 1) * 1.25;
//     float4 result = float4(0.304, 0.841, 0.1, 1) * 1.25;
    float4 result = _PrimaryColor;

    float ditherSize = 0.2f;
    ditherSize *= smoothstep(-200, 10000, camDistance);

    float3 noisePos = (input.positionRS.xyz + _ChunkPosition) * (_ChunkScaling * 0.00003);
    float n = VNoiseD3FBM(noisePos + VNoiseD3(noisePos * 6.0 + 57.2418) * 6.0, 3, 0.5, 3.0).x;
    n = saturate(n);
    n = Dither(n, screenPos, 3, ditherSize);

    float3 normal = -normalize(cross(ddx(input.positionRS.xyz), ddy(input.positionRS.xyz)));
    normal = TransformObjectToWorldDirection(normal);

    normal = normalize(input.normalRS);

//     float lightIntensity = dot(normalize(input.normalRS.xyz), normalize(float3(1, 0, -0.7)));
    float lightIntensity = dot(normal, normalize(float3(1, 0, -0.7)));
    lightIntensity = smoothstep(-0.2, 0.6, lightIntensity);
    lightIntensity = Dither(lightIntensity, screenPos, 3, ditherSize);

    result.rgb = lightIntensity * _PrimaryColor.rgb * n;

    // Atmospheric Scattering
    float viewLen = length(positionWS);
    float3 viewDir = positionWS / viewLen;
    float3 originWS = TransformObjectToWorld(-_ChunkPosition);

    float2 intersections = 0;
    IntersectSphere(0, viewDir, originWS, _AtmosphereRadius, intersections);

    intersections = max(intersections, 0);
    viewLen -= intersections.x;

//     float4 scattering = GetAtmosphericScattering(originWS, _PlanetRadius, _AtmosphereRadius, viewDir * intersections.x, viewDir, viewLen);
//
//     result.rgb = lerp(result.rgb, scattering.rgb, 1.0f - scattering.a / 1.1f);
    float4 scattering = GetAtmosphericScattering(originWS, _PlanetRadius, _AtmosphereRadius, viewDir * intersections.x, viewDir, viewLen);

//     return scattering * 10.0;
//     return scattering;
    result.rgb = lerp(result.rgb, scattering.rgb * 1.0, scattering.a * 0.5);

//     return float4(uv, 0, 1);
    return float4(result.rgb, 1.0);
}
