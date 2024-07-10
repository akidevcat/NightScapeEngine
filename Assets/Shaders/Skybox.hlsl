#include "ShaderLibrary/Core.hlsl"
#include "ShaderLibrary/Hash.hlsl"
#include "ShaderLibrary/Noise.hlsl"

struct VertexInput
{
    float3 position : POSITION;
    float3 normal : POSITION;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 viewDir : TEXCOORD1;
};

PixelInput VertexMain(VertexInput input)
{
    PixelInput output;

    output.position = float4(input.position.x, input.position.y, 0.5, 1.0);

    output.uv = input.uv;
    output.uv.y = 1.0 - output.uv.y;
    output.viewDir = float4(TransformClipToWorld(output.position), 1.0);

    return output;
}

float4 PixelMain(PixelInput input) : SV_TARGET
{
    float3 result = 0;
    uint2 screenPos = GetScreenPos(input.position.xy);
    float3 dir = normalize(input.viewDir.xyz);

    float layer0 = VNoiseD3FBM(dir * 2.0, 4, 0.75, 2.3).x;
    layer0 = saturate(layer0 - 0.6) / 0.4;
    layer0 = Dither(layer0 * 0.6, screenPos, 5, 0.3);

    float layer1 = VNoiseD3FBM(dir * 2.0 + 48.214 + 1.0 * abs(VNoiseD3FBM(dir * 8.0, 4, 0.5, 1.5).x - 0.5), 3, 0.75, 3.0).x;
    layer1 = saturate(layer1 - 0.5) / 0.5;
//     layer1 = Dither(layer1 * 0.6, uint2(hash13(dir + 12.421) * 10000, hash13(dir + 56.123) * 10000), 5, 0.3);
//     layer1 *= VNoiseD3FBM(dir * 100.0 + 418.214, 1, 0.75, 2.3).x;
    layer1 = Dither(layer1, screenPos, 3, 0.05);

    result += layer1 * float3(0.616, 0.392, 0.871) * 0.05;
//     result = lerp(result, layer0 * float3(0.89, 0.153, 0.384) * 2.0, sign(layer0));
//     result += layer0 * float3(0.89, 0.153, 0.384);


//     n = Dither(n, screenPos);

    return float4(result.rgb, 1.0);
}
