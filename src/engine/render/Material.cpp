#include "Material.h"
#include <iostream>
#include <cassert>

#include "../servers/RenderServer.h"
#include "../nsepch.h"

NSE::Material::Material(const NSE_Shader& shader)
{
    S_PID($Globals);
    S_PID(GlobalProperties);
    S_PID(DrawProperties);
    S_PID(LightsProperties);

    _shader = shader;

    assert(("Attempt to create a material with a non-compiled shader", shader->IsCompiled()));

    D3D11_SHADER_INPUT_BIND_DESC inputDescription{};
    bool flResult;

    if (shader->GetVertexShader()->HasMaterialProps())
    {
        flResult = shader->GetVertexShader()->GetInputsDescription()->GetDescription(PID_$Globals, inputDescription);
        assert(flResult);
        _materialPropertiesBuffer = CreateObject<GraphicsBuffer>(GraphicsBuffer::Target::Constant, 0, true);
        _materialPropertiesBuffer->ReflectAsConstantBuffer(shader->GetVertexShader()->GetReflection(), inputDescription.BindPoint, true);
        // _materialPropertiesBuffer->EnableBufferData(); // ToDo?
    }
    else if (shader->GetPixelShader()->HasMaterialProps())
    {
        flResult = shader->GetPixelShader()->GetInputsDescription()->GetDescription(PID_$Globals, inputDescription);
        assert(flResult);
        _materialPropertiesBuffer = CreateObject<GraphicsBuffer>(GraphicsBuffer::Target::Constant, 0, true);
        _materialPropertiesBuffer->ReflectAsConstantBuffer(shader->GetPixelShader()->GetReflection(), inputDescription.BindPoint, true);
        // _materialPropertiesBuffer->EnableBufferData(); // ToDo?
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

    SetConstantBuffer(PID_GlobalProperties, RenderServer::Get()->GetGlobalPropertiesBuffer());
    SetConstantBuffer(PID_DrawProperties, RenderServer::Get()->GetDrawPropertiesBuffer());
    SetConstantBuffer(PID_LightsProperties, RenderServer::Get()->GetLightsPropertiesBuffer());
    SetConstantBuffer(PID_$Globals, _materialPropertiesBuffer);

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
        _materialPropertiesBuffer->Upload();
    }
}

void NSE::Material::EnumerateVertexConstantBuffers(ID3D11Buffer* vsBuffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT], int &vsBuffersLength) const
{
    int length = 0;

    memset(vsBuffers, 0, sizeof(vsBuffers));

    NSE_GraphicsBuffer buffer;

    for (const auto& desc : *_shader->GetVertexShader()->GetInputsDescription())
    {
        length = max(length, desc.second.BindPoint + desc.second.BindCount);

        if (_vsInputs->GetConstantBuffer(desc.first, buffer))
        {
            vsBuffers[desc.second.BindPoint] = buffer->GetD3DBuffer();
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

    NSE_GraphicsBuffer buffer;

    for (const auto& desc : *_shader->GetPixelShader()->GetInputsDescription())
    {
        length = max(length, desc.second.BindPoint + desc.second.BindCount);

        if (_psInputs->GetConstantBuffer(desc.first, buffer))
        {
            psBuffers[desc.second.BindPoint] = buffer->GetD3DBuffer();
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
        result = _materialPropertiesBuffer->GetD3DVariableDescription(nameID, desc);
        if (result)
        {
            assert((UINT)valueSize == desc.Size);
            _materialPropertiesBuffer->Set(valuePtr, valueSize, desc.StartOffset);
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

void NSE::Material::SetConstantBuffer(size_t nameID, const NSE_GraphicsBuffer& buffer) const
{
    _vsInputs->SetConstantBuffer(nameID, buffer);
    _psInputs->SetConstantBuffer(nameID, buffer);
}

void NSE::Material::SetBuffer(size_t nameID, const NSE_GraphicsBuffer &buffer) const
{
    switch (buffer->GetTarget())
    {
        case GraphicsBuffer::Target::Constant:
            _vsInputs->SetConstantBuffer(nameID, buffer);
            _psInputs->SetConstantBuffer(nameID, buffer);
            break;
        case GraphicsBuffer::Target::Structured:
            _vsInputs->SetResource(nameID, buffer->GetD3DResourceView());
            _psInputs->SetResource(nameID, buffer->GetD3DResourceView());
            break;
        default:
            assert(false);
            break;
    }
}

// void NSE::Material::SetConstantBuffer(const NSE_ConstantBuffer& buffer) const
// {
//     _vsInputs->SetConstantBuffer(buffer->GetNameID(), buffer);
//     _psInputs->SetConstantBuffer(buffer->GetNameID(), buffer);
// }

void NSE::Material::SetTexture(size_t nameID, const NSE_Texture& texture) const
{
    _vsInputs->SetResource(nameID, texture->ResourceView());
    _psInputs->SetResource(nameID, texture->ResourceView());
}

void NSE::Material::SetBlendState(const NSE_BlendState& state)
{
    _blendState = state;
}
