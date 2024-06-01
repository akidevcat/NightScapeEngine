#include "Material.h"

#include <iostream>

#include "../servers/RenderServer.h"

NSE::Material::Material(const NSE_Shader& shader)
{
    auto device = RenderServer::Get()->GetDevice();

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

NSE::Material::~Material()
{
    delete _vsMaterialProps;
    delete _psMaterialProps;
}

bool NSE::Material::SetVertexVar(size_t pUid, void *value, size_t valueSize)
{
    if (!_vsMaterialProps)
        return false;

    if (!_vsMaterialProps->SetVar(pUid, value, valueSize))
    {
        return false;
    }

    return true;
}

bool NSE::Material::SetPixelVar(size_t pUid, void *value, size_t valueSize)
{
    if (!_psMaterialProps)
        return false;

    if (!_psMaterialProps->SetVar(pUid, value, valueSize))
    {
        return false;
    }

    return true;
}

void NSE::Material::SetVSResource(size_t uid, ID3D11ShaderResourceView *resource)
{
    if (!_vsMaterialProps)
        return;

    _vsMaterialProps->SetResourceView(uid, resource);
}

void NSE::Material::SetPSResource(size_t uid, ID3D11ShaderResourceView *resource)
{
    if (!_psMaterialProps)
        return;

    _psMaterialProps->SetResourceView(uid, resource);
}

bool NSE::Material::UploadMaterialProperties(ID3D11DeviceContext *context)
{
    if (_vsMaterialProps)
        _shader->GetVertexShader()->UploadBuffer(context, _vsMaterialProps);
    if (_psMaterialProps)
        _shader->GetPixelShader()->UploadBuffer(context, _psMaterialProps);

    return true; // ToDo
}

void NSE::Material::UploadDrawProperties(ConstBufferData* drawProps)
{
    _shader->UploadDrawProperties(drawProps);
}

void NSE::Material::UploadGlobalProperties(ConstBufferData* globalProps)
{
    _shader->UploadGlobalProperties(globalProps);
}

void NSE::Material::UploadAllProperties(ID3D11DeviceContext *context, ConstBufferData* globalProps, ConstBufferData* drawProps)
{
    UploadMaterialProperties(context);
    UploadDrawProperties(drawProps);
    UploadGlobalProperties(globalProps);
}

void NSE::Material::EnumerateBuffers(_Out_ ID3D11Buffer* vsBuffers[3], _Out_ int& vsBuffersLength,
    _Out_ ID3D11Buffer* psBuffers[3], _Out_ int& psBuffersLength, ConstBufferData* globalProps, ConstBufferData* drawProps) const
{
    vsBuffersLength = 0;
    psBuffersLength = 0;

    if (_shader->GetVertexShader()->HasGlobalProps())
    {
        vsBuffers[vsBuffersLength] = globalProps->bPtr;
        vsBuffersLength++;
    }
    if (_shader->GetVertexShader()->HasDrawProps())
    {
        vsBuffers[vsBuffersLength] = drawProps->bPtr;
        vsBuffersLength++;
    }
    if (_vsMaterialProps)
    {
        vsBuffers[vsBuffersLength] = _vsMaterialProps->bPtr;
        vsBuffersLength++;
    }

    if (_shader->GetPixelShader()->HasGlobalProps())
    {
        psBuffers[psBuffersLength] = globalProps->bPtr;
        psBuffersLength++;
    }
    if (_shader->GetPixelShader()->HasDrawProps())
    {
        psBuffers[psBuffersLength] = drawProps->bPtr;
        psBuffersLength++;
    }
    if (_psMaterialProps)
    {
        psBuffers[psBuffersLength] = _psMaterialProps->bPtr;
        psBuffersLength++;
    }
}
