#ifndef RENDERTEXTURE_H
#define RENDERTEXTURE_H

#include "../obj_ptr.h"
#include <d3d11.h>
#include <dxgiformat.h>

#include "Texture.h"

#define NSE_RenderTexture obj_ptr<NSE::RenderTexture>

namespace NSE
{
    class RenderTexture : public Texture
    {
    public:
        RenderTexture() = delete;
        RenderTexture(int width, int height, DXGI_FORMAT format, bool isDepthStencil);
        ~RenderTexture() override;

        void Release() override;

        [[nodiscard]] ID3D11RenderTargetView* RenderTargetView() const { assert(!_isDepthStencil); return _renderTargetView; }
        [[nodiscard]] ID3D11DepthStencilView* DepthStencilView() const { assert( _isDepthStencil); return _depthStencilView; }

    protected:
        bool _isDepthStencil = false;
        ID3D11RenderTargetView* _renderTargetView = nullptr;
        ID3D11DepthStencilView* _depthStencilView = nullptr;
    };
}

#endif //RENDERTEXTURE_H
