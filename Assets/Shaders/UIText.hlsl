#include "ShaderLibrary/Core.hlsl"

TEXTURE2D_ATLAS(_TextImage)
Buffer<uint> _TextBuffer;
uint _TextLength;
bool _IsScreenSpace;

float4 _Tint;

struct TextVertexInput
{
    uint vertexID : SV_VertexID;
};

struct TextPixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

TextPixelInput VertexMain(TextVertexInput input)
{
    TextPixelInput output;

    uint lineIndex = input.vertexID / 4;
    uint lVertexID = input.vertexID % 4;

    float3 vPosOS = float3(1, 1, 0);

    if (lVertexID < 2)
    {
        vPosOS.x *= -1;
    }
    if (lVertexID < 1 || lVertexID > 2)
    {
        vPosOS.y *= -1;
    }

    output.uv = float2(vPosOS.x, vPosOS.y) * 0.5 + 0.5;

    uint stripLeftPx = 0;
    uint stripRightPx = 0;

    uint2 charSizePx = (uint2)_TextImage_AtlasParams.xy;
    charSizePx.x -= stripLeftPx + stripRightPx;
    uint2 lineSizePx = charSizePx * uint2(_TextLength, 1);

    if (_IsScreenSpace)
    {
        output.position = TransformClip_PixelPerfect(vPosOS, uint2(lineSizePx));
    }
    else
    {
        output.position = TransformObjectToClip_PixelPerfect(vPosOS, uint2(lineSizePx));
    }

    output.uv.y = 1.0 - output.uv.y;

    return output;
}

float4 PixelMain(TextPixelInput input) : SV_TARGET
{
    uint2 charSizePx = (uint2)_TextImage_AtlasParams.xy;
    uint2 lineSizePx = charSizePx * uint2(_TextLength, 1);
    uint2 charAtlasCount = _TextImage_AtlasParams.zw / _TextImage_AtlasParams.xy;
    float2 uv = input.uv;

    uv = TransformUV_PixelPerfect(uv, lineSizePx);

    uint charIndex = (uint)(uv.x * _TextLength);
    uint charData = _TextBuffer[charIndex] - 32;

    uint charAtlasIndexX = charData % charAtlasCount.x;
    uint charAtlasIndexY = charData / charAtlasCount.x;

    uv.x *= _TextLength;
    uv.x %= 1.0;

    uint charStartXPx = charAtlasIndexX * charSizePx.x;
    uint charStartYPx = charAtlasIndexY * charSizePx.y;

    uint stripLeftPx = 0;
    uint stripRightPx = 0;

    uv.x = (charStartXPx + uv.x * (charSizePx.x - stripRightPx - stripLeftPx) + stripLeftPx) / (_TextImage_AtlasParams.z);
    uv.y = (charStartYPx + uv.y * charSizePx.y) / (_TextImage_AtlasParams.w);

    float4 color = SAMPLE_TEXTURE2D_POINT(_TextImage, uv);

    return color * _Tint;
}
