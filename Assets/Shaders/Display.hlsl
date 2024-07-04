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
    float4 result = _MainTex.Sample(_PointSampler, input.uv);

//     result.rgb = pow(result.rgb, 1.0 / 2.2);
//     result.rgb = round(result.rgb * 32.0) / 32.0;
//     result.rgb = pow(result.rgb, 2.2);

    return float4(result.rgb, 1.0);
}
