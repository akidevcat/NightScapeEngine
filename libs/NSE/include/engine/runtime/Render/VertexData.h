#pragma once

#include "../Math/Math.h"

namespace NSE
{
    struct VertexData
    {
        float3 position = float3{0, 0, 0};
        float3 normal = float3{0, 0, 0};
        float2 uv = float2{0, 0};
        float4 color = float4{0, 0, 0, 0};
    };
}
