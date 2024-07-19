#include "ShaderLibrary/Core.hlsl"
#include "ShaderLibrary/Hash.hlsl"

TEXTURE2D_ATLAS(_Image)

float4 _Tint;
float2 _Size;
bool _IsScreenSpace;
float2 _AlignmentOffset;

DefaultPixelInput VertexMain(DefaultVertexInput input)
{
    DefaultPixelInput output;

    if (_IsScreenSpace)
    {
        output.position = TransformClip_PixelPerfect(input.position, uint2(_Size), float2(_AlignmentOffset.x, -_AlignmentOffset.y));
    }
    else
    {
        input.position.xy += _AlignmentOffset;
        output.position = TransformObjectToClip_PixelPerfect(input.position, uint2(_Size));
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

//     return hash41(_ObjectID);

    float4 color = SAMPLE_TEXTURE2D_POINT(_Image, uv);

    return color * _Tint;
}
