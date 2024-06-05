#include "Material.h"

#include <iostream>
#include <cassert>

#include "../servers/RenderServer.h"

NSE::Material::Material(const NSE_Shader& shader)
{
    _shader = shader;

    assert(("Attempt to create a material with a non-compiled shader", shader->IsCompiled()));

    if (shader->GetVertexShader()->HasMaterialProps())
    {
        _vsMaterialPropertiesBuffer = new ConstantBufferData{shader->GetVertexShader()->GetMaterialPropertiesBufferSize()};
    }
    if (shader->GetPixelShader()->HasMaterialProps())
    {
        _psMaterialPropertiesBuffer = new ConstantBufferData{shader->GetPixelShader()->GetMaterialPropertiesBufferSize()};
    }

    _vsInputs = new ShaderInputsData{};
    _psInputs = new ShaderInputsData{};
}

NSE::Material::~Material()
{
    delete _vsMaterialPropertiesBuffer;
    delete _psMaterialPropertiesBuffer;
    delete _vsInputs;
    delete _psInputs;
}

void NSE::Material::Upload()
{
    if (_vsMaterialPropertiesBuffer)
    {
        _shader->GetVertexShader()->UploadMaterialProperties(_vsMaterialPropertiesBuffer);
    }
    if (_psMaterialPropertiesBuffer)
    {
        _shader->GetPixelShader()->UploadMaterialProperties(_psMaterialPropertiesBuffer);
    }
}

// bool NSE::Material::SetVertexVar(size_t pUid, void *value, size_t valueSize)
// {
//     if (!_vsMaterialProps)
//         return false;
//
//     if (!_vsMaterialProps->SetVar(pUid, value, valueSize))
//     {
//         return false;
//     }
//
//     return true;
// }

// bool NSE::Material::SetPixelVar(size_t pUid, void *value, size_t valueSize)
// {
//     if (!_psMaterialProps)
//         return false;
//
//     if (!_psMaterialProps->SetVar(pUid, value, valueSize))
//     {
//         return false;
//     }
//
//     return true;
// }

// void NSE::Material::SetVSResource(size_t uid, ID3D11ShaderResourceView *resource)
// {
//     if (!_vsMaterialProps)
//         return;
//
//     _vsMaterialProps->SetResourceView(uid, resource);
// }

// void NSE::Material::SetPSResource(size_t uid, ID3D11ShaderResourceView *resource)
// {
//     if (!_psMaterialProps)
//         return;
//
//     _psMaterialProps->SetResourceView(uid, resource);
// }

// bool NSE::Material::UploadMaterialProperties(ID3D11DeviceContext *context)
// {
//     if (_vsMaterialProps)
//         _shader->GetVertexShader()->UploadBuffer(context, _vsMaterialProps);
//     if (_psMaterialProps)
//         _shader->GetPixelShader()->UploadBuffer(context, _psMaterialProps);
//
//     return true; // ToDo
// }

// void NSE::Material::UploadDrawProperties(ConstBufferData* drawProps)
// {
//     _shader->UploadDrawProperties(drawProps);
// }

// void NSE::Material::UploadGlobalProperties(ConstBufferData* globalProps)
// {
//     _shader->UploadGlobalProperties(globalProps);
// }

// void NSE::Material::UploadAllProperties(ID3D11DeviceContext *context, ConstBufferData* globalProps, ConstBufferData* drawProps)
// {
//     UploadMaterialProperties(context);
//     UploadDrawProperties(drawProps);
//     UploadGlobalProperties(globalProps);
// }

// ToDo no need to keep it inside material definition
void NSE::Material::EnumerateBuffers(_Out_ ID3D11Buffer* vsBuffers[3], _Out_ int& vsBuffersLength,
    _Out_ ID3D11Buffer* psBuffers[3], _Out_ int& psBuffersLength,
    ConstantBuffer* globalPropertiesBuffer, ConstantBuffer* drawPropertiesBuffer) const
{
    vsBuffersLength = 0;
    psBuffersLength = 0;

    if (_shader->GetVertexShader()->HasGlobalProps())
    {
        vsBuffers[vsBuffersLength] = globalPropertiesBuffer->GetGPUBuffer();
        vsBuffersLength++;
    }
    if (_shader->GetVertexShader()->HasDrawProps())
    {
        vsBuffers[vsBuffersLength] = drawPropertiesBuffer->GetGPUBuffer();
        vsBuffersLength++;
    }
    if (_shader->GetVertexShader()->HasMaterialProps())
    {
        vsBuffers[vsBuffersLength] = _shader->GetVertexShader()->GetMaterialPropertiesBuffer()->GetGPUBuffer();
        vsBuffersLength++;
    }

    if (_shader->GetPixelShader()->HasGlobalProps())
    {
        psBuffers[psBuffersLength] = globalPropertiesBuffer->GetGPUBuffer();
        psBuffersLength++;
    }
    if (_shader->GetPixelShader()->HasDrawProps())
    {
        psBuffers[psBuffersLength] = drawPropertiesBuffer->GetGPUBuffer();
        psBuffersLength++;
    }
    if (_shader->GetPixelShader()->HasMaterialProps())
    {
        psBuffers[psBuffersLength] = _shader->GetPixelShader()->GetMaterialPropertiesBuffer()->GetGPUBuffer();
        psBuffersLength++;
    }
}

void NSE::Material::SetVar(const size_t nameID, void *valuePtr, const size_t valueSize) const
{
    D3D11_SHADER_VARIABLE_DESC desc;
    bool result;

    if (_vsMaterialPropertiesBuffer)
    {
        result = _shader->GetVertexShader()->GetMaterialPropertiesBuffer()->GetDescription()->GetVDescription(nameID, desc);
        if (result)
        {
            assert((UINT)valueSize == desc.Size);
            _vsMaterialPropertiesBuffer->Set(desc.StartOffset, valuePtr, valueSize);
        }
    }
    if (_psMaterialPropertiesBuffer)
    {
        result = _shader->GetPixelShader()->GetMaterialPropertiesBuffer()->GetDescription()->GetVDescription(nameID, desc);
        if (result)
        {
            assert((UINT)valueSize == desc.Size);
            _psMaterialPropertiesBuffer->Set(desc.StartOffset, valuePtr, valueSize);
        }
    }
}

void NSE::Material::SetFloat(const size_t nameID, float value) const
{
    SetVar(nameID, &value, sizeof(float));
}

void NSE::Material::SetInt(size_t nameID, int value) const
{
    SetVar(nameID, &value, sizeof(int));
}

void NSE::Material::SetUnsignedInt(const size_t nameID, uint32_t value) const
{
    SetVar(nameID, &value, sizeof(uint32_t));
}

void NSE::Material::SetVector(const size_t nameID, DirectX::XMVECTOR value) const
{
    SetVar(nameID, &value, sizeof(DirectX::XMVECTOR));
}

void NSE::Material::SetColor(const size_t nameID, DirectX::XMVECTOR value) const
{
    // ToDo convert to linear space
    SetVar(nameID, &value, sizeof(DirectX::XMVECTOR));
}

void NSE::Material::SetMatrix(const size_t nameID, DirectX::XMMATRIX value) const
{
    SetVar(nameID, &value, sizeof(DirectX::XMMATRIX));
}
