#pragma once

#include <string>

namespace NSE
{
    class FactoryRegistry;

    class IAppInstance
    {
    public:
        virtual ~IAppInstance() = default;

        virtual void OnAppSetup() {}

        virtual void OnRegisteringComponents(Ref<FactoryRegistry> registry) {}
        virtual void OnRegisteringEntitySystems(Ref<FactoryRegistry> registry) {}
        virtual void OnRegisteringMainSystem() {}

        virtual void OnGameLaunch() = 0;

        virtual void OnDispose() {}

        virtual std::string GetName() { return "Night Scape Engine"; }
    };
}
