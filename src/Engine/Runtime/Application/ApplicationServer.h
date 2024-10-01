#pragma once

#include "../Core/EngineServer.h"
#include "../Core/EngineConfiguration.h"

namespace NSE
{
    class ApplicationServer : public EngineServer<ApplicationServer>
    {
    public:
        explicit ApplicationServer(EngineConfiguration cfg);
        ~ApplicationServer() override;

        bool OnInitialize() override;
        void OnDispose() override;

        bool Update();

    private:
        EngineConfiguration _config;
    };

    constexpr EngineServerAccessor<ApplicationServer> sApplication;
}