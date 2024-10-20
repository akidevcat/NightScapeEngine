#pragma once
#include "IComponentFactory.h"

namespace NSE
{
    template <typename T>
    class GenericComponentFactory : public IComponentFactory
    {
    public:
        ~GenericComponentFactory() override = default;

    public:
        void AddComponentTo(Entity entity) override
        {
            entity.AddComponent<T>();
        }
    };
}
