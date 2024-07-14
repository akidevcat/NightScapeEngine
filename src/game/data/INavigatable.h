#pragma once
#include "../../engine/math/Math.h"

class INavigatable
{
public:
    INavigatable() = default;
    virtual ~INavigatable() = default;

    float testValue = 5;
    float testValue2 = 159;

    virtual NSE::Vector3d GetNavigatablePosition() = 0;
    virtual float4 GetNavigatableColor() = 0;
};
