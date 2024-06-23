#include "ShaderInputsDescription.h"

#include <cassert>

#include "ShaderUtils.h"

ShaderInputsDescription::ShaderInputsDescription(ID3D11ShaderReflection *reflection)
{
    D3D11_SHADER_DESC shaderDescription{};
    D3D11_SHADER_INPUT_BIND_DESC bindDescription{};

    auto result = reflection->GetDesc(&shaderDescription);

    assert(("Failed to reflect shader description", SUCCEEDED(result)));

    for (UINT bResIndex = 0; bResIndex < shaderDescription.BoundResources; bResIndex++)
    {
        result = reflection->GetResourceBindingDesc(bResIndex, &bindDescription);

        assert(("Failed to reflect resource binding description", SUCCEEDED(result)));

        // ToDo
        // if (bindDescription.Type != D3D_SIT_TEXTURE && bindDescription.Type != D3D_SIT_SAMPLER && bindDescription.Type != D3D_SIT_CBUFFER)
        // {
        //     continue;
        // }

        _bindDescriptions.emplace(NSE::ShaderUtils::PropertyToID(bindDescription.Name), bindDescription);
    }
}

bool ShaderInputsDescription::GetDescription(size_t nameID, D3D11_SHADER_INPUT_BIND_DESC &description)
{
    auto result = _bindDescriptions.find(nameID);
    if (result != _bindDescriptions.end())
    {
        description = result->second;
        return true;
    }

    return false;
}
