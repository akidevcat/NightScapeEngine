#include "Material.h"
#include <iostream>
#include <cassert>

#include "../servers/RenderServer.h"
#include "../nsepch.h"

NSE::Material::Material(const NSE_Shader& shader)
{
    S_PID($Globals);
    S_PID(DrawProperties);
    S_PID(MaterialProperties);

    _shader = shader;

    assert(("Attempt to create a material with a non-compiled shader", shader->IsCompiled()));

    D3D11_SHADER_INPUT_BIND_DESC inputDescription{};
    bool flResult;

    if (shader->GetVertexShader()->HasMaterialProps())
    {
        flResult = shader->GetVertexShader()->GetInputsDescription()->GetDescription(PID_$Globals, inputDescription);
        assert(flResult);
        _materialPropertiesBuffer = CreateObject<ConstantBuffer>(PID_$Globals);
        _materialPropertiesBuffer->Reflect(shader->GetVertexShader()->GetReflection(), inputDescription.BindPoint, true);
        _materialPropertiesBuffer->EnableBufferData(); // ToDo?
    }
    else if (shader->GetPixelShader()->HasMaterialProps())
    {
        flResult = shader->GetPixelShader()->GetInputsDescription()->GetDescription(PID_$Globals, inputDescription);
        assert(flResult);
        _materialPropertiesBuffer = CreateObject<ConstantBuffer>(PID_$Globals);
        _materialPropertiesBuffer->Reflect(shader->GetPixelShader()->GetReflection(), inputDescription.BindPoint, true);
        _materialPropertiesBuffer->EnableBufferData(); // ToDo?
    }

    // if (shader->GetVertexShader()->HasMaterialProps())
    // {
    //     _vsMaterialPropertiesBuffer = new ConstantBufferData{shader->GetVertexShader()->GetMaterialPropertiesBufferSize()};
    // }
    // if (shader->GetPixelShader()->HasMaterialProps())
    // {
    //     _psMaterialPropertiesBuffer = new ConstantBufferData{shader->GetPixelShader()->GetMaterialPropertiesBufferSize()};
    // }

    _vsInputs = new ShaderInputsData{};
    _psInputs = new ShaderInputsData{};

    SetConstantBuffer(RenderServer::Get()->GetGlobalPropertiesBuffer());
    SetConstantBuffer(RenderServer::Get()->GetDrawPropertiesBuffer());
    SetConstantBuffer(RenderServer::Get()->GetLightsPropertiesBuffer());
    // _vsInputs->SetConstantBuffer(shader->GetVertexShader()->GetMaterialPropertiesBuffer());
    // _psInputs->SetConstantBuffer(shader->GetPixelShader()->GetMaterialPropertiesBuffer());
    _vsInputs->SetConstantBuffer(_materialPropertiesBuffer);
    _psInputs->SetConstantBuffer(_materialPropertiesBuffer);

    SetBlendState(RenderServer::Get()->GetBlendStateOpaque());
}

NSE::Material::~Material()
{
    // delete _vsMaterialPropertiesBuffer;
    // delete _psMaterialPropertiesBuffer;
    delete _vsInputs;
    delete _psInputs;

    if (_blendState)
    {
        _blendState->Release();
        _blendState = nullptr;
    }

    DestroyObject(_materialPropertiesBuffer);
}

void NSE::Material::Upload()
{
    if (_materialPropertiesBuffer)
    {
        _materialPropertiesBuffer->UploadData();
    }
    // if (_vsMaterialPropertiesBuffer)
    // {
    //     _shader->GetVertexShader()->UploadMaterialProperties(_vsMaterialPropertiesBuffer);
    // }
    // if (_psMaterialPropertiesBuffer)
    // {
    //     _shader->GetPixelShader()->UploadMaterialProperties(_psMaterialPropertiesBuffer);
    // }
}

void NSE::Material::EnumerateVertexConstantBuffers(ID3D11Buffer* vsBuffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT], int &vsBuffersLength) const
{
    int length = 0;

    memset(vsBuffers, 0, sizeof(vsBuffers));

    NSE_ConstantBuffer buffer;

    for (const auto& desc : *_shader->GetVertexShader()->GetInputsDescription())
    {
        length = max(length, desc.second.BindPoint + desc.second.BindCount);

        if (_vsInputs->GetConstantBuffer(desc.first, buffer))
        {
            vsBuffers[desc.second.BindPoint] = buffer->GetGPUBuffer();
        }
        else
        {
            vsBuffers[desc.second.BindPoint] = nullptr;
        }
    }

    vsBuffersLength = length;
}

void NSE::Material::EnumeratePixelConstantBuffers(ID3D11Buffer* psBuffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT], int& psBuffersLength) const
{
    int length = 0;

    memset(psBuffers, 0, sizeof(psBuffers));

    NSE_ConstantBuffer buffer;

    for (const auto& desc : *_shader->GetPixelShader()->GetInputsDescription())
    {
        length = max(length, desc.second.BindPoint + desc.second.BindCount);

        if (_psInputs->GetConstantBuffer(desc.first, buffer))
        {
            psBuffers[desc.second.BindPoint] = buffer->GetGPUBuffer();
        }
        else
        {
            psBuffers[desc.second.BindPoint] = nullptr;
        }
    }

    psBuffersLength = length;
}

void NSE::Material::SetVar(const size_t nameID, void *valuePtr, const size_t valueSize) const
{
    D3D11_SHADER_VARIABLE_DESC desc;
    bool result;

    if (_materialPropertiesBuffer)
    {
        result = _materialPropertiesBuffer->GetDescription()->GetVDescription(nameID, desc);
        if (result)
        {
            assert((UINT)valueSize == desc.Size);
            _materialPropertiesBuffer->GetBufferData()->Set(desc.StartOffset, valuePtr, valueSize);
        }
    }

    // if (_vsMaterialPropertiesBuffer)
    // {
    //     result = _shader->GetVertexShader()->GetMaterialPropertiesBuffer()->GetDescription()->GetVDescription(nameID, desc);
    //     if (result)
    //     {
    //         assert((UINT)valueSize == desc.Size);
    //         _vsMaterialPropertiesBuffer->Set(desc.StartOffset, valuePtr, valueSize);
    //     }
    // }
    // if (_psMaterialPropertiesBuffer)
    // {
    //     result = _shader->GetPixelShader()->GetMaterialPropertiesBuffer()->GetDescription()->GetVDescription(nameID, desc);
    //     if (result)
    //     {
    //         assert((UINT)valueSize == desc.Size);
    //         _psMaterialPropertiesBuffer->Set(desc.StartOffset, valuePtr, valueSize);
    //     }
    // }
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

void NSE::Material::SetConstantBuffer(size_t nameID, const obj_ptr<NSE::ConstantBuffer>& buffer) const
{
    _vsInputs->SetConstantBuffer(nameID, buffer);
    _psInputs->SetConstantBuffer(nameID, buffer);
}

void NSE::Material::SetConstantBuffer(const NSE_ConstantBuffer& buffer) const
{
    _vsInputs->SetConstantBuffer(buffer->GetNameID(), buffer);
    _psInputs->SetConstantBuffer(buffer->GetNameID(), buffer);
}

void NSE::Material::SetTexture(size_t nameID, const NSE_Texture& texture) const
{
    _vsInputs->SetResource(nameID, texture->ResourceView());
    _psInputs->SetResource(nameID, texture->ResourceView());
}

void NSE::Material::SetBlendState(const NSE_BlendState& state)
{
    _blendState = state;
}
