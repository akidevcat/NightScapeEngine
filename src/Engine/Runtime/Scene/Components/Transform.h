#pragma once

namespace NSE::Components
{
    struct Transform
    {
        double3 position = double3{0,0,0};
        quaternion rotation = QUATERNION_IDENTITY;
        float3 scale = float3{1,1,1};
    };
}
