#pragma once

#include "vkRenderAPI.h"
#include "../../RenderPass.h"

namespace NSE::Vulkan
{
    class vkRenderPass : public RenderPass
    {
    public:
        vkRenderPass(const RenderPassConfiguration& configuration);
        ~vkRenderPass() override;

        void Initialize() override;
        void Dispose() override;

    private:
        bool _isInitialized = false;
        RenderPassConfiguration _configuration{};

        VkRenderPass _renderPass = nullptr;
    };
}