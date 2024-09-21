#pragma once

namespace NSE
{
    class IGameInstance
    {
    public:
        virtual ~IGameInstance();

        virtual void OnGameLaunch() = 0;
    };
}
