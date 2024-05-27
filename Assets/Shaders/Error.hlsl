#include "Include/NSECommon.hlsl"

cbuffer MaterialProperties
{

};

//Texture2D shaderTexture : register(t0);
//SamplerState SampleType : register(s0);

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

    output.position.xyz = input.position;
    output.position.z = 0.5;
    output.position.w = 1.0f;
    output.uv = input.uv;

    return output;
}

float4 PixelMain(PixelInput input) : SV_TARGET
{
    return float4(1, 0, 1, 1);
}
