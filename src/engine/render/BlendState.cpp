#include "BlendState.h"

#include "../servers/RenderServer.h"

NSE::BlendState::BlendState()
{
    D3D11_BLEND_DESC description;
    ZeroMemory(&description, sizeof(D3D11_BLEND_DESC));

    description.RenderTarget[0].BlendEnable = TRUE;
    description.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    description.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
    description.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    description.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    description.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    description.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    description.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    assert(("Unable to create blend state",
        SUCCEEDED(NSE::RenderServer::Get()->GetDevice()->CreateBlendState(&description, &_d3dObject))));
}

NSE::BlendState::BlendState(const D3D11_BLEND_DESC &description)
{
    assert(("Unable to create blend state",
        SUCCEEDED(NSE::RenderServer::Get()->GetDevice()->CreateBlendState(&description, &_d3dObject))));
}

NSE::BlendState::~BlendState()
{
    Release();
}

void NSE::BlendState::Release()
{
    if (_d3dObject)
    {
        _d3dObject->Release();
        _d3dObject = nullptr;
    }
}
