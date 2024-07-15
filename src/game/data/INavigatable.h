#pragma once
#include "../../engine/math/Math.h"

class INavigatable
{
public:
    INavigatable() = default;
    virtual ~INavigatable() = default;

    virtual const NSE::Vector3d& GetNavigatablePosition() = 0;
    virtual const float4& GetNavigatableColor() = 0;
    virtual const float GetMasslockRadius() = 0;
};
