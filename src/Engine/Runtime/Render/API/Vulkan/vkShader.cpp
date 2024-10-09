#include "vkShader.h"

#include <vulkan/vulkan_core.h>

#include "vkRenderAPI.h"
#include "../../../Assets/AssetsServer.h"

NSE::Vulkan::vkShader::vkShader()
{

}

NSE::Vulkan::vkShader::vkShader(const std::string& shaderPath)
{
    LoadFromFile(shaderPath);
}

NSE::Vulkan::vkShader::~vkShader()
{

}

void NSE::Vulkan::vkShader::OnLoadFromFile(const std::string& shaderPath)
{
    Dispose();

    auto flLoadResult = sAssets->LoadSerializable<vkShader>(shaderPath, this);
    assert(("Failed to load vulkan shader from file", flLoadResult));

    Initialize();
}

void NSE::Vulkan::vkShader::Initialize()
{
    if (_isInitialized)
        return;

    auto shaderModule = CreateShaderModule(_shaderCode);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = shaderModule;
    vertShaderStageInfo.pName = "vertexMain";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = shaderModule;
    fragShaderStageInfo.pName = "fragmentMain";

    _shaderStages.emplace_back(vertShaderStageInfo);
    _shaderStages.emplace_back(fragShaderStageInfo);

    _isInitialized = true;
}

void NSE::Vulkan::vkShader::Dispose()
{
    _shaderCode = std::string{};
    _shaderStages.clear();

    _isInitialized = false;
}

VkShaderModule NSE::Vulkan::vkShader::CreateShaderModule(const std::string &shaderCode)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderCode.size();
    // ToDo align to 32 bits?
    createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.c_str());

    VkShaderModule result;
    auto flResult = vkCreateShaderModule(vkRenderAPI::Get()->GetVulkanDevice(), &createInfo, nullptr, &result) == VK_SUCCESS;
    assert(("Failed to create Vulkan shader module", flResult));
    if (!flResult)
        return nullptr;

    return result;
}

bool NSE::Vulkan::vkShaderSerializer::Deserialize(const std::string &textObject, vkShader *result) const
{
    result->_shaderCode = textObject;

    return true;
}
