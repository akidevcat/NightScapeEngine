#include "Shader.h"

#include "../servers/RenderServer.h"

NSE::Shader::Shader(const wchar_t *path) : Shader(path, path) { }

NSE::Shader::Shader(const wchar_t *vsPath, const wchar_t *psPath)
{
    _vShader = new VertexShader{vsPath};
    _pShader = new PixelShader{psPath};
}

NSE::Shader::~Shader()
{
    delete _vShader;
    delete _pShader;
}

void NSE::Shader::Release()
{
    if (_vShader)
        _vShader->Release();

    if (_pShader)
        _pShader->Release();
}

bool NSE::Shader::Compile()
{
    auto device = RenderServer::Get()->GetDevice();

    if (_vShader)
        if (!_vShader->Compile(device))
            return false;

    if (_pShader)
        if (!_pShader->Compile(device))
            return false;

    _isCompiled = true;
    return true;
}

void NSE::Shader::UploadDrawProperties(ConstBufferData* drawProps)
{
    auto context = RenderServer::Get()->GetDeviceContext();

    if (_vShader)
    {
        _vShader->UploadBuffer(context, drawProps);
    }

    if (_pShader)
    {
        _pShader->UploadBuffer(context, drawProps);
    }
}

void NSE::Shader::UploadGlobalProperties(ConstBufferData* globalProps)
{
    auto context = RenderServer::Get()->GetDeviceContext();

    if (_vShader)
    {
        _vShader->UploadBuffer(context, globalProps);
    }

    if (_pShader)
    {
        _pShader->UploadBuffer(context, globalProps);
    }
}