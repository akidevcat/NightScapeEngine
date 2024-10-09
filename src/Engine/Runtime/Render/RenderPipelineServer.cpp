#include "RenderPipelineServer.h"

#include "RenderServer.h"
#include "../Scene/SceneServer.h"
#include "ClusteredForward/ClusteredForwardRP.h"
#include "../../../Vendor/ImGui/imgui_impl_sdl2.h"

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

    ImGui::Render();

    sRender->api()->ClearRenderTargetColor(float4{1, 0, 1, 1});

    for (auto& scene : *SceneServer::Get())
    {
        auto view = scene->GetEntities<Components::Transform, Components::Camera>();
        for (const auto& [entity, transform, camera] : view.each())
        {
            _pipeline->Render(camera, transform);
        }
    }

    sRender->api()->RenderImGuiDrawData();

    sRender->api()->Present();
}
