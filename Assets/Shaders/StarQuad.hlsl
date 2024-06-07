#include "Include/NSECommon.hlsl"
#include "Include/NSEHash.hlsl"

cbuffer MaterialProperties
{
    uint _PixelSizeX;
    uint _PixelSizeY;
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
    float4 params : TEXCOORD1;
};

PixelInput VertexMain(VertexInput input)
{
    PixelInput output;

    float aspect = (float)_TargetResolution.x / (float)_TargetResolution.y;
    float2 pixelSize = float2(2.0, 2.0) / (float2)_TargetResolution;

    uint2 px = uint2(_PixelSizeX / 2, _PixelSizeY / 2);

    if (_PixelSizeX % 2 == 1 && input.position.x > 0)
    {
        px.x += 1;
    }
    if (_PixelSizeY % 2 == 1 && input.position.y > 0)
    {
        px.y += 1;
    }

    pixelSize.x *= (float)px.x;
    pixelSize.y *= (float)px.y;

    float4 ndcObjectPosition = mul(_ProjectionMatrix, mul(_ViewMatrix, mul(_ModelMatrix, float4(0, 0, 0, 1))));

    output.position = ndcObjectPosition;
    output.position /= output.position.w;
    output.position += float4(input.position.xy * pixelSize, 0, 0);

    output.uv = input.uv;
    output.params.x = hash11(_ObjectID);
    output.params.y = hash11(_ObjectID + 100000);
    output.params.z = hash11(_ObjectID + 200000);
    output.params.w = hash11(_ObjectID + 300000);

    return output;
}

float4 PixelMain(PixelInput input) : SV_TARGET
{
//     return 1;
    float2 px = float2(_PixelSizeX, _PixelSizeY);

    input.uv = (floor(input.uv * px) + 0.5) / px;
//     return float4(input.uv, 0, 1);
    float2 p = abs(input.uv - 0.5) * 2.0;
    float i = 1.0 / pow(p.x + p.y, 0.7) * 0.1;
    i -= 0.01;
    i = smoothstep(0, 1, i);
    i *= (sin(_Time * (0.8 * input.params.y + 0.2) * 2.0 + input.params.x * 100.0) * 0.5 + 0.5) * 0.5 + 0.5;
    i *= input.params.z * 0.5 + 0.5;
    i = (i - 0.05) / 0.95;
    i = saturate(i);
    return float4(i, i, i, 1.0);
}
