#pragma once

#include "../Core/EngineServer.h"
#include "../Core/EngineConfiguration.h"

namespace NSE
{
    class PhysicsServer : public EngineServer<PhysicsServer>
    {
    public:
        PhysicsServer(EngineConfiguration cfg);
        ~PhysicsServer() override;

        bool OnInitialize() override;
        void OnDispose() override;

    private:
        EngineConfiguration _config;
    };

    constexpr EngineServerAccessor<PhysicsServer> Physics;
}