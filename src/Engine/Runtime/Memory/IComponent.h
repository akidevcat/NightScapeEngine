#pragma once

#include "FactoryRegistry.h"

namespace NSE
{
    template <typename T>
    class IComponent
    {
        FactoryRegistry::RegisterComponentFactory<T>();

    private:
        static ;
    };
}
