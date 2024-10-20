#pragma once

namespace NSE::Components
{
    struct Light
    {
        float intensity = 1.0f;
        float scattering = 1.0f;
        float3 color = float3{1.0f, 1.0f, 1.0f};
    };
}