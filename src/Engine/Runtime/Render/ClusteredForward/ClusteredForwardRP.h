#pragma once

#include "../RenderPipeline.h"

namespace NSE
{
    class ClusteredForwardRP : public RenderPipeline
    {
    public:
        void OnLoad() override;
        void OnUnload() override;

        void Render(const Components::Camera &camera, const Components::Transform &transform) override;
    };
}
