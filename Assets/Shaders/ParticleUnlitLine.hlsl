#include "ShaderLibrary/Core.hlsl"
#include "ShaderLibrary/Particles.hlsl"
#include "ShaderLibrary/Hash.hlsl"

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

//     output.color = hash41(pData.alpha) * 4.0;
    output.color = hash11(pData.alpha) * 4.0;

    return output;
}

float4 PixelMain(ParticlePixelInput input) : SV_TARGET
{
    float2 uv = input.uv;

    return float4(input.color.rgb, uv.x * input.color.a);

//     return 1;
}
