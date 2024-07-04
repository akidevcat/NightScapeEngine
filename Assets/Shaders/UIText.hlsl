#include "ShaderLibrary/Core.hlsl"

TEXTURE2D_ATLAS(_TextImage)
Buffer<uint> _TextBuffer;
uint _TextLength;

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

//     output.position = TransformObjectToView(positionOS);
//     output.position.xyz += vPosOS * size;
//     output.position = TransformViewToClip(output.position.xyz);


//     if (_IsScreenSpace)
//     {
//         output.position = TransformObjectToWorld(input.position);
//     }

    uint stripLeftPx = 4;
    uint stripRightPx = 4;

    uint2 charSizePx = (uint2)_TextImage_AtlasParams.xy;
    charSizePx.x -= stripLeftPx + stripRightPx;
    uint2 lineSizePx = charSizePx * uint2(_TextLength, 1);

//     output.position += _TextLineIndex * float4(0, -2.0f, 0, 0);
    output.position = TransformObjectToClip_PixelPerfect(vPosOS, uint2(lineSizePx));

//     output.uv.y = 1.0 - output.uv.y;
//     output.uv.xy = _Image_AtlasParams.xy + _Image_AtlasParams.zw * output.uv.xy;
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
    uint charData = _TextBuffer[charIndex];

    uint charAtlasIndexX = charData % charAtlasCount.x;
    uint charAtlasIndexY = charData / charAtlasCount.y;

    uv.x *= _TextLength;
    uv.x %= 1.0;

    uint charStartXPx = charAtlasIndexX * charSizePx.x;
    uint charStartYPx = charAtlasIndexY * charSizePx.y;

    uint stripLeftPx = 4;
    uint stripRightPx = 4;

    uv.x = (charStartXPx + uv.x * (charSizePx.x - stripRightPx - stripLeftPx) + stripLeftPx) / (_TextImage_AtlasParams.z - 1);
    uv.y = (charStartYPx + uv.y * charSizePx.y) / (_TextImage_AtlasParams.w - 1);

    float4 color = SAMPLE_TEXTURE2D_POINT(_TextImage, uv);

    return color * _Tint;
}
