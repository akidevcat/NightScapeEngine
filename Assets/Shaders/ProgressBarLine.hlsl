#include "ShaderLibrary/Core.hlsl"

float _ProgressValue;
float4 _ForegroundColor;
float4 _BackgroundColor;

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
    float4 result = 0;

    result = lerp(_BackgroundColor, _ForegroundColor, step(_ProgressValue, uv.x));

    return result;
}
