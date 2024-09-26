#pragma once

namespace NSE
{
    struct EngineConfiguration
    {
        enum class VSyncMode { None, Normal, Adaptive };
        enum class RenderAPI { OpenGL };
        enum class RenderPipeline { None, ClusteredForward };

        VSyncMode vSyncMode = VSyncMode::Normal;
        RenderAPI renderAPI = RenderAPI::OpenGL;
        RenderPipeline renderPipeline = RenderPipeline::ClusteredForward;
    };
}