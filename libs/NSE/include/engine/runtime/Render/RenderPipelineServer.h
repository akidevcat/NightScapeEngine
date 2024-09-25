#pragma once

#include "IRenderPipeline.h"
#include "../Core/EngineServer.h"

namespace NSE
{
    class RenderPipelineServer : public EngineServer<RenderPipelineServer>
    {
    public:
        RenderPipelineServer();
        ~RenderPipelineServer() override;

        bool OnInitialize() override;
        void OnDispose() override;

        void LoadPipeline(IRenderPipeline* pipeline);

    private:
        void RenderFrame();

    private:
        IRenderPipeline* _pipeline = nullptr;

        friend NSE::Engine;
    };

    constexpr EngineServerAccessor<RenderPipelineServer> RenderPipeline;
}