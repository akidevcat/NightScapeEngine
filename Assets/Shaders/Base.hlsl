#include "ShaderLibrary/Core.hlsl"
#include "ShaderLibrary/Lighting.hlsl"

Texture2D _MainTex;

DefaultPixelInput VertexMain(DefaultVertexInput input)
{
    DefaultPixelInput output;

    output.position = TransformObjectToClip(input.position);
    output.positionRS = TransformObjectToWorld(input.position);
    output.normal = TransformObjectToClipDirection(input.normal);
    output.normalRS = TransformObjectToWorldDirection(input.normal);
    output.uv = input.uv;
    output.uv.y = 1.0 - output.uv.y;

    return output;
}

float4 PixelMain(DefaultPixelInput input) : SV_TARGET
{
    float2 uv = input.uv;
    float2 screenUv = GetScreenUv(input.position.xy);
    uint2 screenPos = GetScreenPos(input.position.xy);
//     return screenPos.x % 2 == 0;
//     return float4(screenUv, 0, 1);

    float4 albedo = float4(0.7, 0.7, 0.7, 1);
    float4 ambient = float4(0.09, 0.09, 0.13, 1) * 0.2;

    float4 result = albedo * ambient; // ambient
//     result = 1;
//     result *= _MainTex.Sample(_PointSampler, uv);

//     result *= saturate(dot(input.normal.xyz, float3(0, 0, -1)) * 0.5 + 0.5);
    result += albedo * GetBasicLighting(input.positionRS.xyz, input.normalRS.xyz, screenPos);

    return float4(result.rgb, 1.0);
}
