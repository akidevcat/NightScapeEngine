#include "ShaderLibrary/Core.hlsl"

Texture2D _MainTex;

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

    output.position = float4(input.position.x, input.position.y, 0.5, 1.0);

    output.uv = input.uv;
    output.uv.y = 1.0 - output.uv.y;

    return output;
}

float4 PixelMain(PixelInput input) : SV_TARGET
{
//     return float4(input.uv.x, input.uv.y, 1.0 - input.uv.x, 1.0) + _MainTex.Sample(_PointSampler, input.uv);
    return _MainTex.Sample(_PointSampler, input.uv);
}
