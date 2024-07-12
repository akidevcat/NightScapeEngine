#include "ShaderLibrary/Core.hlsl"

TEXTURE2D_ATLAS(_Image)

float4 _ForegroundColor;
float4 _BackgroundColor;
float4 _Tint;
float2 _Size;
bool _IsScreenSpace;
float2 _UVOffset;
float _ProgressValue;

DefaultPixelInput VertexMain(DefaultVertexInput input)
{
    DefaultPixelInput output;

    if (_IsScreenSpace)
    {
        output.position = TransformObjectToWorld(input.position);
        output.position = TransformClip_PixelPerfect(input.position, uint2(_Size));
    }
    else
    {
        output.position = TransformObjectToClip_PixelPerfect(input.position, uint2(_Size));
    }

    output.uv = input.uv;
    output.uv.y = 1.0 - output.uv.y;
    output.uv = abs(output.uv + _UVOffset);

    return output;
}

float4 PixelMain(DefaultPixelInput input) : SV_TARGET
{
    float2 uv = input.uv;

    uv = TransformUV_PixelPerfect(uv, uint2(_Size));
    uv = _Image_AtlasParams.xy + _Image_AtlasParams.zw * uv;

    float4 color = SAMPLE_TEXTURE2D_POINT(_Image, uv);

    float barMask = 1.0 - step(0.01, color.b);
    float barStep = color.r;

    if (color.b < 0.01 && color.a > 0.5)
    {
        if (_ProgressValue > color.g)
        {
            return _ForegroundColor;
        }
        else
        {
            return _BackgroundColor;
        }
    }

    return color * _Tint;
}
