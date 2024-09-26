#pragma once

#include "../Core/EngineServer.h"
#include "../Core/EngineConfiguration.h"

namespace NSE
{
    class InputServer : public EngineServer<InputServer>
    {
    public:
        InputServer(EngineConfiguration cfg);
        ~InputServer() override;

        bool OnInitialize() override;
        void OnDispose() override;

    private:
        EngineConfiguration _config;
    };

    constexpr EngineServerAccessor<InputServer> Input;
}