#include "Material.h"

#include <iostream>

Material::Material(ID3D11Device* device, Shader* shader)
{
    _shader = shader;
    // ToDo Assert

    if (!shader->IsCompiled())
    {
        std::cerr << "Attempt to create a material with a non-compiled shader" << std::endl;
        return;
    }

    auto vMatProps = shader->GetVertexShader()->GetMaterialPropsLookup();
    auto pMatProps = shader->GetPixelShader()->GetMaterialPropsLookup();

    if (vMatProps)
    {
        _vsMaterialProps = new ConstBufferData{device, vMatProps->id, vMatProps->uid, vMatProps->description, vMatProps->description.Size};
        _vsMaterialProps->vDesc = vMatProps->vDesc;
    }
    if (pMatProps)
    {
        _psMaterialProps = new ConstBufferData{device, pMatProps->id, pMatProps->uid, pMatProps->description, pMatProps->description.Size};
        _psMaterialProps->vDesc = pMatProps->vDesc;
    }
}

Material::~Material()
{
    delete _vsMaterialProps;
    delete _psMaterialProps;
}

bool Material::SetVertexVar(size_t pUid, void *value, size_t valueSize)
{
    if (!_vsMaterialProps)
        return false;

    if (!_vsMaterialProps->SetVar(pUid, value, valueSize))
    {
        return false;
    }

    return true;
}

bool Material::SetPixelVar(size_t pUid, void *value, size_t valueSize)
{
    if (!_psMaterialProps)
        return false;

    if (!_psMaterialProps->SetVar(pUid, value, valueSize))
    {
        return false;
    }

    return true;
}

bool Material::UploadMaterialProperties(ID3D11DeviceContext *context)
{
    if (_vsMaterialProps)
        _shader->GetVertexShader()->UploadBuffer(context, _vsMaterialProps);
    if (_psMaterialProps)
        _shader->GetPixelShader()->UploadBuffer(context, _psMaterialProps);

    return true; // ToDo
}

void Material::UploadDrawProperties(ID3D11DeviceContext *context)
{
    _shader->UploadDrawProperties(context);
}

void Material::UploadGlobalProperties(ID3D11DeviceContext *context, ConstBufferData* globalProps)
{
    _shader->UploadGlobalProperties(context, globalProps);
}

void Material::UploadAllProperties(ID3D11DeviceContext *context, ConstBufferData* globalProps)
{
    UploadMaterialProperties(context);
    UploadDrawProperties(context);
    UploadGlobalProperties(context, globalProps);
}

void Material::EnumerateBuffers(_Out_ ID3D11Buffer* vsBuffers[3], _Out_ int& vsBuffersLength,
    _Out_ ID3D11Buffer* psBuffers[3], _Out_ int& psBuffersLength, ConstBufferData* globalProps) const
{
    // auto vsGlobalProps = _shader->GetVertexShader()->GetGlobalProps();
    // auto psGlobalProps = _shader->GetPixelShader()->GetGlobalProps();
    auto vsDrawProps = _shader->GetVertexShader()->GetDrawProps();
    auto psDrawProps = _shader->GetPixelShader()->GetDrawProps();

    vsBuffersLength = 0;
    psBuffersLength = 0;

    if (_shader->GetVertexShader()->HasGlobalProps())
    {
        // vsBuffers[vsBuffersLength] = vsGlobalProps->bPtr;
        vsBuffers[vsBuffersLength] = globalProps->bPtr;
        vsBuffersLength++;
    }
    if (_vsMaterialProps)
    {
        vsBuffers[vsBuffersLength] = _vsMaterialProps->bPtr;
        vsBuffersLength++;
    }
    if (vsDrawProps)
    {
        vsBuffers[vsBuffersLength] = vsDrawProps->bPtr;
        vsBuffersLength++;
    }

    if (_shader->GetPixelShader()->HasGlobalProps())
    {
        // psBuffers[psBuffersLength] = psGlobalProps->bPtr;
        psBuffers[psBuffersLength] = globalProps->bPtr;
        psBuffersLength++;
    }
    if (_psMaterialProps)
    {
        psBuffers[psBuffersLength] = _psMaterialProps->bPtr;
        psBuffersLength++;
    }
    if (psDrawProps)
    {
        psBuffers[psBuffersLength] = psDrawProps->bPtr;
        psBuffersLength++;
    }
}
