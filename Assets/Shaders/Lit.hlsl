#include "ShaderLibrary/Core.hlsl"
#include "ShaderLibrary/Lighting.hlsl"

TEXTURE2D(_MainTex);
// Texture2D   _MainTex;
float4      _MainTint;

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

    float4 result = _MainTex.Sample(_PointSampler, uv);


    result *= saturate(dot(input.normalRS.xyz, float3(0, 0, -1)) * 0.25 + 0.75);
//     result *= GetBasicLighting(input.positionRS.xyz, input.normalRS.xyz);

    return float4(result.rgb, 1.0);
}
