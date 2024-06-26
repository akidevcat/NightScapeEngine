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

inline void InitializeParticlePixelInput(ParticleVertexInput input, out ParticlePixelInput output)
{

}

#endif