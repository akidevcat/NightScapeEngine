#pragma once

// #include "../../../../Ref"
#include "Shader.h"
#include "PrimitiveTopology.h"

namespace NSE
{
    struct PipelineConfiguration
    {
        Ref<Shader> shader = nullptr;
        PrimitiveTopology topology = PrimitiveTopology::Triangles;
        bool depthTest = true;
        bool depthWrite = true;
        bool wireframeMode = false;
    };
}
