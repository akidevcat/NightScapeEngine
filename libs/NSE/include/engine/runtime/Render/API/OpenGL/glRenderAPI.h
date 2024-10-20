#pragma once

#include "../../IRenderAPI.h"
#include "../../../Core/EngineConfiguration.h"

namespace NSE
{
    class glRenderAPI : public IRenderAPI
    {
    public:
        explicit glRenderAPI(EngineConfiguration config, SDL_Window* window);

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
        void NewImGuiFrame() const override;
        void RenderImGuiDrawData() const override;

    private:
        EngineConfiguration _config;

        GLuint _glProgramID = 0;
        SDL_GLContext _glContext = nullptr;
        SDL_Window* _window = nullptr;
    };
}
