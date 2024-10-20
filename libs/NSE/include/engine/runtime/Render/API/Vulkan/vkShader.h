#pragma once

#include <vulkan/vulkan_core.h>

#include "../../Shader.h"
#include "../../../Serializer/ObjectSerializer.h"

namespace NSE::Vulkan
{
    class vkShaderSerializer;

    class vkShader : public Shader
    {
    public:
        vkShader();
        explicit vkShader(const std::string& shaderPath);
        ~vkShader() override;

        void Initialize();
        void Dispose();

        [[nodiscard]] bool initialized() const { return _isInitialized; }
        [[nodiscard]] const std::vector<VkPipelineShaderStageCreateInfo>& shaderStages() const { return _shaderStages; }

    protected:
        void OnLoadFromFile(const std::string &assetPath) override;

    private:
        static VkShaderModule CreateShaderModule(const std::string &shaderCode);

    private:
        bool _isInitialized = false;
        std::string _shaderCode{};

        std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;

        friend vkShaderSerializer;
    };

    class vkShaderSerializer : public ObjectSerializer<vkShader>
    {
        bool Deserialize(const std::string &textObject, vkShader *result) const override;
    };
}
