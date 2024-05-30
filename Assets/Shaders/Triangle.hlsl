#include "Include/NSECommon.hlsl"

cbuffer MaterialProperties
{

};

Texture2D shaderTexture;
SamplerState SampleType;

struct VertexInput
{
    float3 position : POSITION;
    float3 normal : POSITION;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

PixelInput VertexMain(VertexInput input)
{
    PixelInput output;

    output.position = mul(_ModelMatrix, float4(input.position, 1));
    output.position = mul(_ViewMatrix, output.position);
    output.position = mul(_ProjectionMatrix, output.position);
//     output.position.xyz /= output.position.w;

    output.uv = input.uv;

    return output;
}

float4 PixelMain(PixelInput input) : SV_TARGET
{
//     return float4(input.uv.x, input.uv.y, 1.0 - input.uv.x, 1.0) * (sin(_Time * 10.0) * 0.5 + 0.5);
    return float4(input.uv.x, input.uv.y, 1.0 - input.uv.x, 1.0) + shaderTexture.Sample(SampleType, input.uv);
}
