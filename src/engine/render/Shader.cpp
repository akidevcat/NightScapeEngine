#include "Shader.h"

Shader::Shader(const wchar_t *path) : Shader(path, path) { }

Shader::Shader(const wchar_t *vsPath, const wchar_t *psPath)
{
    _vShader = new VertexShader{vsPath};
    _pShader = new PixelShader{psPath};
}

Shader::~Shader()
{
    delete _vShader;
    delete _pShader;
}

void Shader::Release()
{
    if (_vShader)
        _vShader->Release();

    if (_pShader)
        _pShader->Release();
}

bool Shader::Compile(ID3D11Device *device)
{
    if (_vShader)
        if (!_vShader->Compile(device))
            return false;

    if (_pShader)
        if (!_pShader->Compile(device))
            return false;

    _isCompiled = true;
    return true;
}

void Shader::UploadDrawProperties(ID3D11DeviceContext *context, ConstBufferData* drawProps)
{
    if (_vShader)
    {
        _vShader->UploadBuffer(context, drawProps);
    }

    if (_pShader)
    {
        _pShader->UploadBuffer(context, drawProps);
    }
}

void Shader::UploadGlobalProperties(ID3D11DeviceContext *context, ConstBufferData* globalProps)
{
    if (_vShader)
    {
        _vShader->UploadBuffer(context, globalProps);
    }

    if (_pShader)
    {
        _pShader->UploadBuffer(context, globalProps);
    }
}