#include "Include/NSECommon.hlsl"

DefaultPixelInput VertexMain(DefaultVertexInput input)
{
    DefaultPixelInput output;

    output.position = TransformObjectToClip(input.position);

    output.uv = input.uv;

    return output;
}

float4 PixelMain(DefaultPixelInput input) : SV_TARGET
{
    float2 uv = input.uv;
    float4 result = float4(uv.x, uv.y, 1.0 - uv.x, 1.0);

    return result;
}
