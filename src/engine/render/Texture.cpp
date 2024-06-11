#include "Texture.h"

NSE::Texture::Texture(int width, int height)
{
    _width = width;
    _height = height;
}

NSE::Texture::~Texture()
{
    Texture::Release();
}

void NSE::Texture::Release()
{
    if (_d3dTextureView)
    {
        _d3dTextureView->Release();
        _d3dTextureView = nullptr;
    }

    if (_d3dResource)
    {
        _d3dResource->Release();
        _d3dResource = nullptr;
    }

    if (_data)
    {
        free(_data);
    }
}
