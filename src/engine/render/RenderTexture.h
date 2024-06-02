#ifndef RENDERTEXTURE_H
#define RENDERTEXTURE_H

#include "../obj_ptr.h"
#include <d3d11.h>
#include <dxgiformat.h>

#include "../entity/Object.h"

#define NSE_RenderTexture obj_ptr<NSE::RenderTexture>

namespace NSE
{
    class RenderTexture : public Object
    {
    public:
        RenderTexture() = delete;
        RenderTexture(int width, int height, DXGI_FORMAT colorFormat, DXGI_FORMAT depthStencilFormat);
        ~RenderTexture() override;

        void Release();

        [[nodiscard]] DXGI_FORMAT GetColorFormat() const { return _colorFormat; }
        [[nodiscard]] DXGI_FORMAT GetDepthStencilFormat() const { return _depthStencilFormat; }
        [[nodiscard]] ID3D11RenderTargetView* GetRTV() const { return _colorRTV; }
        [[nodiscard]] ID3D11ShaderResourceView* GetSRV() const { return _colorSRV; }
        [[nodiscard]] ID3D11DepthStencilView* GetDepthStencilView() const { return _depthStencilView; }
        [[nodiscard]] int GetWidth() const { return _width; }
        [[nodiscard]] int GetHeight() const { return _height; }

    private:
        int _width = 1;
        int _height = 1;
        DXGI_FORMAT _colorFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
        DXGI_FORMAT _depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
        ID3D11Texture2D* _colorTexture = nullptr;
        ID3D11Texture2D* _depthStencilTexture = nullptr;
        ID3D11RenderTargetView* _colorRTV = nullptr;
        ID3D11ShaderResourceView* _colorSRV = nullptr;
        ID3D11DepthStencilView* _depthStencilView = nullptr;
        D3D11_VIEWPORT _viewport = {};
    };
}

#endif //RENDERTEXTURE_H
