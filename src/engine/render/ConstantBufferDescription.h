#ifndef CONSTANTBUFFERDESCRIPTION_H
#define CONSTANTBUFFERDESCRIPTION_H

#include <d3d11shader.h>
#include <unordered_map>

class ConstantBufferDescription
{
public:
    ConstantBufferDescription() = delete;
    ConstantBufferDescription(ID3D11ShaderReflection* reflection, size_t bufferID);
    ~ConstantBufferDescription() = default;

    [[nodiscard]] D3D11_SHADER_BUFFER_DESC   GetDescription() const { return _description; }
    [[nodiscard]] bool GetVDescription(size_t nameID, D3D11_SHADER_VARIABLE_DESC& desc);


private:
    D3D11_SHADER_BUFFER_DESC _description = {};
    std::unordered_map<size_t, D3D11_SHADER_VARIABLE_DESC> _variableDescriptions = {};
};

#endif //CONSTANTBUFFERDESCRIPTION_H
