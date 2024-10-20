#pragma once

#include "Shader.h"
#include "PrimitiveTopology.h"
#include "CullMode.h"

namespace NSE
{
    class RenderPass;
}

namespace NSE
{
    struct PipelineConfiguration
    {
        Ref<Shader> shader = nullptr;
        Ref<RenderPass> renderPass = nullptr;
        // ToDo render target
        // ToDo vertex layout?
        // ToDo blend mode
        // ToDo depth test
        PrimitiveTopology topology = PrimitiveTopology::Triangles;
        CullMode cullMode = CullMode::Back;
        bool flEnableDepthClamp = false;
        bool flDiscardRasterizer = false;
        bool flEnableWireframeMode = false;
    };
}
