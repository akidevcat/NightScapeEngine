#include "ShaderLibrary/Core.hlsl"

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float4 vsPosition : TEXCOORD1;
};

PixelInput VertexMain(DefaultVertexInput input)
{
    PixelInput output;

    output.position = TransformObjectToClip(input.position);
    output.normal = TransformObjectToViewDirection(input.normal);
    output.uv = input.uv;
    output.vsPosition = TransformObjectToView(input.position);

    return output;
}

float4 PixelMain(PixelInput input) : SV_TARGET
{
    float2 uv = input.uv;
//     float4 result = float4(uv.x, uv.y, 1.0 - uv.x, 1.0);
//     float4 result = float4(0.35, 0.26, 0.38, 1);

//     result *= saturate(dot(input.normal.xyz, float3(0, 0, -1)) * 0.5 + 0.5);
    float3 viewDir = normalize(input.vsPosition.xyz);
    float ndotv = 1.0 - abs(dot(input.normal.xyz, viewDir));
    ndotv = pow(ndotv, 4.0);

    float s0 = smoothstep(-4, 0, sin(_Time * 2.0));
    float s1 = smoothstep(-5, 0, sin((_Time + 57.21495) * 3.7));

    float intensity = ndotv * s0 * s1;

    intensity = step(0.3, intensity) * 0.5 + 0.5;

    return intensity * float4(0.72, 0.2, 0.0, 0.0) * 1.3;
}
