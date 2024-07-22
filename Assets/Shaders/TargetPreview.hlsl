#include "ShaderLibrary/Core.hlsl"
#include "ShaderLibrary/Lighting.hlsl"

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

    float4 result = float4(0.72, 0.2, 0.0, 1.0);

    float3 viewDir = normalize(input.positionRS.xyz);
    float ndotv = 1.0 - abs(dot(input.normalRS.xyz, viewDir));
    ndotv = pow(ndotv, 2.0);

    result *= saturate(dot(input.normalRS.xyz, float3(0, 0, -1)) * 0.25 + 0.75);
    result *= ndotv;
    result *= 2;

    return float4(result.rgb, 1.0);
}
