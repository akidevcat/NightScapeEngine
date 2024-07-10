#include "ShaderLibrary/Core.hlsl"
#include "ShaderLibrary/Particles.hlsl"

struct ParticleData
{
    float3 position;
    float size;
    float3 color;
    float padding;
};

StructuredBuffer<ParticleData> _ParticlesData;

ParticlePixelInput VertexMain(ParticleVertexInput input)
{
    uint particleID = GetParticleID_Quad(input.vertexID);

    ParticleData pData = _ParticlesData[particleID];

    ParticlePixelInput output;

    InitializeVertexParticle_Billboard_PixelPerfect(input.vertexID, pData.position, uint2(pData.size, pData.size), output);
    output.color.rgb = float4(pData.color, 1.0);
    output.color.a = pData.size;

    return output;
}

float4 PixelMain(ParticlePixelInput input) : SV_TARGET
{
    float2 uv = input.uv;
    uv = TransformUV_PixelPerfect(uv, uint2(input.color.a, input.color.a));

    float2 p = abs(uv - 0.5) * 2.0;
    float i = 1.0 / pow(p.x + p.y, 0.4) * 0.1;
    i -= 0.01;
    i = smoothstep(0, 1, i);
    i = (i - 0.05) / 0.95;
    i = saturate(i);

    return float4(input.color.rgb * i, 1);
//     return float4(uv.x, uv.y, 0, 1);
}
