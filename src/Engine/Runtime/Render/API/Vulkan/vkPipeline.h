#pragma once

#include <vulkan/vulkan_core.h>

#include "../../Pipeline.h"

namespace NSE::Vulkan
{
    class vkPipeline : public Pipeline
    {
    public:
        vkPipeline(const PipelineConfiguration& config);
        ~vkPipeline() override;

        void Initialize() final;
        void Dispose() final;

        const PipelineConfiguration& configuration() const override { return _config; }

        VkPipelineLayout layout() const { return _layout; }
        VkPipeline pipeline() const { return _pipeline; }
        VkPipelineCache pipelineCache() const { return _pipelineCache; }

    private:
        PipelineConfiguration _config;
        bool _isInitialized = false;

        VkPipelineLayout _layout = nullptr;
        VkPipeline _pipeline = nullptr;
        VkPipelineCache _pipelineCache = nullptr;
    };
}