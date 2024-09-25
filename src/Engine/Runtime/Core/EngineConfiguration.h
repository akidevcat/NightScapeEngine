#pragma once

namespace NSE
{
    struct EngineConfiguration
    {
        enum class VSyncMode { None, Normal, Adaptive };
        enum class RenderAPI { OpenGL };

        VSyncMode vSyncMode = VSyncMode::Normal;
        RenderAPI renderAPI = RenderAPI::OpenGL;
    };
}