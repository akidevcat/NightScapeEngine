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

    output.position = float4(1, 1, 0, 1);

    if (lVertexID < 2)
    {
        output.position.x *= -1;
    }
    if (lVertexID < 1 || lVertexID > 2)
    {
        output.position.y *= -1;
    }

    output.normal = 0;
    output.uv = float2(output.position.x, output.position.y) * 0.5 + 0.5;
    output.position = TransformObjectToClip(output.position * size + positionOS);
}

#endif