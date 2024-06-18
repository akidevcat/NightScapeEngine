#include "Include/NSECommon.hlsl"
#include "Include/NSEHash.hlsl"

uint _PixelSizeX;
uint _PixelSizeY;

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
    float4 params : TEXCOORD1;
};

PixelInput VertexMain(VertexInput input)
{
    PixelInput output;

    output.position = TransformObjectToNDC_PixelPerfect(input.position, uint2(_PixelSizeX, _PixelSizeY));

    output.uv = input.uv;
    output.params.x = hash11(_ObjectID);
    output.params.y = hash11(_ObjectID + 100000);
    output.params.z = hash11(_ObjectID + 200000);
    output.params.w = hash11(_ObjectID + 300000);

    return output;
}

float4 PixelMain(PixelInput input) : SV_TARGET
{
    float2 uv = TransformUV_PixelPerfect(input.uv, uint2(_PixelSizeX, _PixelSizeY));

    float2 p = abs(uv - 0.5) * 2.0;
    float i = 1.0 / pow(p.x + p.y, 0.7) * 0.1;
    i -= 0.01;
    i = smoothstep(0, 1, i);
    i *= (sin(_Time * (0.8 * input.params.y + 0.2) * 2.0 + input.params.x * 100.0) * 0.5 + 0.5) * 0.5 + 0.5;
    i *= input.params.z * 0.5 + 0.5;
    i = (i - 0.05) / 0.95;
    i = saturate(i);

    return float4(i, i, i, 1.0);
}
