#include "ShaderLibrary/Core.hlsl"
#include "ShaderLibrary/Hash.hlsl"

Texture2D _MainTex;

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
};

PixelInput VertexMain(VertexInput input)
{
    PixelInput output;

    output.position = float4(input.position.x, input.position.y, 0.5, 1.0);

    output.uv = input.uv;
    output.uv.y = 1.0 - output.uv.y;

    return output;
}

float4 PixelMain(PixelInput input) : SV_TARGET
{
    float2 res = uint2(120, 90);
    float2 uv = input.uv * res + 0.5;
    float2 fl = floor(uv);
    float2 fr = frac(uv);
    float2 aa = fwidth(uv) * 0.25;
    fr = smoothstep(0.5 - aa, 0.5 + aa, fr);

    uv = (fl+fr-0.5) / res;
//     uv = (input.uv * (float2)res + 0.5);
//     uv = (uv - 0.5) / (float2)res;

    float4 result = _MainTex.Sample(_LinearSampler, uv);

//     float noise = hash12(input.uv * res + _Time);
//
//     noise = pow(noise, 15.0);
//     noise = saturate((noise - 0.94) * 10.0);
//     result.rgb += noise;

//     result.rgb = pow(result.rgb, 1.0 / 2.2);
//     result.rgb = round(result.rgb * 32.0) / 32.0;
//     result.rgb = pow(result.rgb, 2.2);

    return float4(result.rgb, 1.0);
}
