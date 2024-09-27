#pragma once

#include "../Core/EngineServer.h"
#include "../Core/EngineConfiguration.h"

namespace NSE
{
    class TimeServer : public EngineServer<TimeServer>
    {
    public:
        explicit TimeServer(EngineConfiguration cfg);
        ~TimeServer() override;

        bool OnInitialize() override;
        void OnDispose() override;

    private:
        void BeginFrame();
        void EndFrame();

    private:
        EngineConfiguration _config;

        friend Engine;
    };

    constexpr EngineServerAccessor<TimeServer> Time;
}