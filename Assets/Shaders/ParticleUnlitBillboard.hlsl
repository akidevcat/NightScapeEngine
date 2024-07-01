#include "ShaderLibrary/Core.hlsl"
#include "ShaderLibrary/Particles.hlsl"

struct ParticleData
{
    float3 positionOS;
    float size;
};

StructuredBuffer<ParticleData> _ParticlesData;

ParticlePixelInput VertexMain(ParticleVertexInput input)
{
    uint particleID = GetParticleID_Quad(input.vertexID);

    ParticleData pData = _ParticlesData[particleID];

    ParticlePixelInput output;

    InitializeVertexParticle_Billboard(input.vertexID, pData.positionOS, pData.size, output);

    return output;
}

float4 PixelMain(ParticlePixelInput input) : SV_TARGET
{
    float2 uv = input.uv;
    return float4(uv.x, uv.y, 0, 1);
}
