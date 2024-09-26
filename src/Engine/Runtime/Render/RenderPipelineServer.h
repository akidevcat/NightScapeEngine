#pragma once

#include "RenderPipeline.h"
#include "../Core/EngineConfiguration.h"
#include "../Core/EngineServer.h"

namespace NSE
{
    class RenderPipelineServer : public EngineServer<RenderPipelineServer>
    {
    public:
        explicit RenderPipelineServer(const EngineConfiguration& config);
        ~RenderPipelineServer() override;

        bool OnInitialize() override;
        void OnDispose() override;

        void LoadPipeline(RenderPipeline*&& pipeline);
        void UnloadPipeline();

    private:
        void RenderFrame();

    private:
        EngineConfiguration _config;
        RenderPipeline* _pipeline = nullptr;

        friend NSE::Engine;
    };

    constexpr EngineServerAccessor<RenderPipelineServer> Pipeline;
}