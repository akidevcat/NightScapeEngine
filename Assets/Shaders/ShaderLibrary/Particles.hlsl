#ifndef NSE_PARTICLES_INCLUDED
#define NSE_PARTICLES_INCLUDED

struct ParticleVertexInput
{
    uint vertexID : SV_VertexID;
};

struct ParticlePixelInput
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float4 color : TEXCOORD1;
};

inline uint GetParticleID_Quad(uint vertexID)
{
    return vertexID / 4;
}

inline uint GetParticleID_Line(uint vertexID)
{
    return vertexID / 2;
}

inline uint GetParticleID_Point(uint vertexID)
{
    return vertexID;
}

inline void InitializeVertexParticle_Billboard(uint vertexID, float3 positionOS, float size,
    out ParticlePixelInput output)
{
    uint lVertexID = vertexID % 4;

    float3 vPosOS = float3(1, 1, 0);

    if (lVertexID < 2)
    {
        vPosOS.x *= -1;
    }
    if (lVertexID < 1 || lVertexID > 2)
    {
        vPosOS.y *= -1;
    }

    output.normal = 0;
    output.uv = float2(vPosOS.x, vPosOS.y) * 0.5 + 0.5;

    output.position = TransformObjectToView(positionOS);
    output.position.xyz += vPosOS * size;
    output.position = TransformViewToClip(output.position.xyz);
    output.color = 1;
}

inline void InitializeVertexParticle_Billboard_PixelPerfect(uint vertexID, float3 positionOS, uint2 sizeInPixels,
    out ParticlePixelInput output)
{
    uint lVertexID = vertexID % 4;

    float3 vPosOS = float3(1, 1, 0);

    if (lVertexID < 2)
    {
        vPosOS.x *= -1;
    }
    if (lVertexID < 1 || lVertexID > 2)
    {
        vPosOS.y *= -1;
    }

    output.normal = 0;
    output.uv = float2(vPosOS.x, vPosOS.y) * 0.5 + 0.5;

    output.position = TransformObjectToClip(positionOS);

    float2 pixelSize = float2(2.0, 2.0) / (float2)_TargetResolution;

    uint2 px = uint2(sizeInPixels.x / 2, sizeInPixels.y / 2);

    if (sizeInPixels.x % 2 == 1 && vPosOS.x > 0)
    {
        px.x += 1;
    }
    if (sizeInPixels.y % 2 == 1 && vPosOS.y > 0)
    {
        px.y += 1;
    }

    pixelSize.x *= (float)px.x;
    pixelSize.y *= (float)px.y;

    output.position /= output.position.w;
    output.position += float4(vPosOS.xy * pixelSize, 0, 0);
    output.color = 1;
}

inline void InitializeVertexParticle_Line(uint vertexID, float3 positionOS, float3 directionOS,
    out ParticlePixelInput output)
{
    uint lVertexID = vertexID % 2;

    float3 vPosOS = 0;
    float2 uv = float2(0, 0.5);

    if (lVertexID > 0)
    {
        vPosOS = directionOS;
        uv.x = 1;
    }

    output.normal = 0;
    output.uv = uv;

    output.position = TransformObjectToClip(positionOS + vPosOS);
    output.color = 1;
}

inline void InitializeVertexParticle_Point(float3 positionOS,
    out ParticlePixelInput output)
{
    output.normal = 0;
    output.uv = float2(0.5, 0.5);
    output.position = TransformObjectToClip(positionOS);
    output.color = 1;
}

#endif