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
    uint lVertexID = input.vertexID % 4;
    ParticleData pData = _ParticlesData[particleID];

    ParticlePixelInput output;

// ToDo
    output.position = float4(1, 1, 0, 1);

    if (lVertexID < 2)
    {
        output.position.x *= -1;
    }
    if (lVertexID < 1 || lVertexID > 2)
    {
        output.position.y *= -1;
    }

    output.normal = 0;
    output.uv = float2(output.position.x, output.position.y) * 0.5 + 0.5;

    output.position = TransformObjectToClip(output.position * pData.size + pData.positionOS);
//     output.normal = TransformObjectToClipDirection(input.normal);
//     output.uv = input.uv;

    return output;
}

float4 PixelMain(ParticlePixelInput input) : SV_TARGET
{
    float2 uv = input.uv;

    return 1;
}
