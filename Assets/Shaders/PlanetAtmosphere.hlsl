#include "ShaderLibrary/Core.hlsl"
#include "ShaderLibrary/Lighting.hlsl"

float _PlanetRadius;
float _AtmosphereRadius;

DefaultPixelInput VertexMain(DefaultVertexInput input)
{
    DefaultPixelInput output;

    output.position = TransformObjectToClip(input.position);
    output.positionRS = TransformObjectToWorld(input.position);
    output.normal = TransformObjectToClipDirection(input.normal);
    output.normalRS.xyz = normalize(TransformObjectToWorldDirection(input.normal));
    output.normalRS.w = 0;
    output.uv = input.uv;
    output.uv.y = 1.0 - output.uv.y;

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
    float atmAccumRayLength = 0.0;

    for (int i = 0; i < sampleCount; i++)
    {
        float height = 1.0 - max(length(samplePos - planetOrigin) - planetRadius, 0.0) / (atmosphereRadius - planetRadius);
        height = saturate(height);

        float sampleDensity = height * 2.2;

        atmAccumDensity += sampleDensity * sampleLength;

//         float sampleLight = lerp(float3(0.506, 0.835, 1), float3(0.9, 0.2, 0.0), saturate(atmAccumRayLength * 0.1));
        float3 absorbedLight = float3(0.506, 0.835, 1) * (exp(sampleLength)) * 0.001;

        // light correction by height
        absorbedLight *= 1.0 + (1.0 - camHeight) * 10.0;

        atmAccumLightEnergy += atmTransmittance * absorbedLight;

        samplePos += rayDirection * sampleLength;
        atmAccumRayLength += sampleLength;
        atmTransmittance *= saturate(1.0 - sampleDensity);
    }

    return float4(atmAccumLightEnergy, 1.0 - atmTransmittance);
}

float4 PixelMain(DefaultPixelInput input) : SV_TARGET
{
    float2 uv = input.uv;
    float2 screenUv = GetScreenUv(input.position.xy);
    uint2 screenPos = GetScreenPos(input.position.xy);

    float3 viewDir = normalize(input.positionRS.xyz);
    float3 originWS = TransformObjectToWorld(0);

    float2 intersections = 0;
    IntersectSphere(0, viewDir, originWS, _AtmosphereRadius, intersections);

    intersections = max(intersections, 0);

//     const uint sampleCount = 6;
//     float3 samplePos = 0 + viewDir * intersections.x;
//     float sampleLength = (intersections.y - intersections.x) / sampleCount;

//     float camHeight = max(length(0 - originWS) - _PlanetRadius, 0.0) /  (_AtmosphereRadius - _PlanetRadius);
//     camHeight = saturate(camHeight);

//     float atmAccumDensity = 0.0f;
//     float atmTransmittance = 1.0f;
//     float3 atmAccumLightEnergy = 0;

//     for (int i = 0; i < sampleCount; i++)
//     {
// //         float height = 1.0 - length(samplePos - originWS) / _AtmosphereRadius;
//
//         float height = 1.0 - max(length(samplePos - originWS) - _PlanetRadius, 0.0) / (_AtmosphereRadius - _PlanetRadius);
//         height = saturate(height);
//
// //         float sampleDensity = pow(height, 0.3) * 2.2;
//         float sampleDensity = height * 2.2;
//
//         atmAccumDensity += sampleDensity * sampleLength;
//
//         float3 absorbedLight = float3(0.506, 0.835, 1) * (exp(sampleLength)) * 0.001;
//
//         // light correction by height
//         absorbedLight *= 1.0 + (1.0 - camHeight) * 10.0;
//
//         atmAccumLightEnergy += atmTransmittance * absorbedLight;
//
//         samplePos += viewDir * sampleLength;
//         atmTransmittance *= saturate(1.0 - sampleDensity);
//     }

    return GetAtmosphericScattering(originWS, _PlanetRadius, _AtmosphereRadius, viewDir * intersections.x, viewDir, intersections.y - intersections.x);

//     return float4(atmAccumLightEnergy, 1.0 - atmTransmittance);
//     return 1.0 - atmTransmittance;

//     return float4(atmAccumLightEnergy, atmTransmittance);
}
