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
