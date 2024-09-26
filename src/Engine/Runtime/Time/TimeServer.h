#pragma once

#include "../Core/EngineServer.h"
#include "../Core/EngineConfiguration.h"

namespace NSE
{
    class TimeServer : public EngineServer<TimeServer>
    {
    public:
        TimeServer(EngineConfiguration cfg);
        ~TimeServer() override;

        bool OnInitialize() override;
        void OnDispose() override;

    private:
        EngineConfiguration _config;
    };

    constexpr EngineServerAccessor<TimeServer> Time;
}