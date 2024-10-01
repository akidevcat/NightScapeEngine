#pragma once

#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include "../../IRenderAPI.h"
#include "../../../../../Vendor/vk-bootstrap/VkBootstrap.h"
#include "../../../Core/EngineConfiguration.h"

namespace NSE
{
    class vkRenderAPI : public IRenderAPI
    {
    public:
        explicit vkRenderAPI(EngineConfiguration config, SDL_Window* window);

        bool OnInitialize() override;
        void OnDispose() override;

    public:
        void Present() const override;
        void ClearRenderTargetColor(float4 color) const override;
        void ClearRenderTargetDepth(float depth) const override;
        void ClearRenderTargetStencil(int stencil) const override;

    public:
        [[nodiscard]] SRef<GraphicsBuffer> CreateGraphicsBuffer(GraphicsBuffer::Target target, size_t size, bool keepDataOnCPU) const final;
        [[nodiscard]] SRef<GraphicsBuffer> CreateGraphicsBuffer(GraphicsBuffer::Target target, size_t stride, size_t count, bool keepDataOnCPU) const final;

    private:
        EngineConfiguration _config;

        GLuint _glProgramID = 0;
        SDL_GLContext _glContext = nullptr;
        SDL_Window* _window = nullptr;

        vkb::Instance _vkbInstance;
        vkb::Device _vkbDevice;
        VkSurfaceKHR _vkSurface = nullptr;
        VkQueue _vkQueue = nullptr;
        vkb::Swapchain _vkbSwapchain;
    };
}
