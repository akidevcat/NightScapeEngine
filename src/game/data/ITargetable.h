#pragma once
#include <string>

#include "../../engine/math/Math.h"

class ITargetable
{
public:
    ITargetable() = default;
    virtual ~ITargetable() = default;

    virtual const NSE::Vector3d& GetTargetablePosition() = 0;
    virtual float4 GetTargetableColor() = 0;
    virtual const std::string& GetTargetableName() = 0;
};