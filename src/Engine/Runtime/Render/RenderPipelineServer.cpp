#include "RenderPipelineServer.h"

#include "RenderServer.h"
#include "ClusteredForward/ClusteredForwardRP.h"

NSE::RenderPipelineServer::RenderPipelineServer(const EngineConfiguration& config)
{
    _config = config;
}

NSE::RenderPipelineServer::~RenderPipelineServer()
{
}

bool NSE::RenderPipelineServer::OnInitialize()
{
    switch (_config.renderPipeline)
    {
        default:
        case EngineConfiguration::RenderPipeline::None:

            break;
        case EngineConfiguration::RenderPipeline::ClusteredForward:
            LoadPipeline(new ClusteredForwardRP{});
            break;
    }

    return true;
}

void NSE::RenderPipelineServer::OnDispose()
{
}

void NSE::RenderPipelineServer::LoadPipeline(RenderPipeline*&& pipeline)
{
    _pipeline = pipeline;
    _pipeline->OnLoad();
}

void NSE::RenderPipelineServer::UnloadPipeline()
{
    _pipeline->OnUnload();
    free(_pipeline);
    _pipeline = nullptr;
}

void NSE::RenderPipelineServer::RenderFrame()
{
    if (!_pipeline)
        return;

    Render->api()->ClearRenderTargetColor(float4{0, 1, 1, 1});
    Render->api()->Present();
}
