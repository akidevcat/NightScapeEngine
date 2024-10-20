#include "vkRenderPass.h"

NSE::Vulkan::vkRenderPass::vkRenderPass(const RenderPassConfiguration& configuration)
{
    _configuration = configuration;
}

NSE::Vulkan::vkRenderPass::~vkRenderPass()
{
    vkRenderPass::Dispose();
}

void NSE::Vulkan::vkRenderPass::Initialize()
{
    vkRenderPass::Dispose();
    //
    // auto device = vkRenderAPI::inst->GetVulkanDevice();
    // auto flResult = false;
    //
    // VkRenderPassCreateInfo renderPassInfo{};
    // renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    // renderPassInfo.attachmentCount = 1;
    // renderPassInfo.pAttachments = &colorAttachment;
    // renderPassInfo.subpassCount = 1;
    // renderPassInfo.pSubpasses = &subpass;
    //
    // flResult = vkCreateRenderPass(device, &renderPassInfo, nullptr, &_renderPass) == VK_SUCCESS;
    // assert(("Failed to create Vulkan render pass", flResult));
    //
    _isInitialized = true;
}

void NSE::Vulkan::vkRenderPass::Dispose()
{
    auto device = vkRenderAPI::inst->GetVulkanDevice();

    if (_renderPass)
    {
        vkDestroyRenderPass(device, _renderPass, nullptr);
        _renderPass = nullptr;
    }

    _isInitialized = false;
}
