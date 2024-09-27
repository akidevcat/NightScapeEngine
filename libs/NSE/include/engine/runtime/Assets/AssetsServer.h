#pragma once

#include "../Core/EngineServer.h"
#include "../Core/EngineConfiguration.h"

namespace NSE
{
    class AssetsServer : public EngineServer<AssetsServer>
    {
    public:
        AssetsServer(EngineConfiguration cfg);
        ~AssetsServer() override;

        bool OnInitialize() override;
        void OnDispose() override;

    private:
        EngineConfiguration _config;
    };

    constexpr EngineServerAccessor<AssetsServer> sAssets;
}