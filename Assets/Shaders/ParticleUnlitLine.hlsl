#include "ShaderLibrary/Core.hlsl"
#include "ShaderLibrary/Particles.hlsl"

struct ParticleData
{
    float3 positionOS;
    float3 directionOS;
    float size;
    float alpha;
};

StructuredBuffer<ParticleData> _ParticlesData;

ParticlePixelInput VertexMain(ParticleVertexInput input)
{
    uint particleID = GetParticleID_Line(input.vertexID);

    ParticleData pData = _ParticlesData[particleID];

    ParticlePixelInput output;

    InitializeVertexParticle_Line(input.vertexID, pData.positionOS, pData.directionOS, output);

    return output;
}

float4 PixelMain(ParticlePixelInput input) : SV_TARGET
{
    float2 uv = input.uv;

return uv.x;
    return float4(uv.x, uv.y, 0, 1);

//     return 1;
}
