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
}

#endif