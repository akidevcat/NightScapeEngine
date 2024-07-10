#include "ShaderLibrary/Core.hlsl"
#include "ShaderLibrary/Lighting.hlsl"
#include "ShaderLibrary/Noise.hlsl"

cbuffer ChunkDrawBuffer
{
    float _ChunkScaling;
    float3 _ChunkPosition;
};

DefaultPixelInput VertexMain(DefaultVertexInput input)
{
    DefaultPixelInput output;

    output.position = TransformObjectToClip(input.position);
    output.positionRS = float4(input.position, 1);
    output.normal = TransformObjectToClipDirection(input.normal);
    output.normalRS = TransformObjectToWorldDirection(input.normal);
    output.uv = input.uv;

    return output;
}

float4 PixelMain(DefaultPixelInput input) : SV_TARGET
{
    float2 uv = input.uv;
    uint2 screenPos = GetScreenPos(input.position.xy);

    float4 result = float4(0.804, 0.541, 0.91, 1) * 1.25;

    float n = VNoiseD3FBM((input.positionRS.xyz + _ChunkPosition) * (_ChunkScaling * 0.0001), 3, 0.5, 2.0).x;
    n = saturate(n);
    n = Dither(n, screenPos);

    float3 normal = -normalize(cross(ddx(input.positionRS.xyz), ddy(input.positionRS.xyz)));
    normal = TransformObjectToWorldDirection(normal);

//     float lightIntensity = dot(normalize(input.normalRS.xyz), normalize(float3(1, 0, -0.7)));
    float lightIntensity = dot(normal, normalize(float3(1, 0, -0.7)));
    lightIntensity = smoothstep(-0.2, 0.6, lightIntensity);
    lightIntensity = Dither(lightIntensity, screenPos);

    result.rgb = lightIntensity * float3(0.773, 0.404, 0.922) * 1.3 * n;

//     return float4(uv, 0, 1);
    return float4(result.rgb, 1.0);
}
