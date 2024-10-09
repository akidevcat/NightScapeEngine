#pragma once

#include "../../Pipeline.h"

namespace NSE::Vulkan
{
    class vkPipeline : public Pipeline
    {
    public:
        vkPipeline(const PipelineConfiguration& config);
        ~vkPipeline() override;

        PipelineConfiguration& GetConfiguration() override { return _config; }
        const PipelineConfiguration& GetConfiguration() const override { return _config; }

    private:
        PipelineConfiguration _config;
    };
}