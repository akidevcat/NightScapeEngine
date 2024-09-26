#include "RenderPipelineServer.h"

#include "RenderServer.h"

NSE::RenderPipelineServer::RenderPipelineServer()
{
}

NSE::RenderPipelineServer::~RenderPipelineServer()
{
}

bool NSE::RenderPipelineServer::OnInitialize()
{
    return true;
}

void NSE::RenderPipelineServer::OnDispose()
{
}

void NSE::RenderPipelineServer::LoadPipeline(IRenderPipeline *pipeline)
{
    _pipeline = pipeline;
}

void NSE::RenderPipelineServer::RenderFrame()
{
    Render->api()->ClearRenderTargetColor(float4{1.0f});
    Render->api()->Present();
}
