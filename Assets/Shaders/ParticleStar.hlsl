#include "ShaderLibrary/Core.hlsl"
#include "ShaderLibrary/Particles.hlsl"
#include "ShaderLibrary/Hash.hlsl"

struct ParticleData
{
    float3 position;
    float2 size;
    float3 color;
    float4 atlasMapping;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float4 uv : TEXCOORD0;
    float4 color : TEXCOORD1;
    nointerpolation float4 mapping : TEXCOORD2;
};

TEXTURE2D_ATLAS(_StarsTexture);

StructuredBuffer<ParticleData> _ParticlesData;

PixelInput VertexMain(ParticleVertexInput input)
{
    uint particleID = GetParticleID_Quad(input.vertexID);

    ParticleData pData = _ParticlesData[particleID];

    ParticlePixelInput output;

    InitializeVertexParticle_Billboard_PixelPerfect(input.vertexID, pData.position, uint2(pData.size), output);
    output.color.rgb = float4(pData.color, 1.0);
    output.color.a = pData.size;
//     output.uv.xy = pData.atlasMapping.xy + pData.atlasMapping.zw * output.uv.xy;

    PixelInput realOutput;

    realOutput.position = output.position;
    realOutput.normal = 0;
    realOutput.uv.xy = output.uv.xy;
    realOutput.uv.zw = pData.size;
    realOutput.color = output.color;
    realOutput.color.a = sin(_Time * 1.0 + hash11(particleID) * 100.0) * 0.3 + 0.7;
    realOutput.mapping = pData.atlasMapping;

    return realOutput;
}

float4 BlendOverlay(float4 base, float4 blend, float opacity)
{
    float4 result1 = 1.0 - 2.0 * (1.0 - base) * (1.0 - blend);
    float4 result2 = 2.0 * base * blend;
    float4 zeroOrOne = step(base, 0.5);
    float4 result = result2 * zeroOrOne + (1 - zeroOrOne) * result1;
    return lerp(base, result, opacity);
}

float4 PixelMain(PixelInput input) : SV_TARGET
{
    float2 uv = input.uv;

//     uv = TransformUV_PixelPerfect(uv, uint2(input.uv.z, input.uv.w));
    uv.xy = input.mapping.xy + input.mapping.zw * uv.xy;
    float4 result = SAMPLE_TEXTURE2D_POINT(_StarsTexture, uv);

//     float2 p = abs(uv - 0.5) * 2.0;
//     float i = 1.0 / pow(p.x + p.y, 0.4) * 0.1;
//     i -= 0.01;
//     i = smoothstep(0, 1, i);
//     i = (i - 0.05) / 0.95;
//     i = saturate(i);

    result = BlendOverlay(result, input.color, 1.0);
    result.a *= input.color.a;

    return result;

    return float4(input.color.rgb * result.rgb * 2.0, 1);
//     return float4(uv.x, uv.y, 0, 1);
}
