#include "RenderTexture.h"

#include <d3d11.h>
#include <iostream>

#include "../servers/RenderServer.h"

NSE::RenderTexture::RenderTexture(int width, int height, DXGI_FORMAT colorFormat, DXGI_FORMAT depthStencilFormat)
{
    auto device = RenderServer::Get()->GetDevice();

    _width = width;
    _height = height;
    _colorFormat = colorFormat;
    _depthStencilFormat = depthStencilFormat;

    HRESULT result;

    // D3D11_TEXTURE2D_DESC textureDesc;
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    DXGI_FORMAT textureFormat;

    D3D11_TEXTURE2D_DESC textureDesc;

    ZeroMemory(&textureDesc, sizeof(textureDesc));

    // Setup the render target texture description.
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = colorFormat;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    result = device->CreateTexture2D(&textureDesc, nullptr, &_colorTexture);
    if(FAILED(result))
    {
        std::cerr << "Failed to create color texture" << std::endl << result;
        return;
    }

    renderTargetViewDesc.Format = textureDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;

    result = device->CreateRenderTargetView(_colorTexture, &renderTargetViewDesc, &_colorRTV);
    if(FAILED(result))
    {
        std::cerr << "Failed to create rtv" << std::endl;
        return;
    }

    shaderResourceViewDesc.Format = textureDesc.Format;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;

    result = device->CreateShaderResourceView(_colorTexture, &shaderResourceViewDesc, &_colorSRV);
    if(FAILED(result))
    {
        std::cerr << "Failed to create srv" << std::endl;
        return;
    }

    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
    depthBufferDesc.Width = width;
    depthBufferDesc.Height = height;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = depthStencilFormat;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    result = device->CreateTexture2D(&depthBufferDesc, nullptr, &_depthStencilTexture);
    if(FAILED(result))
    {
        std::cerr << "Failed to create depth stencil texture" << std::endl;
        return;
    }

    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

    depthStencilViewDesc.Format = depthStencilFormat;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    result = device->CreateDepthStencilView(_depthStencilTexture, &depthStencilViewDesc, &_depthStencilView);
    if(FAILED(result))
    {
        std::cerr << "Failed to create depth stencil view" << std::endl;
        return;
    }

    _viewport.Width = (float)width;
    _viewport.Height = (float)height;
    _viewport.MinDepth = 0.0f;
    _viewport.MaxDepth = 1.0f;
    _viewport.TopLeftX = 0;
    _viewport.TopLeftY = 0;
}

NSE::RenderTexture::~RenderTexture()
{
    Release();
}

void NSE::RenderTexture::Release()
{
    if(_depthStencilView)
    {
        _depthStencilView->Release();
        _depthStencilView = nullptr;
    }

    if(_depthStencilTexture)
    {
        _depthStencilTexture->Release();
        _depthStencilTexture = nullptr;
    }

    if(_colorSRV)
    {
        _colorSRV->Release();
        _colorSRV = nullptr;
    }

    if(_colorRTV)
    {
        _colorRTV->Release();
        _colorRTV = nullptr;
    }

    if(_colorTexture)
    {
        _colorTexture->Release();
        _colorTexture = nullptr;
    }
}
