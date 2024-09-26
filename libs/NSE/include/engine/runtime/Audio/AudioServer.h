#pragma once

#include "../Core/EngineServer.h"
#include "../Core/EngineConfiguration.h"

namespace NSE
{
    class AudioServer : public EngineServer<AudioServer>
    {
    public:
        AudioServer(EngineConfiguration cfg);
        ~AudioServer() override;

        bool OnInitialize() override;
        void OnDispose() override;

    private:
        EngineConfiguration _config;
    };

    constexpr EngineServerAccessor<AudioServer> Audio;
}