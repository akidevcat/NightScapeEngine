#include "Texture2D.h"

#include "../servers/RenderServer.h"

NSE::Texture2D::Texture2D(int width, int height, DXGI_FORMAT format) : Texture(width, height)
{
    _format = format;

    auto device = RenderServer::Get()->GetDevice();


}

NSE::Texture2D::Texture2D(int width, int height, ID3D11Resource *resource, ID3D11ShaderResourceView *view) : Texture(width, height)
{
    _d3dResource = resource;
    _d3dTextureView = view;
}

NSE::Texture2D::~Texture2D()
{
    Texture2D::Release();
}

void NSE::Texture2D::Upload()
{

}

void NSE::Texture2D::Release()
{
    Texture::Release();
}
