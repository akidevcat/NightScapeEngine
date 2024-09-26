#pragma once

#include "../Core/EngineServer.h"
#include "../Core/EngineConfiguration.h"

namespace NSE
{
    class ProfilingServer : public EngineServer<ProfilingServer>
    {
    public:
        ProfilingServer(EngineConfiguration cfg);
        ~ProfilingServer() override;

        bool OnInitialize() override;
        void OnDispose() override;

    private:
        EngineConfiguration _config;
    };

    constexpr EngineServerAccessor<ProfilingServer> Profiling;
}