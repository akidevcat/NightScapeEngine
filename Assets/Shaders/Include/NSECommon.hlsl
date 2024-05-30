#ifndef NSE_COMMON_INCLUDED
#define NSE_COMMON_INCLUDED

cbuffer GlobalProperties
{
    float _Time;
    float _DeltaTime;
}

cbuffer DrawProperties
{
    float4x4 _ProjectionMatrix;
    float4x4 _ModelMatrix;
    float4x4 _ViewMatrix;
    float4x4 _MVPMatrix;
};

#endif