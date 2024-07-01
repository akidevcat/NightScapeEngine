#ifndef NSE_CORE_INCLUDED
#define NSE_CORE_INCLUDED

#define TEXTURE2D(NAME) Texture2D NAME; \
                        float4 NAME ## _Params;

#define TEXTURE2D_ATLAS(NAME) Texture2D NAME; \
                              float4 NAME ## _Params; \
                              float4 NAME ## _AtlasParams;

#define SAMPLE_TEXTURE2D_POINT(NAME, COORDS) NAME ## .Sample(_PointSampler, COORDS)
#define SAMPLE_TEXTURE2D_LINEAR(NAME, COORDS) NAME ## .Sample(_LinearSampler, COORDS)

cbuffer GlobalProperties
{
    float _Time;
    float _DeltaTime;
//     float _VertexJitterResolution;
}

cbuffer DrawProperties
{
    uint _ObjectID;
    float4x4 _ProjectionMatrix;
    float4x4 _ModelMatrix;
    float4x4 _ViewMatrix;
    float4x4 _MVPMatrix;
    uint2 _TargetResolution;
};

SamplerState _PointSampler;
SamplerState _LinearSampler;

struct DefaultVertexInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};

struct DefaultPixelInput
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float4 positionRS : TEXCOORD1;
    float4 normalRS : TEXCOORD2;
};

float4 TransformObjectToClip(float3 position)
{
    float4 result = mul(_ModelMatrix, float4(position, 1));
    result = mul(_ViewMatrix, result);

    result.rgb = round(result.rgb * 48.0) / 48.0;

    result = mul(_ProjectionMatrix, result);

    return result;
}

float4 TransformObjectToView(float3 position)
{
    float4 result = mul(_ModelMatrix, float4(position, 1));
    result = mul(_ViewMatrix, result);

    result.rgb = round(result.rgb * 48.0) / 48.0;

    return result;
}

float4 TransformObjectToWorld(float3 position)
{
    return mul(_ModelMatrix, float4(position, 1));
}

float4 TransformViewToClip(float3 position)
{
    return mul(_ProjectionMatrix, float4(position, 1));
}

float4 TransformObjectToWorldDirection(float3 position)
{
    return mul(_ModelMatrix, float4(position, 0));
}

float4 TransformObjectToClipDirection(float3 direction)
{
    float4 result = mul(_ModelMatrix, float4(direction, 0));
    result = mul(_ViewMatrix, result);

    result = mul(_ProjectionMatrix, result);

    return result;
}

float4 TransformObjectToViewDirection(float3 direction)
{
    float4 result = mul(_ModelMatrix, float4(direction, 0));
    result = mul(_ViewMatrix, result);

    return result;
}

float4 TransformObjectToClip_PixelPerfect(float3 position, uint2 sizeInPixels)
{
    float4 result = mul(_ProjectionMatrix, mul(_ViewMatrix, mul(_ModelMatrix, float4(0, 0, 0, 1))));

    float2 pixelSize = float2(2.0, 2.0) / (float2)_TargetResolution;

    uint2 px = uint2(sizeInPixels.x / 2, sizeInPixels.y / 2);

    if (sizeInPixels.x % 2 == 1 && position.x > 0)
    {
        px.x += 1;
    }
    if (sizeInPixels.y % 2 == 1 && position.y > 0)
    {
        px.y += 1;
    }

    pixelSize.x *= (float)px.x;
    pixelSize.y *= (float)px.y;

    result /= result.w;
    result += float4(position.xy * pixelSize, 0, 0);

    return result;
}

float2 TransformUV_PixelPerfect(float2 uv, uint2 sizeInPixels)
{
    float2 px = (float2)sizeInPixels;
    return (floor(uv * px) + 0.5) / px;
}

float Luminance(float3 c)
{
    return (0.2126*c.r + 0.7152*c.g + 0.0722*c.b);
}

#endif