#include "ShaderLibrary/Core.hlsl"
#include "ShaderLibrary/Particles.hlsl"

struct ParticleData
{
    float3 position;
    float padding;
    float4 color;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 color : TEXCOORD0;
};

StructuredBuffer<ParticleData> _ParticlesData;

PixelInput VertexMain(ParticleVertexInput input)
{
    uint particleID = GetParticleID_Point(input.vertexID);

    ParticleData pData = _ParticlesData[particleID];

    PixelInput output;

//     output.normal = 0;
//     output.uv = float2(0.5, 0.5);
//     output.position = TransformWorldToClip(pData.position);
    output.position = TransformObjectToWorld(0);

    float pLength = length(pData.position);
    pLength = max(0.00001, pLength);

    float3 pos = pData.position / pLength;

    pos *= 0.2 * (1.0 - 1.0 / (1.0 + pLength));

    output.position = TransformWorldToClip(output.position + pos);
    output.color = pData.color;
//     InitializeVertexParticle_Point(pData.positionOS, output);

    return output;
}

float4 PixelMain(PixelInput input) : SV_TARGET
{
    return input.color;
}
