#include "ConstantBufferDescription.h"

#include <cassert>

#include "ShaderUtils.h"

ConstantBufferDescription::ConstantBufferDescription(ID3D11ShaderReflection *reflection, size_t bufferID)
{
    auto bufferReflection = reflection->GetConstantBufferByIndex(bufferID);
    ID3D11ShaderReflectionVariable* varReflection = nullptr;

    assert(("Failed to get constant buffer description",
        SUCCEEDED(bufferReflection->GetDesc(&_description))));

    D3D11_SHADER_VARIABLE_DESC varDescription = {};

    for (UINT i = 0; i < _description.Variables; i++)
    {
        varReflection = bufferReflection->GetVariableByIndex(i);

        assert(("Failed to reflect constant buffer variable",
            SUCCEEDED(varReflection->GetDesc(&varDescription))));

        _variableDescriptions.emplace(NSE::ShaderUtils::PropertyToID(varDescription.Name), varDescription);
    }
}

bool ConstantBufferDescription::GetVDescription(size_t nameID, D3D11_SHADER_VARIABLE_DESC &desc)
{
    auto result = _variableDescriptions.find(nameID);
    if (result != _variableDescriptions.end())
    {
        desc = result->second;
        return true;
    }

    return false;
}
