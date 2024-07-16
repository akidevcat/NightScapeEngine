#include "ShaderLibrary/Core.hlsl"
#include "ShaderLibrary/Hash.hlsl"

TEXTURE2D_ATLAS(_Image)

float4 _Tint;
float2 _Size;
bool _IsPixelPerfect;
bool _IsScreenSpace;

DefaultPixelInput VertexMain(DefaultVertexInput input)
{
    DefaultPixelInput output;

    if (_IsScreenSpace)
    {
        output.position = TransformObjectToWorld(input.position);

        if (_IsPixelPerfect)
//             output.position = TransformObjectToClip_PixelPerfect(input.position, uint2(_Size));
            output.position = TransformClip_PixelPerfect(input.position, uint2(_Size));
        else
            output.position = TransformObjectToClip(input.position);
    }
    else
    {
        if (_IsPixelPerfect)
            output.position = TransformObjectToClip_PixelPerfect(input.position, uint2(_Size));
        else
            output.position = TransformObjectToClip(input.position);
    }

    output.uv = input.uv;
    output.uv.y = 1.0 - output.uv.y;


    return output;
}

float4 PixelMain(DefaultPixelInput input) : SV_TARGET
{
    float2 uv = input.uv;

    uv = TransformUV_PixelPerfect(uv, uint2(_Size));
    uv = _Image_AtlasParams.xy + _Image_AtlasParams.zw * uv.xy;
    uv.y *= 0.99; // ToDo
    float4 color = SAMPLE_TEXTURE2D_POINT(_Image, uv);

    return color * _Tint;
}
