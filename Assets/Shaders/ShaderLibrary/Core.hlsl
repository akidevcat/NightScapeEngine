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

float2 GetScreenUv(float2 positionCS)
{
    return positionCS / _TargetResolution;
}

uint2 GetScreenPos(float2 positionCS)
{
    return (uint2)positionCS;
}

float4x4 inverse(float4x4 m) {
    float n11 = m[0][0], n12 = m[1][0], n13 = m[2][0], n14 = m[3][0];
    float n21 = m[0][1], n22 = m[1][1], n23 = m[2][1], n24 = m[3][1];
    float n31 = m[0][2], n32 = m[1][2], n33 = m[2][2], n34 = m[3][2];
    float n41 = m[0][3], n42 = m[1][3], n43 = m[2][3], n44 = m[3][3];

    float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
    float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
    float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
    float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

    float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
    float idet = 1.0f / det;

    float4x4 ret;

    ret[0][0] = t11 * idet;
    ret[0][1] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) * idet;
    ret[0][2] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) * idet;
    ret[0][3] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) * idet;

    ret[1][0] = t12 * idet;
    ret[1][1] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) * idet;
    ret[1][2] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) * idet;
    ret[1][3] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) * idet;

    ret[2][0] = t13 * idet;
    ret[2][1] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) * idet;
    ret[2][2] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) * idet;
    ret[2][3] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) * idet;

    ret[3][0] = t14 * idet;
    ret[3][1] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) * idet;
    ret[3][2] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) * idet;
    ret[3][3] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) * idet;

    return ret;
}

inline bool IntersectSphere(float3 rO, float3 rD, float3 sO, float sR, out float2 i)
{
    float a = dot(rD, rD);
    float3 s0_r0 = rO - sO;
    float b = 2.0 * dot(rD, s0_r0);
    float c = dot(s0_r0, s0_r0) - sR * sR;
    float d = b * b - 4.0 * a * c;
    if (d < 0.0)
    {
        i = 0;
        return false;
    }
    float dR = sqrt(d);
    i = float2(-b - dR, -b + dR) / (2.0 * a);
    return true;
}

float4 TransformObjectToClip(float3 position)
{
    float4 result = mul(_ModelMatrix, float4(position, 1));
    result = mul(_ViewMatrix, result);

//     result.xyz = round(result.rgb * 48.0) / 48.0;

    result = mul(_ProjectionMatrix, result);

    return result;
}

float4 TransformWorldToClip(float3 position)
{
    float4 result = mul(_ViewMatrix, float4(position, 1));

//     result.xyz = round(result.rgb * 48.0) / 48.0;

    result = mul(_ProjectionMatrix, result);

    return result;
}

float4 TransformObjectToView(float3 position)
{
    float4 result = mul(_ModelMatrix, float4(position, 1));
    result = mul(_ViewMatrix, result);

//     result.xyz = round(result.rgb * 48.0) / 48.0;

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

float3 TransformClipToWorld(float4 position)
{
    float4 result = mul(inverse(_ProjectionMatrix), position);
    result = mul(inverse(_ViewMatrix), result);

    result /= result.w;

    return result;
}

float4 TransformClip_PixelPerfect(float3 position, uint2 sizeInPixels, float2 offset = float2(0, 0))
{
    float2 pixelSize = float2(2.0, 2.0) / (float2)_TargetResolution;

    float4 result = mul(_ModelMatrix, float4(0, 0, 0, 1));
    result /= result.w;

    if (offset.x == 0 && sizeInPixels.x % 2 == 1)
    {
        result.x += pixelSize.x / 2.0;
    }

    if (offset.y == 0 && sizeInPixels.y % 2 == 1)
    {
        result.y += pixelSize.y / 2.0;
    }

    // result.xy now stores screen NDC position of the pivot
    // ToDo make result.xy always left top position of the quad?
    // Target: find top left corner always

    // if offset = 0:
    // topLeft = result.xy - pixelSize * float2(1, 1) <---- pixelSize which is multiplied by px
    // if offset != 0:
    // topLeft = result.xy - pixelSize * (float2(1, 1) - offset)





    float2 screenPixelSize = float2(1.0, 1.0) / (float2)_TargetResolution;
    float2 quadScreenSize = float2(sizeInPixels) * screenPixelSize;


    float2 topLeftPositionSS = result.xy + (float2(-1, 1) - offset) / 1.0 * quadScreenSize;

    // suppose result.xy was positioned as we want it to be...
    // topLeftPositionSS would be right at the needed place

    float2 vertexPositionOS = position.xy + float2(1, -1);
//     float2 vertexPositionOS = position.xy + float2(0.3, 0.3);
//     float2 vertexPositionOS = position.xy;

    // offset 0
    // 1) result.xy + float2(-1, -1) / 2.0...
    // 2) position.xy + float2(1, 1)

    float2 vertexPositionSS = topLeftPositionSS + vertexPositionOS / 1.0 * quadScreenSize;

//     vertexPositionSS = topLeftPositionSS;
//     float2 vertexPositionSS = result.xy + vertexPositionOS / 1.0 * quadScreenSize;

    return float4(vertexPositionSS, result.z, result.w);

//     result.xy = result.xy * (float2)_TargetResolution / float2(2.0, 2.0);
//     result.xy = round(result.xy);
//     result.xy = result.xy / ((float2)_TargetResolution / float2(2.0, 2.0));

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

    result += float4((position.xy + offset) * pixelSize, 0, 0);

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

float Dither(float value, uint2 screenPos, uint shadeCount = 3, float shadeSize = 0.1f)
{
    value = saturate(value);

    float leftStep = floor(value * shadeCount);
    float rightStep = leftStep + 1;
    float timeStep = value * shadeCount - leftStep;

    float dValue = pow(timeStep, 1.0);
    float ditherMask = step(0.5 - shadeSize, dValue);
    ditherMask *= (screenPos.x % 2) ^ (screenPos.y % 2);
    ditherMask = saturate(ditherMask + step(0.5 + shadeSize, dValue));

    value = (leftStep + ditherMask) / shadeCount;

    return saturate(value);
}

float3 DitherByLuminance(float3 value, uint2 screenPos, uint shadeCount = 3, float shadeSize = 0.1f)
{
    float lum = Luminance(value);
    value /= lum;
    value *= Dither(lum, screenPos, shadeCount, shadeSize);
    return value;
}

#endif