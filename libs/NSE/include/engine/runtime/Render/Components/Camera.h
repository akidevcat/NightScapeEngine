#pragma once

namespace NSE::Components
{
    struct Camera
    {
        enum class Type { Perspective, Orthographic };

        Type type = Type::Perspective;

        int priority = 0;

        float fieldOfView = 60.0f;
        float orthographicSize = 10.0f;
        float clipPlanesNear = 0.1f;
        float clipPlanesFar = 1000.0f;
    };
}