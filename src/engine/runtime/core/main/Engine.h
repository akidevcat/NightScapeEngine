#pragma once

#include "../game/IGameInstance.h"

namespace NSE
{
    class Engine
    {
    public:
        Engine(IGameInstance* game);
        ~Engine();

        void Initialize();
        void UpdateFrame();
        void Shutdown();

    private:
        IGameInstance* _game = nullptr;
    };
}
