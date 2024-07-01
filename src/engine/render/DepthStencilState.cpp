#include "DepthStencilState.h"

#include "../servers/RenderServer.h"

NSE::DepthStencilState::DepthStencilState()
{
    D3D11_DEPTH_STENCIL_DESC description;
    ZeroMemory(&description, sizeof(description));

    description.DepthEnable = true;
    description.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    description.DepthFunc = D3D11_COMPARISON_LESS;
    description.StencilEnable = true;
    description.StencilReadMask = 0xFF;
    description.StencilWriteMask = 0xFF;
    description.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    description.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    description.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    description.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    description.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    description.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    description.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    description.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    auto result = RenderServer::Get()->GetDevice()->CreateDepthStencilState(&description, &_d3dObject);

    assert(("Unable to create blend state",
        SUCCEEDED(result)));
}

NSE::DepthStencilState::DepthStencilState(const D3D11_DEPTH_STENCIL_DESC& description)
{
    auto result = RenderServer::Get()->GetDevice()->CreateDepthStencilState(&description, &_d3dObject);

    assert(("Unable to create blend state",
        SUCCEEDED(result)));
}

NSE::DepthStencilState::~DepthStencilState()
{
    Release();
}

void NSE::DepthStencilState::Release()
{
    if (_d3dObject)
    {
        _d3dObject->Release();
        _d3dObject = nullptr;
    }
}
