#include "vkRenderAPI.h"

#include "vkPipeline.h"
#include "vkShader.h"
#include "../../../../../Vendor/vk-bootstrap/VkBootstrap.h"
#include "../../../../../Vendor/ImGui/imgui.h"
#include "../../../../../Vendor/ImGui/imgui_impl_vulkan.h"
#include "../../../../../Vendor/ImGui/imgui_impl_sdl2.h"
#include "../../Pipeline.h"
#include "../../../Serializer/SerializerServer.h"

NSE::Vulkan::vkRenderAPI::vkRenderAPI(EngineConfiguration config, SDL_Window *window)
{
    _config = config;
    _window = window;
}

bool NSE::Vulkan::vkRenderAPI::OnInitialize()
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

    VkPhysicalDeviceVulkan13Features features13 = {};
    features13.dynamicRendering = true;

    vkb::PhysicalDeviceSelector selector{_vkbInstance};
    auto pDeviceResult = selector
                        .set_surface(_vkSurface)
                        .set_minimum_version (1, 3)
                        .set_required_features_13(features13)
                        .add_required_extension(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME)
                        .add_required_extension(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME)
                        .add_required_extension(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME)
                        .require_dedicated_transfer_queue()
                        .select();
    assert(("Failed to create Vulkan physical device", pDeviceResult));
    if (!pDeviceResult) {
        std::cerr << pDeviceResult.error().message() << std::endl;
        return false;
    }

    _vkbPhysicalDevice = pDeviceResult.value();

    vkb::DeviceBuilder deviceBuilder{_vkbPhysicalDevice};
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

    sSerializer->RegisterSerializer<vkShaderSerializer>();

    if (!InitializeImGui())
        return false;

    return true;
}

void NSE::Vulkan::vkRenderAPI::OnDispose()
{
    ImGui_ImplVulkan_DestroyFontsTexture();
    ImGui_ImplVulkan_Shutdown();

    vkDestroyDescriptorPool(_vkbDevice, _vkImGuiPool, nullptr);
    vkb::destroy_swapchain(_vkbSwapchain);
    vkb::destroy_surface(_vkbInstance, _vkSurface);
    vkb::destroy_device(_vkbDevice);
    vkb::destroy_instance(_vkbInstance);

    SDL_Vulkan_UnloadLibrary();
}

void NSE::Vulkan::vkRenderAPI::Present() const
{

}

void NSE::Vulkan::vkRenderAPI::ClearRenderTargetColor(float4 color) const
{

}

void NSE::Vulkan::vkRenderAPI::ClearRenderTargetDepth(float depth) const
{

}

void NSE::Vulkan::vkRenderAPI::ClearRenderTargetStencil(int stencil) const
{

}

NSE::SRef<NSE::GraphicsBuffer> NSE::Vulkan::vkRenderAPI::CreateGraphicsBuffer(GraphicsBuffer::Target target, size_t size,
    bool keepDataOnCPU) const
{
    return nullptr;
}

NSE::SRef<NSE::GraphicsBuffer> NSE::Vulkan::vkRenderAPI::CreateGraphicsBuffer(GraphicsBuffer::Target target, size_t stride,
    size_t count, bool keepDataOnCPU) const
{
    return nullptr;
}

NSE::SRef<NSE::Pipeline> NSE::Vulkan::vkRenderAPI::CreatePipeline(const PipelineConfiguration &config) const
{
    return SRef{new vkPipeline{config}};
}

void NSE::Vulkan::vkRenderAPI::NewImGuiFrame() const
{
    ImGui_ImplVulkan_NewFrame();
}

void NSE::Vulkan::vkRenderAPI::RenderImGuiDrawData() const
{
    // vkCmdBeginRenderingKHR()


    // vkCmdBeginRendering(cmd, &renderInfo);
    // ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
    // vkCmdEndRendering(cmd);
}

bool NSE::Vulkan::vkRenderAPI::InitializeImGui()
{
    VkDescriptorPoolSize poolSizes[] =
    {
        { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets = 1000;
    poolInfo.poolSizeCount = std::size(poolSizes);
    poolInfo.pPoolSizes = poolSizes;

    auto poolResult = vkCreateDescriptorPool(_vkbDevice, &poolInfo, nullptr, &_vkImGuiPool);

    assert(("Failed to create Vulkan descriptor pool", poolResult == VK_SUCCESS));
    if (poolResult != VK_SUCCESS)
    {
        return false;
    }

    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForVulkan(_window);

    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance = _vkbInstance;
    initInfo.PhysicalDevice = _vkbPhysicalDevice;
    initInfo.Device = _vkbDevice;
    initInfo.Queue = _vkQueue;
    initInfo.DescriptorPool = _vkImGuiPool;
    initInfo.MinImageCount = 3;
    initInfo.ImageCount = 3;
    initInfo.UseDynamicRendering = true;

    VkPipelineRenderingCreateInfoKHR pCreateInfo = {};
    pCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;

    initInfo.PipelineRenderingCreateInfo = pCreateInfo;
    initInfo.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
    initInfo.PipelineRenderingCreateInfo.pColorAttachmentFormats = &_vkbSwapchain.image_format;

    ImGui_ImplVulkan_Init(&initInfo);



    // immediate_submit([&](VkCommandBuffer cmd) {
    //     ImGui_ImplVulkan_CreateFontsTexture(cmd);
    // });

    return true;
}
