#include "ShaderLibrary/Core.hlsl"
#include "ShaderLibrary/Lighting.hlsl"

Texture2D   _MainTex;
float4      _MainTint;

DefaultPixelInput VertexMain(DefaultVertexInput input)
{
    DefaultPixelInput output;

    output.position = TransformObjectToClip(input.position);
    output.positionRS = TransformObjectToWorld(input.position);
    output.normal = TransformObjectToClipDirection(input.normal);
    output.normalRS = TransformObjectToWorldDirection(input.normal);
    output.uv = input.uv;

    return output;
}

float4 PixelMain(DefaultPixelInput input) : SV_TARGET
{
    float2 uv = input.uv;

    float4 result = float4(0.25, 0.17, 0.17, 1) * 1.25;
    result = 1;
    result *= _MainTex.Sample(_PointSampler, uv);

//     result *= saturate(dot(input.normal.xyz, float3(0, 0, -1)) * 0.5 + 0.5);
//     result *= GetBasicLighting(input.positionRS.xyz, input.normalRS.xyz);

    return float4(result.rgb, 1.0 + _Test);
}
