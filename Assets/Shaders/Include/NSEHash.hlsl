#ifndef NSE_HASH_INCLUDED
#define NSE_HASH_INCLUDED

// Hash Functions
//
// murmurHashNM() takes M unsigned integers and returns N hash values.
// The returned values are unsigned integers between 0 and 2^32 - 1.
//
// hashNM() takes M floating point numbers and returns N hash values.
// The returned values are floating point numbers between 0.0 and 1.0.

//------------------------------------------------------------------------------

uint murmurHash11(uint src) {
    const uint M = 0x5bd1e995u;
    uint h = 1190494759u;
    src *= M; src ^= src>>24u; src *= M;
    h *= M; h ^= src;
    h ^= h>>13u; h *= M; h ^= h>>15u;
    return h;
}

// 1 output, 1 input
float hash11(float src) {
    uint h = murmurHash11(asuint(src));
    return asfloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uint murmurHash12(uint2 src) {
    const uint M = 0x5bd1e995u;
    uint h = 1190494759u;
    src *= M; src ^= src>>24u; src *= M;
    h *= M; h ^= src.x; h *= M; h ^= src.y;
    h ^= h>>13u; h *= M; h ^= h>>15u;
    return h;
}

// 1 output, 2 inputs
float hash12(float2 src) {
    uint h = murmurHash12(asuint(src));
    return asfloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uint murmurHash13(uint3 src) {
    const uint M = 0x5bd1e995u;
    uint h = 1190494759u;
    src *= M; src ^= src>>24u; src *= M;
    h *= M; h ^= src.x; h *= M; h ^= src.y; h *= M; h ^= src.z;
    h ^= h>>13u; h *= M; h ^= h>>15u;
    return h;
}

// 1 output, 3 inputs
float hash13(float3 src) {
    uint h = murmurHash13(asuint(src));
    return asfloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uint murmurHash14(uint4 src) {
    const uint M = 0x5bd1e995u;
    uint h = 1190494759u;
    src *= M; src ^= src>>24u; src *= M;
    h *= M; h ^= src.x; h *= M; h ^= src.y; h *= M; h ^= src.z; h *= M; h ^= src.w;
    h ^= h>>13u; h *= M; h ^= h>>15u;
    return h;
}

// 1 output, 4 inputs
float hash14(float4 src) {
    uint h = murmurHash14(asuint(src));
    return asfloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uint2 murmurHash21(uint src) {
    const uint M = 0x5bd1e995u;
    uint2 h = uint2(1190494759u, 2147483647u);
    src *= M; src ^= src>>24u; src *= M;
    h *= M; h ^= src;
    h ^= h>>13u; h *= M; h ^= h>>15u;
    return h;
}

// 2 outputs, 1 input
float2 hash21(float src) {
    uint2 h = murmurHash21(asuint(src));
    return asfloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uint2 murmurHash22(uint2 src) {
    const uint M = 0x5bd1e995u;
    uint2 h = uint2(1190494759u, 2147483647u);
    src *= M; src ^= src>>24u; src *= M;
    h *= M; h ^= src.x; h *= M; h ^= src.y;
    h ^= h>>13u; h *= M; h ^= h>>15u;
    return h;
}

// 2 outputs, 2 inputs
float2 hash22(float2 src) {
    uint2 h = murmurHash22(asuint(src));
    return asfloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uint2 murmurHash23(uint3 src) {
    const uint M = 0x5bd1e995u;
    uint2 h = uint2(1190494759u, 2147483647u);
    src *= M; src ^= src>>24u; src *= M;
    h *= M; h ^= src.x; h *= M; h ^= src.y; h *= M; h ^= src.z;
    h ^= h>>13u; h *= M; h ^= h>>15u;
    return h;
}

// 2 outputs, 3 inputs
float2 hash23(float3 src) {
    uint2 h = murmurHash23(asuint(src));
    return asfloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uint2 murmurHash24(uint4 src) {
    const uint M = 0x5bd1e995u;
    uint2 h = uint2(1190494759u, 2147483647u);
    src *= M; src ^= src>>24u; src *= M;
    h *= M; h ^= src.x; h *= M; h ^= src.y; h *= M; h ^= src.z; h *= M; h ^= src.w;
    h ^= h>>13u; h *= M; h ^= h>>15u;
    return h;
}

// 2 outputs, 4 inputs
float2 hash24(float4 src) {
    uint2 h = murmurHash24(asuint(src));
    return asfloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uint3 murmurHash31(uint src) {
    const uint M = 0x5bd1e995u;
    uint3 h = uint3(1190494759u, 2147483647u, 3559788179u);
    src *= M; src ^= src>>24u; src *= M;
    h *= M; h ^= src;
    h ^= h>>13u; h *= M; h ^= h>>15u;
    return h;
}

// 3 outputs, 1 input
float3 hash31(float src) {
    uint3 h = murmurHash31(asuint(src));
    return asfloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uint3 murmurHash32(uint2 src) {
    const uint M = 0x5bd1e995u;
    uint3 h = uint3(1190494759u, 2147483647u, 3559788179u);
    src *= M; src ^= src>>24u; src *= M;
    h *= M; h ^= src.x; h *= M; h ^= src.y;
    h ^= h>>13u; h *= M; h ^= h>>15u;
    return h;
}

// 3 outputs, 2 inputs
float3 hash32(float2 src) {
    uint3 h = murmurHash32(asuint(src));
    return asfloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uint3 murmurHash33(uint3 src) {
    const uint M = 0x5bd1e995u;
    uint3 h = uint3(1190494759u, 2147483647u, 3559788179u);
    src *= M; src ^= src>>24u; src *= M;
    h *= M; h ^= src.x; h *= M; h ^= src.y; h *= M; h ^= src.z;
    h ^= h>>13u; h *= M; h ^= h>>15u;
    return h;
}

// 3 outputs, 3 inputs
float3 hash33(float3 src) {
    uint3 h = murmurHash33(asuint(src));
    return asfloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uint3 murmurHash34(uint4 src) {
    const uint M = 0x5bd1e995u;
    uint3 h = uint3(1190494759u, 2147483647u, 3559788179u);
    src *= M; src ^= src>>24u; src *= M;
    h *= M; h ^= src.x; h *= M; h ^= src.y; h *= M; h ^= src.z; h *= M; h ^= src.w;
    h ^= h>>13u; h *= M; h ^= h>>15u;
    return h;
}

// 3 outputs, 4 inputs
float3 hash34(float4 src) {
    uint3 h = murmurHash34(asuint(src));
    return asfloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uint4 murmurHash41(uint src) {
    const uint M = 0x5bd1e995u;
    uint4 h = uint4(1190494759u, 2147483647u, 3559788179u, 179424673u);
    src *= M; src ^= src>>24u; src *= M;
    h *= M; h ^= src;
    h ^= h>>13u; h *= M; h ^= h>>15u;
    return h;
}

// 4 outputs, 1 input
float4 hash41(float src) {
    uint4 h = murmurHash41(asuint(src));
    return asfloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uint4 murmurHash42(uint2 src) {
    const uint M = 0x5bd1e995u;
    uint4 h = uint4(1190494759u, 2147483647u, 3559788179u, 179424673u);
    src *= M; src ^= src>>24u; src *= M;
    h *= M; h ^= src.x; h *= M; h ^= src.y;
    h ^= h>>13u; h *= M; h ^= h>>15u;
    return h;
}

// 4 outputs, 2 inputs
float4 hash42(float2 src) {
    uint4 h = murmurHash42(asuint(src));
    return asfloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uint4 murmurHash43(uint3 src) {
    const uint M = 0x5bd1e995u;
    uint4 h = uint4(1190494759u, 2147483647u, 3559788179u, 179424673u);
    src *= M; src ^= src>>24u; src *= M;
    h *= M; h ^= src.x; h *= M; h ^= src.y; h *= M; h ^= src.z;
    h ^= h>>13u; h *= M; h ^= h>>15u;
    return h;
}

// 4 outputs, 3 inputs
float4 hash43(float3 src) {
    uint4 h = murmurHash43(asuint(src));
    return asfloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uint4 murmurHash44(uint4 src) {
    const uint M = 0x5bd1e995u;
    uint4 h = uint4(1190494759u, 2147483647u, 3559788179u, 179424673u);
    src *= M; src ^= src>>24u; src *= M;
    h *= M; h ^= src.x; h *= M; h ^= src.y; h *= M; h ^= src.z; h *= M; h ^= src.w;
    h ^= h>>13u; h *= M; h ^= h>>15u;
    return h;
}

// 4 outputs, 4 inputs
float4 hash44(float4 src) {
    uint4 h = murmurHash44(asuint(src));
    return asfloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

#endif