#include "ShaderInputsData.h"

void ShaderInputsData::SetResource(size_t nameID, ID3D11ShaderResourceView *resource)
{
    auto result = _resources.find(nameID);
    if (result != _resources.end())
    {
        result->second = resource;
    }
    else
    {
        _resources.emplace(nameID, resource);
    }
}

void ShaderInputsData::SetSampler(size_t nameID, ID3D11SamplerState *sampler)
{
    auto result = _samplers.find(nameID);
    if (result != _samplers.end())
    {
        result->second = sampler;
    }
    else
    {
        _samplers.emplace(nameID, sampler);
    }
}

// void ShaderInputsData::SetBuffer(size_t nameID, ID3D11Buffer *buffer)
// {
//     auto result = _buffers.find(nameID);
//     if (result != _buffers.end())
//     {
//         result->second = buffer;
//     }
//     else
//     {
//         _buffers.emplace(nameID, buffer);
//     }
// }

bool ShaderInputsData::GetResource(size_t nameID, ID3D11ShaderResourceView *&resource)
{
    auto result = _resources.find(nameID);
    if (result != _resources.end())
    {
        resource = result->second;
        return true;
    }

    return false;
}

bool ShaderInputsData::GetSampler(size_t nameID, ID3D11SamplerState *&sampler)
{
    auto result = _samplers.find(nameID);
    if (result != _samplers.end())
    {
        sampler = result->second;
        return true;
    }

    return false;
}

// bool ShaderInputsData::GetBuffer(size_t nameID, ID3D11Buffer *&buffer)
// {
//     auto result = _buffers.find(nameID);
//     if (result != _buffers.end())
//     {
//         buffer = result->second;
//         return true;
//     }
//
//     return false;
// }

void ShaderInputsData::SetConstantBuffer(size_t nameID, const obj_ptr<NSE::ConstantBuffer>& buffer)
{
    auto result = _cbuffers.find(nameID);
    if (result != _cbuffers.end())
    {
        result->second = buffer;
    }
    else
    {
        _cbuffers.emplace(nameID, buffer);
    }
}

bool ShaderInputsData::GetConstantBuffer(size_t nameID, obj_ptr<NSE::ConstantBuffer> &buffer)
{
    auto result = _cbuffers.find(nameID);
    if (result != _cbuffers.end())
    {
        buffer = result->second;
        return true;
    }

    return false;
}
