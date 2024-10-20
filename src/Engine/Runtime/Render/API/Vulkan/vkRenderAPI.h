#pragma once

#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include "../../IRenderAPI.h"
#include "../../RenderServer.h"
#include "../../../../../Vendor/vk-bootstrap/VkBootstrap.h"
#include "../../../Core/EngineConfiguration.h"
#include "../../../Core/EngineServer.h"

namespace NSE::Vulkan
{
    class vkRenderAPI : public IRenderAPI
    {
    public:
        explicit vkRenderAPI(EngineConfiguration config, SDL_Window* window);

        bool OnInitialize() override;
        void OnDispose() override;

    public:
        static vkRenderAPI* Get()
        {
            return dynamic_cast<vkRenderAPI*>(const_cast<IRenderAPI*>(sRender->api()));
        }

        static constexpr EngineServerAccessor<vkRenderAPI> inst;

    public:
        void Present() const override;
        void ClearRenderTargetColor(float4 color) const override;
        void ClearRenderTargetDepth(float depth) const override;
        void ClearRenderTargetStencil(int stencil) const override;

    public:
        [[nodiscard]] SRef<GraphicsBuffer>  CreateGraphicsBuffer(GraphicsBuffer::Target target, size_t size, bool keepDataOnCPU) const final;
        [[nodiscard]] SRef<GraphicsBuffer>  CreateGraphicsBuffer(GraphicsBuffer::Target target, size_t stride, size_t count, bool keepDataOnCPU) const final;
        [[nodiscard]] SRef<Pipeline>        CreatePipeline(const PipelineConfiguration &config) const final;

    public:
        [[nodiscard]] VkInstance    GetVulkanInstance() const { return _vkbInstance.instance; }
        [[nodiscard]] VkDevice      GetVulkanDevice() const { return _vkbDevice.device; }
        [[nodiscard]] vkb::Swapchain GetVkbSwapchain() const { return _vkbSwapchain; }
        [[nodiscard]] VkSwapchainKHR GetVulkanSwapchain() const { return _vkbSwapchain; }

    private:
        void NewImGuiFrame() const override;
        void RenderImGuiDrawData() const override;

    private:
        bool InitializeImGui();

    private:
        EngineConfiguration _config;

        GLuint _glProgramID = 0;
        SDL_GLContext _glContext = nullptr;
        SDL_Window* _window = nullptr;

        vkb::Instance _vkbInstance;
        vkb::Device _vkbDevice;
        vkb::PhysicalDevice _vkbPhysicalDevice;
        VkSurfaceKHR _vkSurface = nullptr;
        VkQueue _vkQueue = nullptr;
        vkb::Swapchain _vkbSwapchain;
        VkDescriptorPool _vkImGuiPool;
    };
}
