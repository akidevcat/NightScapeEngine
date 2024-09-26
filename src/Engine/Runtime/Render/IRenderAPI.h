#pragma once

namespace NSE
{
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
    };
}