#pragma once

#include "GraphicsBuffer.h"
#include "Pipeline.h"
#include "RenderPass.h"

namespace NSE
{
    class RenderServer;
    class RenderPipelineServer;

    class IRenderAPI
    {
    public:
        virtual ~IRenderAPI() = default;

        virtual bool OnInitialize() { return true; }
        virtual void OnDispose() {}

    public:
        virtual void Present() const = 0;
        virtual void ClearRenderTargetColor(float4 color) const = 0;
        virtual void ClearRenderTargetDepth(float depth) const = 0;
        virtual void ClearRenderTargetStencil(int stencil) const = 0;

    /*
     * Constructors
     */
    public:
        [[nodiscard]] virtual SRef<GraphicsBuffer>      CreateGraphicsBuffer    (GraphicsBuffer::Target target, size_t size, bool keepDataOnCPU) const = 0;
        [[nodiscard]] virtual SRef<GraphicsBuffer>      CreateGraphicsBuffer    (GraphicsBuffer::Target target, size_t stride, size_t count, bool keepDataOnCPU) const = 0;
        [[nodiscard]] virtual SRef<Pipeline>            CreatePipeline          (const PipelineConfiguration &config) const { return nullptr; }
        [[nodiscard]] virtual SRef<RenderPass>          CreateRenderPass        (const RenderPassConfiguration& configuration) const { return nullptr; }
        [[nodiscard]] virtual SRef<Shader>              CreateShader            (const std::string& filePath) const { return nullptr; }

    private:
        virtual void NewImGuiFrame() const = 0;
        virtual void RenderImGuiDrawData() const = 0;

        friend RenderServer;
        friend RenderPipelineServer;
    };
}
