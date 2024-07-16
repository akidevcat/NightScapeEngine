#pragma once
#include "../../engine/math/Math.h"

class INavigatable
{
public:
    INavigatable() = default;
    virtual ~INavigatable() = default;

    virtual const NSE::Vector3d& GetNavigatablePosition() = 0;
    virtual float4 GetNavigatableColor() = 0;
    virtual float GetMasslockRadius() = 0;
};
