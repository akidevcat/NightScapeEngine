#include "vkPipeline.h"

#include "vkRenderAPI.h"
#include "vkShader.h"
#include "../../RenderServer.h"

NSE::Vulkan::vkPipeline::vkPipeline(const PipelineConfiguration &config)
{
    _config = config;

    auto device = vkRenderAPI::inst->GetVulkanDevice();
    auto shader = config.shader.As<vkShader>();

    // VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
    // pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    // pPipelineLayoutCreateInfo.pNext = nullptr;
    // pPipelineLayoutCreateInfo.setLayoutCount = (uint32_t)descriptorSetLayouts.size();
    // pPipelineLayoutCreateInfo.pSetLayouts = descriptorSetLayouts.data();
    // pPipelineLayoutCreateInfo.pushConstantRangeCount = (uint32_t)vulkanPushConstantRanges.size();
    // pPipelineLayoutCreateInfo.pPushConstantRanges = vulkanPushConstantRanges.data();
}

NSE::Vulkan::vkPipeline::~vkPipeline()
{

}
