#pragma once

namespace NSE
{
    struct EngineConfiguration
    {
        enum class VSyncMode { None, Normal, Adaptive };

        VSyncMode vSyncMode = VSyncMode::Normal;
    };
}