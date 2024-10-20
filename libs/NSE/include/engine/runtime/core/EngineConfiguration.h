#pragma once

namespace NSE
{
    struct EngineConfiguration
    {
        enum class VSyncMode { None, Normal, Adaptive };
        enum class RenderAPI { OpenGL, Vulkan };
        enum class RenderPipeline { None, ClusteredForward };

        std::string name = "Night Scape Engine App";
        VSyncMode vSyncMode = VSyncMode::Normal;
        RenderAPI renderAPI = RenderAPI::Vulkan;
        RenderPipeline renderPipeline = RenderPipeline::ClusteredForward;
    };
}