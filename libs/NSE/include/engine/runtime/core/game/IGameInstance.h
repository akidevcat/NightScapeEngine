#pragma once
#include <string>

namespace NSE
{
    class IGameInstance
    {
    public:
        virtual ~IGameInstance();

        virtual void OnGameLaunch() = 0;

        virtual std::string GetName() { return "Night Scape Engine"; }
    };
}
