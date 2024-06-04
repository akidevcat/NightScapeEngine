#include "Include/NSECommon.hlsl"

cbuffer MaterialProperties
{

};

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

    float4 vsObjectPosition = mul(_ViewMatrix, mul(_ModelMatrix, float4(0, 0, 0, 1)));


//     output.position = mul(_ModelMatrix, float4(input.position, 1));
//     output.position = mul(_ViewMatrix, output.position);
    output.position = mul(_ProjectionMatrix, vsObjectPosition + float4(input.position.xy * float2(1, 2), 0, 0));

    output.uv = input.uv;

    return output;
}

float4 PixelMain(PixelInput input) : SV_TARGET
{
    input.uv = round(input.uv * 15.0) / 15.0;
    float2 p = abs(input.uv - 0.5) * 2.0;
    float i = 1.0 / pow(p.x * p.y, 1.0) * 0.01;
    i -= 0.1;
    clip(i);
    return i;
}
