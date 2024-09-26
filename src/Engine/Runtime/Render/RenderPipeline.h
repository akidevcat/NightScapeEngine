#pragma once

#include "Components/Camera.h"
#include "../Scene/Components/Transform.h"

namespace NSE
{

    class RenderPipelineServer;

    class RenderPipeline
    {
    public:
        virtual ~RenderPipeline() = 0;

    protected:
        virtual void OnLoad() {}
        virtual void OnUnload() {}

    public:
        virtual void Render(const Components::Camera& camera, const Components::Transform& transform) {}

        friend RenderPipelineServer;
    };
}
