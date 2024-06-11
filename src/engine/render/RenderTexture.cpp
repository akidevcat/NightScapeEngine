#include "RenderTexture.h"

#include <d3d11.h>
#include <iostream>

#include "../servers/RenderServer.h"

NSE::RenderTexture::RenderTexture(int width, int height, DXGI_FORMAT format, bool isDepthStencil) : Texture(width, height)
{
    _format = format;
    _isDepthStencil = isDepthStencil;

    auto device = RenderServer::Get()->GetDevice();

    HRESULT result;

    { // Create 2D Texture
        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(desc));

        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1; // ToDo
        desc.ArraySize = 1;
        desc.Format = format;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0; // ToDo? Depth Only?
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

        // ToDo ...
        if (desc.Format == DXGI_FORMAT_D24_UNORM_S8_UINT)
        {
            desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
        }

        if (isDepthStencil)
        {
            desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
        }
        else
        {
            desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        }

        ID3D11Texture2D* tex2D;

        result = device->CreateTexture2D(&desc, nullptr, &tex2D);
        assert(SUCCEEDED(result));

        _d3dResource = tex2D;
    }

    if (!isDepthStencil) { // Create Render Target View
        D3D11_RENDER_TARGET_VIEW_DESC desc;

        desc.Format = format;
        desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        desc.Texture2D.MipSlice = 0;

        result = device->CreateRenderTargetView(_d3dResource, &desc, &_renderTargetView);
        assert(SUCCEEDED(result));
    }

    { // Create Shader Resource View
        D3D11_SHADER_RESOURCE_VIEW_DESC desc;

        desc.Format = format;
        desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        desc.Texture2D.MostDetailedMip = 0;
        desc.Texture2D.MipLevels = 1;

        if (desc.Format == DXGI_FORMAT_D24_UNORM_S8_UINT)
        {
            desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS; // ToDo
        }

        result = device->CreateShaderResourceView(_d3dResource, &desc, &_d3dTextureView);
        assert(SUCCEEDED(result));
    }

    if (isDepthStencil) { // Create Depth Stencil View
        D3D11_DEPTH_STENCIL_VIEW_DESC desc;
        ZeroMemory(&desc, sizeof(desc));

        desc.Format = format;
        desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        desc.Texture2D.MipSlice = 0;

        result = device->CreateDepthStencilView(_d3dResource, &desc, &_depthStencilView);
        assert(SUCCEEDED(result));
    }
}

NSE::RenderTexture::~RenderTexture()
{
    RenderTexture::Release();
}

void NSE::RenderTexture::Release()
{
    Texture::Release();

    if(_depthStencilView)
    {
        _depthStencilView->Release();
        _depthStencilView = nullptr;
    }

    if(_renderTargetView)
    {
        _renderTargetView->Release();
        _renderTargetView = nullptr;
    }
}
