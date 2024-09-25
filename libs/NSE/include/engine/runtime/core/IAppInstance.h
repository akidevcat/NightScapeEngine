#pragma once
#include <string>

namespace NSE
{
    class IAppInstance
    {
    public:
        virtual ~IAppInstance() = default;

        virtual void OnGameLaunch() = 0;

        virtual std::string GetName() { return "Night Scape Engine"; }
    };
}
