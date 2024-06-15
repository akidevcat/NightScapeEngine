#ifndef NSE_COMMON_INCLUDED
#define NSE_COMMON_INCLUDED

cbuffer GlobalProperties
{
    float _Time;
    float _DeltaTime;
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
    result = mul(_ProjectionMatrix, result);

    return result;
}

float4 TransformObjectToView(float3 position)
{
    float4 result = mul(_ModelMatrix, float4(position, 1));
    result = mul(_ViewMatrix, result);

    return result;
}

float4 TransformObjectToWorld(float3 position)
{
    return mul(_ModelMatrix, float4(position, 1));
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

float4 TransformObjectToNDC_PixelPerfect(float3 position, uint2 sizeInPixels)
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

#endif