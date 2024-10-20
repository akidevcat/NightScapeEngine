#pragma once

#include "../Scene/Entity.h"

namespace NSE
{
    class IComponentFactory
    {
    public:
        IComponentFactory() = default;
        virtual ~IComponentFactory() = default;

    public:
        virtual void AddComponentTo(Entity entity) = 0;
    };
}
