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

    return output;
}

float4 PixelMain(DefaultPixelInput input) : SV_TARGET
{
    float2 uv = input.uv;
    uint2 screenPos = GetScreenPos(input.position.xy);

    float4 result = float4(0.25, 0.17, 0.17, 1) * 1.25;

    float lightIntensity = dot(input.normalRS.xyz, normalize(float3(1, 0, -0.7)));
    lightIntensity = Dither(lightIntensity, screenPos);

    result.rgb = lightIntensity * float3(0.4, 0.5, 0.9) * 1.3;



    return float4(result.rgb, 1.0);
}
