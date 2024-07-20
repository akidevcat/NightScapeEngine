#include "ShaderLibrary/Core.hlsl"
#include "ShaderLibrary/Particles.hlsl"
#include "ShaderLibrary/Hash.hlsl"

struct ParticleData
{
    float3 position;
    float3 direction;
    float2 color;
};

StructuredBuffer<ParticleData> _ParticlesData;

ParticlePixelInput VertexMain(ParticleVertexInput input)
{
    uint particleID = GetParticleID_Line(input.vertexID);

    ParticleData pData = _ParticlesData[particleID];

    ParticlePixelInput output;



    InitializeVertexParticle_Line(input.vertexID, pData.position, pData.direction * 0.25, output);

//     output.color = hash41(pData.alpha) * 4.0;
    output.color = float4(1,0,0,1);

    return output;
}

float4 PixelMain(ParticlePixelInput input) : SV_TARGET
{
    float2 uv = input.uv;

    return input.color;
}
