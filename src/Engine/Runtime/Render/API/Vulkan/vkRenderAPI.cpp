#include "vkRenderAPI.h"

#include "../../../../../Vendor/vk-bootstrap/VkBootstrap.h"

NSE::vkRenderAPI::vkRenderAPI(EngineConfiguration config, SDL_Window *window)
{
    _config = config;
    _window = window;
}

bool NSE::vkRenderAPI::OnInitialize()
{
    vkb::InstanceBuilder builder;
    auto instanceResult = builder
                    .set_app_name(_config.name.c_str())
                    .set_engine_name("Night Scape Engine")
                    .request_validation_layers()
                    .use_default_debug_messenger()
                    .build();
    assert(("Failed to create Vulkan instance", instanceResult));
    if (!instanceResult)
    {
        std::cerr << instanceResult.error().message() << std::endl;
        return false;
    }

    _vkbInstance = instanceResult.value();

    SDL_Vulkan_CreateSurface(_window, _vkbInstance, &_vkSurface);
    if (!_vkSurface)
    {
        std::cerr << "Problem occured while engine initialization: " << std::endl;
        std::cerr << "SDL failed to create a Vulkan surface:" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        return false;
    }

    vkb::PhysicalDeviceSelector selector{_vkbInstance};
    auto pDeviceResult = selector
                        .set_surface(_vkSurface)
                        .set_minimum_version (1, 1)
                        .require_dedicated_transfer_queue()
                        .select();
    assert(("Failed to create Vulkan physical device", pDeviceResult));
    if (!pDeviceResult) {
        std::cerr << pDeviceResult.error().message() << std::endl;
        return false;
    }

    vkb::DeviceBuilder deviceBuilder{pDeviceResult.value()};
    auto deviceResult = deviceBuilder.build();
    assert(("Failed to create Vulkan device", deviceResult));
    if (!deviceResult)
    {
        std::cerr << deviceResult.error().message() << std::endl;
        return false;
    }

    _vkbDevice = deviceResult.value();
    auto graphicsQueueResult = _vkbDevice.get_queue(vkb::QueueType::graphics);
    assert(("Failed to get Vulkan graphics queue", graphicsQueueResult));
    if (!graphicsQueueResult)
    {
        std::cerr << graphicsQueueResult.error().message() << std::endl;
        return false;
    }

    _vkQueue = graphicsQueueResult.value();

    vkb::SwapchainBuilder swapchainBuilder{_vkbDevice};
    auto swapchainResult = swapchainBuilder.build();
    assert(("Failed to create Vulkan swapchain", swapchainResult));
    if (!swapchainResult)
    {
        std::cerr << swapchainResult.error().message() << std::endl;
        return false;
    }

    _vkbSwapchain = swapchainResult.value();

    return true;
}

void NSE::vkRenderAPI::OnDispose()
{
    vkb::destroy_swapchain(_vkbSwapchain);
    vkb::destroy_surface(_vkbInstance, _vkSurface);
    vkb::destroy_device(_vkbDevice);
    vkb::destroy_instance(_vkbInstance);
}

void NSE::vkRenderAPI::Present() const
{

}

void NSE::vkRenderAPI::ClearRenderTargetColor(float4 color) const
{

}

void NSE::vkRenderAPI::ClearRenderTargetDepth(float depth) const
{

}

void NSE::vkRenderAPI::ClearRenderTargetStencil(int stencil) const
{

}

NSE::SRef<NSE::GraphicsBuffer> NSE::vkRenderAPI::CreateGraphicsBuffer(GraphicsBuffer::Target target, size_t size,
    bool keepDataOnCPU) const
{
    return nullptr;
}

NSE::SRef<NSE::GraphicsBuffer> NSE::vkRenderAPI::CreateGraphicsBuffer(GraphicsBuffer::Target target, size_t stride,
    size_t count, bool keepDataOnCPU) const
{
    return nullptr;
}
